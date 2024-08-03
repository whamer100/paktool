#include <iostream>
#include <filesystem>
#include "vendor/libpeggle/libpeggle.h"
#include "vendor/CLI11.hpp"
#include "logma.h"

int main(const int argc, char** argv)
{
    CLI::App app("PopCap Pak Tool");

    std::string input_path;
    std::string output_path;
    bool verbosity{false};

    app.add_flag("-v,--verbose", verbosity, "Increase verbosity");

    CLI::App* sub_unpack = app.add_subcommand("unpack", "Unpack mode");
    CLI::App* sub_pack = app.add_subcommand("pack", "Pack mode");

    sub_unpack->add_option("input", input_path, "Input file")
        ->required();
    sub_unpack->add_option("output", output_path, "Output directory")
        ->required();

    sub_pack->add_option("input", input_path, "Input directory")
        ->required();
    sub_pack->add_option("output", output_path, "Output file")
        ->required();

    CLI11_PARSE(app, argc, argv);

    if (verbosity)
        Peggle::change_logging(Peggle::LogVerbose);
    else
        Peggle::change_logging(Peggle::LogDefault);

    if (sub_pack->parsed()) {
        if (!std::filesystem::is_directory(input_path)) {
            log_fatal("Input path is not a directory!");
            exit(1);
        }
        auto pak = Peggle::Pak(input_path);
        pak.SetXor(0xF7);
        if (std::filesystem::is_directory(output_path)) {
            log_fatal("Output path is a directory!");
            exit(1);
        }
        pak.Save(output_path);
    }
    else if (sub_unpack->parsed()) {
        if (std::filesystem::is_directory(input_path)) {
            log_fatal("Input path is a directory!");
            exit(1);
        }
        const auto pak = Peggle::Pak(input_path);
        if (!pak.IsPak()) {
            log_fatal("Input file is not a valid Pak file!");
            exit(1);
        }
        if (std::filesystem::exists(output_path) && !std::filesystem::is_directory(output_path)) {
            log_fatal("Output path is not a valid directory!");
            exit(1);
        }
        pak.Export(output_path);
    }
    else {
        std::printf("%s", app.help().c_str());
        exit(1);
    }

    return 0;
}
