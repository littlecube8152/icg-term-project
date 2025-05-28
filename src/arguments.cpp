#include "arguments.h"

#include <algorithm>
#include <functional>
#include <format>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "constants.h"

void ArgumentParser::parse(int argc, char *argv[])
{
    bool valid = true;

    const std::vector<std::tuple<std::string, std::string, std::string, std::function<bool(int)>>> integer_arguments = {
        {"-f", "--fps", "Set FPS of the video output. Default: 30.",
         [this](int arg)
         { option_fps = arg; return true; }},
        {"-d", "--depth", "Set ray-tracing recursion depth. Default: 32.",
         [this](int arg)
         { option_depth = arg; return true; }},
        {"-s", "--samples", "Set ray-tracing samples per unit length. Actual samples per pixel will be the square of this argument. Default: 4.",
         [this](int arg)
         { option_samples = arg; return true; }},
        {"-l", "--length", "Set simulation time length in second. Default: 2 (second).",
         [this](int arg)
         { option_length = arg; return true; }},
        {"-r", "--resolution", "Set the height of the image output. Must be one of 144, 240, 360, 540 (default), 720, 1080, 2160, or 4320.",
         [this](int arg)
         {
             static const std::vector<int> support = {144, 240, 360, 540, 720, 1080, 2160, 4320};
             if (std::find(support.begin(), support.end(), arg) == support.end())
                 return false;
             option_resolution = arg;
             return true;
         }},
        {"-q", "--quality", "Set CRF (constant rate factor) of the encoding. Default to 18.",
         [this](int arg)
         {
             option_crf = arg;
             return true;
         }},
        {"-t", "--thread", "Set GPU rendering worker thread numbers. Default: 1.",
         [this](int arg)
         { option_worker_count = arg; return true; }},
    };
    const std::vector<std::tuple<std::string, std::string, std::string, std::function<bool(void)>>> flag_arguments = {
        {"-h", "--help", "Display this help message.",
         []()
         { return false; }},
        {"-n", "--no-window", "Do not open visible window and close immediate after every frames are computed and saved.",
         [this]()
         { option_no_window = true; return true; }}};

    auto check_integer = [](std::string s) -> int
    {
        return (s.find_first_not_of("0123456789") == std::string::npos);
    };
    for (int i = 1; i < argc; i++)
    {
        bool match = false;

        for (auto [short_name, full_name, _, routine] : integer_arguments)
            if (!match && (argv[i] == short_name || argv[i] == full_name))
            {
                match = true;
                if (i + 1 >= argc || !check_integer(argv[i + 1]))
                {
                    valid = false;
                }
                else
                {
                    valid &= routine(std::stoi(argv[i + 1]));
                    i++;
                }
            }
        for (auto [short_name, full_name, _, routine] : flag_arguments)
            if (!match && (argv[i] == short_name || argv[i] == full_name))
            {
                valid &= routine();
                match = true;
            }
        if (!match)
            valid = false;
    }
    if (!valid)
    {
        std::cerr << "Usage: main [options]\n";
        std::cerr << "Available options:\n";
        for (auto [short_name, full_name, description, _] : integer_arguments)
            std::cerr << std::format("\t{}, {}\n\t\t{}\n", short_name, full_name, description);
        for (auto [short_name, full_name, description, _] : flag_arguments)
            std::cerr << std::format("\t{}, {}\n\t\t{}\n", short_name, full_name, description);
        exit(-1);
    }
}
