#include "exporter.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libswscale/swscale.h"
}

#include "GL/gl.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <utility>

// wrapper for AVFrame with RGBA image content
struct RGBAImage
{
    AVFrame *frame;

    RGBAImage(int width, int height, std::vector<uint8_t> pixels)
    {
        // Allocate frames
        if ((frame = av_frame_alloc()) == NULL)
            throw std::runtime_error("could not create avcodec image.");
        frame->format = AV_PIX_FMT_RGBA;
        frame->width = width;
        frame->height = height;

        if (av_image_alloc(frame->data, frame->linesize, width, height, AV_PIX_FMT_RGBA, 32) < 0)
            throw std::runtime_error("could not allocate image data buffer.");

        for (int y = 0; y < height; y++)
            std::memcpy(frame->data[0] + (y * frame->linesize[0]), pixels.data() + y * width * 4, width * 4);
    }

    operator AVFrame *() { return frame; }
    AVFrame *operator->() { return frame; }

    ~RGBAImage()
    {
        av_freep(&frame->data[0]);
        av_frame_free(&frame);
    }
};

struct YUVImage
{
    AVFrame *yuv_frame;

    YUVImage(int width, int height)
    {
        if ((yuv_frame = av_frame_alloc()) == NULL)
            throw std::runtime_error("could not create avcodec frame.");
        yuv_frame->format = AV_PIX_FMT_YUV420P;
        yuv_frame->width = width;
        yuv_frame->height = height;
        av_frame_get_buffer(yuv_frame, 0);
    }

    operator AVFrame *() { return yuv_frame; }
    AVFrame *operator->() { return yuv_frame; }

    AVFrame *&getFrame()
    {
        return yuv_frame;
    }

    ~YUVImage() { av_frame_free(&yuv_frame); }
};

struct CodecPacket
{
    AVPacket *packet;

    CodecPacket()
    {
        if ((packet = av_packet_alloc()) == NULL)
            throw std::runtime_error("could not create avcodec packet.");
        packet->data = nullptr;
        packet->size = 0;
    }

    operator AVPacket *() { return packet; }
    AVPacket *operator->() { return packet; }

    ~CodecPacket()
    {
        av_packet_unref(packet);
        av_packet_free(&packet);
    }
};

void saveToPNG(std::string filename, int width, int height, std::vector<uint8_t> pixels)
{
    const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_PNG);
    if (codec == NULL)
        throw std::runtime_error("could not found PNG codec.");

    AVCodecContext *context = avcodec_alloc_context3(codec);
    if (context == NULL)
        throw std::runtime_error("could not allocate codec context.");

    context->width = width;
    context->height = height;
    context->pix_fmt = AV_PIX_FMT_RGBA;
    context->time_base = AVRational{1, 25}; // doesn't matter here

    if (avcodec_open2(context, codec, nullptr) < 0)
    {
        avcodec_free_context(&context);
        throw std::runtime_error("could not open codec.");
    }

    RGBAImage frame(width, height, pixels);

    CodecPacket pkt;
    if (avcodec_send_frame(context, frame) != 0 /* success */ ||
        avcodec_receive_packet(context, pkt) != 0 /* success */)
    {
        avcodec_free_context(&context);
        throw std::runtime_error("could not send frame to the encoder and receive the result.");
    }

    std::fstream file;
    file.open(filename, std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<char *>(pkt->data), pkt->size);
    file.close();

    avcodec_free_context(&context);
}

// Exports a file into MKV.

void MKVExporter::writeFrames()
{
    CodecPacket pkt;

    while (avcodec_receive_packet(codec_ctx, pkt) == 0)
    {
        pkt->stream_index = stream->index;
        pkt->pts = av_rescale_q(pkt->pts, codec_ctx->time_base, stream->time_base);
        pkt->dts = av_rescale_q(pkt->dts, codec_ctx->time_base, stream->time_base);
        pkt->duration = av_rescale_q(pkt->duration, codec_ctx->time_base, stream->time_base);
        av_interleaved_write_frame(fmt_ctx, pkt);
        av_packet_unref(pkt);
    }
}

