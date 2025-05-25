#ifndef EXPORTER_H_
#define EXPORTER_H_

#include <string>
#include <vector>
#include <cstdint>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

void saveToPNG(std::string filename, int width, int height, std::vector<uint8_t> pixels);
std::vector<uint8_t> dumpPixelFromGL(int width, int height);

struct MKVExporter
{

private:
    int width;
    int height;
    AVRational time_base;
    int frame_index;
    int bitrate;

    AVFormatContext *fmt_ctx;
    AVCodecContext *codec_ctx;
    AVStream *stream;
    struct SwsContext *sws_ctx;

    void writeFrames();

public:
    const AVCodecID codec_id = AV_CODEC_ID_H264;

    MKVExporter(int width, int height, AVRational time_base, int bitrate = 10'000'000);

    // open an MKV file for export.
    void open(std::string filename);
    // add an frame to the video.
    void addFrame(std::vector<uint8_t> pixels);
    // close the file and finish exporting.
    void close();
};

#endif
