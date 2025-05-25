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

#include "arguments.h"


void saveToPNG(std::string filename, int width, int height, std::vector<uint8_t> pixels);

struct MKVExporter
{

private:
    int width;
    int height;
    AVRational time_base;
    int frame_index;
    int crf;

    AVFormatContext *fmt_ctx;
    AVCodecContext *codec_ctx;
    AVStream *stream;
    struct SwsContext *sws_ctx;

    void writeFrames();

public:
    const AVCodecID codec_id = AV_CODEC_ID_H264;

    MKVExporter(const ArgumentParser &options);

    // open an MKV file for export.
    void open(std::string filename);
    // add an frame to the video.
    void addFrame(std::vector<uint8_t> pixels);
    // close the file and finish exporting.
    void close();
};

#endif
