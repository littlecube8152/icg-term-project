#include "arguments.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <ranges>

#include "constants.h"
#include "scenes.h"

void ArgumentParser::registerScene(std::function<Scene(void)> constructor, std::string name, bool set_default)
{
    scene_constructors[name] = constructor;
    if (set_default)
        option_scene = name;
}

void ArgumentParser::initSceneSelection()
{
    registerScene([this]() -> Scene
                  { return SceneMaterialDemo(*this, option_time_ratio); }, "MaterialDemo");
    registerScene([this]() -> Scene
                  { return SceneRandomBalls(*this, option_time_ratio); }, "RandomBalls");
    registerScene([this]() -> Scene
                  { return SceneRelativityTest(*this, option_time_ratio); }, "RelativityTest", true);
    registerScene([this]() -> Scene
                  { return SceneRelativisticMovementTest(*this, option_time_ratio); }, "RelativisticMovementTest");
    registerScene([this]() -> Scene
                  { return SceneRelativisticDopplerTest(*this, option_time_ratio); }, "RelativisticDopplerTest");
    registerScene([this]() -> Scene
                  { return SceneTeapotDemo(*this, option_time_ratio); }, "TeapotDemo");
    registerScene([this]() -> Scene
                  { return SceneRelativisticMovementDemo(*this, option_time_ratio); }, "RelativisticMovementDemo");
    registerScene([this]() -> Scene
                  { return SceneTerellRotationTest(*this, option_time_ratio); }, "TerellRotationTest");
}

void ArgumentParser::parse(int argc, char *argv[])
{
    bool need_help = false;

    auto to_integer = [](std::string s) -> int
    {
        std::size_t pos;
        int result = std::stoi(s, &pos);
        if (pos != s.size())
            throw std::invalid_argument("to_integer");
        return result;
    };

    auto to_float = [](std::string s) -> float
    {
        std::size_t pos;
        float result = std::stof(s, &pos);
        if (pos != s.size() || std::isinf(result) || std::isnan(result))
            throw std::invalid_argument("to_float");
        return result;
    };

    std::string scene_list;
    auto scenes = std::views::keys(scene_constructors);
    for (bool first = true; const auto &scene_name : scenes)
    {
        scene_list += std::format("{} {}", (first ? "" : ","), scene_name);
        first = false;
    }

    std::vector<std::tuple<std::string, std::string, std::string, std::function<void(std::string)>>> arguments = {
        {"-f", "--fps", std::format("Set FPS of the video output. Default: {}.", option_fps),
         [this, &to_integer](std::string arg)
         { option_fps = to_integer(arg); 
            if (option_fps <= 0) throw std::invalid_argument("fps"); }},
        {"-d", "--depth", std::format("Set ray-tracing recursion depth. Default: {}.", option_depth),
         [this, &to_integer](std::string arg)
         { option_depth = to_integer(arg); }},
        {"-s", "--samples", std::format("Set ray-tracing samples per unit length. Actual samples per pixel will be the square of this argument. Default: {}.", option_samples),
         [this, &to_integer](std::string arg)
         { option_samples = to_integer(arg); }},
        {"-l", "--length", std::format("Set simulation time length in second. Default: {} (second).", option_length),
         [this, &to_integer](std::string arg)
         { option_length = to_integer(arg); }},
        {"-r", "--resolution", "Set the height of the image output. Must be one of 144, 240, 360, 540 (default), 720, 1080, 2160, or 4320.",
         [this, &to_integer](std::string arg)
         {
             option_resolution = to_integer(arg);
             static const std::vector<int> support = {144, 240, 360, 540, 720, 1080, 2160, 4320};
             if (std::find(support.begin(), support.end(), option_resolution) == support.end())
                 throw std::invalid_argument("resolution");
         }},
        {"-q", "--quality", std::format("Set CRF (constant rate factor) of the encoding. Default: {}.", option_crf),
         [this, &to_integer](std::string arg)
         { option_crf = to_integer(arg); }},
        {"-t", "--thread", std::format("Set GPU rendering worker thread numbers. Default: {}.", option_worker_count),
         [this, &to_integer](std::string arg)
         { option_worker_count = to_integer(arg); }},
        {"-T", "--time-ratio", std::format("Set the time ratio. It is highly recommended to set to a value near the inverse of the light speed. Default: {}.", option_time_ratio),
         [this, &to_float](std::string arg)
         { option_time_ratio = to_float(arg); }},
        {"-S", "--scene", std::format("Choose the scene from the presets. Must be one of{}. Default: {}.", scene_list, option_scene),
         [this](std::string scene)
         { option_scene = scene; return scene_constructors.find(scene) != scene_constructors.end(); }}};

    const std::vector<std::tuple<std::string, std::string, std::string, std::function<void(void)>>> flag_arguments = {
        {"-h", "--help", "Display this help message.",
         [&need_help]()
         { need_help = true; }},
        {"-n", "--no-window", "Do not open visible window and close immediate after all frames are computed and saved.",
         [this]()
         { option_no_window = true; }}};

    auto print_usage = [&arguments, &flag_arguments]()
    {
        std::cerr << "Usage: main [options]\n";
        std::cerr << "Available options:\n";
        for (auto [short_name, full_name, description, _] : arguments)
            std::cerr << std::format("\t{}, {}\n\t\t{}\n", short_name, full_name, description);
        for (auto [short_name, full_name, description, _] : flag_arguments)
            std::cerr << std::format("\t{}, {}\n\t\t{}\n", short_name, full_name, description);
        exit(-1);
    };

    for (int i = 1; i < argc; i++)
    {
        std::string current = argv[i];
        bool match = false;

        try
        {
            for (auto [short_name, full_name, _, routine] : arguments)
                if (!match && (current == short_name || current == full_name))
                {
                    match = true;
                    if (i + 1 >= argc)
                        throw std::invalid_argument("not enough arguments");
                    routine(argv[i + 1]);
                    i++;
                }
            for (auto [short_name, full_name, _, routine] : flag_arguments)
                if (!match && (current == short_name || current == full_name))
                {
                    match = true;
                    routine();
                }

            if (!match)
            {
                std::cerr << std::format("ERROR: Unrecognized argument {}.\n", current);
                print_usage();
            }
            if (need_help)
                print_usage();
        }
        catch (...)
        {
            std::cerr << std::format("ERROR: Invalid argument for {}.\n", current);
            print_usage();
        }
    }
}
