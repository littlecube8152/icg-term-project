#include "writer.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
}

#include <GL/gl.h>

#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

void saveToPNG(std::string filename, int width, int height)
{
    std::vector<uint8_t> pixels(width * height * 4);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    // Flip vertically (OpenGL is bottom-left origin; PNG is top-left)
    for (int y = 0; y < height / 2; y++)
    {
        std::swap_ranges(pixels.begin() + y * width * 4, pixels.begin() + (y + 1) * width * 4,
                         pixels.begin() + (height - y - 1) * width * 4);
    }

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

    AVFrame *frame = av_frame_alloc();
    if (frame == NULL)
        throw std::runtime_error("could not allocate frame.");
    frame->format = context->pix_fmt;
    frame->width = context->width;
    frame->height = context->height;

    if (avcodec_open2(context, codec, nullptr) < 0)
    {
        avcodec_free_context(&context);
        throw std::runtime_error("could not open codec.");
    }

    av_image_alloc(frame->data, frame->linesize, width, height, context->pix_fmt, 32);

    std::memcpy(frame->data[0], pixels.data(), width * height * 4);

    AVPacket *pkt = av_packet_alloc();
    pkt->data = nullptr;
    pkt->size = 0;

    if (avcodec_send_frame(context, frame) != 0 /* success */ ||
        avcodec_receive_packet(context, pkt) != 0 /* success */)
    {
        av_packet_unref(pkt);
        av_packet_free(&pkt);
        av_freep(&frame->data[0]);
        av_frame_free(&frame);
        avcodec_free_context(&context);
        throw std::runtime_error("could not send frame to the encoder and receive the result.");
    }

    std::fstream file;
    file.open(filename, std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<char *>(pkt->data), pkt->size);
    file.close();

    av_packet_unref(pkt);
    av_packet_free(&pkt);
    av_freep(&frame->data[0]);
    av_frame_free(&frame);
    avcodec_free_context(&context);
}