MKVExporter::MKVExporter(const ArgumentParser &options) : width(options.getWidth()), height(options.getHeight()), time_base(options.getTimeBase()), crf(options.getCRF()) {};

void MKVExporter::open(std::string filename)
{
    fmt_ctx = nullptr;
    codec_ctx = nullptr;
    auto cleanup_and_throw = [&](std::string msg)
    {
        if (codec_ctx != nullptr)
            avcodec_free_context(&codec_ctx);
        if (fmt_ctx != nullptr)
            avformat_free_context(fmt_ctx);
        throw std::runtime_error(msg);
    };

    const AVCodec *codec = avcodec_find_encoder(codec_id);
    if (!codec)
        cleanup_and_throw("(MKVExporter) could not find codec.");

    codec_ctx = avcodec_alloc_context3(codec);
    codec_ctx->codec_id = codec_id;
    codec_ctx->width = width;
    codec_ctx->height = height;
    codec_ctx->time_base = time_base;
    codec_ctx->framerate = {time_base.num, time_base.num};
    codec_ctx->gop_size = 12;
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    av_opt_set(codec_ctx->priv_data, "crf", std::to_string(crf).c_str(), 0);

    // Allocate format context
    avformat_alloc_output_context2(&fmt_ctx, nullptr, "matroska", filename.c_str());
    if (!fmt_ctx)
        throw std::runtime_error("(MKVExporter) could not create output context");

    // Add stream
    if ((stream = avformat_new_stream(fmt_ctx, nullptr)) == NULL)
        cleanup_and_throw("(MKVExporter) could not allocate stream.");

    if (fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
        codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // Open codec
    if (avcodec_open2(codec_ctx, codec, nullptr) < 0)
        cleanup_and_throw("(MKVExporter) could not open codec.");

    // Copy codec params to stream
    avcodec_parameters_from_context(stream->codecpar, codec_ctx);
    stream->time_base = codec_ctx->time_base;

    // Open output file
    if (!(fmt_ctx->oformat->flags & AVFMT_NOFILE))
    {
        if (avio_open(&fmt_ctx->pb, filename.c_str(), AVIO_FLAG_WRITE) < 0)
            cleanup_and_throw("(MKVExporter) could not open output file via avio_open.");
    }

    // Write header
    if (avformat_write_header(fmt_ctx, nullptr) < 0)
        cleanup_and_throw("(MKVExporter) could not write header.");

    // Get converter context
    sws_ctx = sws_getContext(
        width, height, AV_PIX_FMT_RGBA,
        width, height, AV_PIX_FMT_YUV420P,
        0, nullptr, nullptr, nullptr);

    frame_index = 0;
}

void MKVExporter::addFrame(std::vector<uint8_t> pixels)
{
    RGBAImage frame(width, height, pixels);
    YUVImage yuv_frame(width, height);

    sws_scale(sws_ctx, frame->data, frame->linesize, 0, height,
              yuv_frame->data, yuv_frame->linesize);

    yuv_frame->pts = frame_index++;

    // Encode
    if (avcodec_send_frame(codec_ctx, yuv_frame) >= 0)
        writeFrames();
}

void MKVExporter::close()
{
    // Flush encoder
    avcodec_send_frame(codec_ctx, nullptr);
    writeFrames();

    if (av_write_trailer(fmt_ctx) < 0)
        throw std::runtime_error("(MKVExporter) could not write trailer.");

    // Cleanup
    sws_freeContext(sws_ctx);
    avcodec_free_context(&codec_ctx);
    if (!(fmt_ctx->oformat->flags & AVFMT_NOFILE))
        avio_closep(&fmt_ctx->pb);
    avformat_free_context(fmt_ctx);
}
