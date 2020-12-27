// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#include "FileUtils.hpp"

#include <filesystem>
#include <fstream>
#include <regex>

namespace fs = std::filesystem;

int NumLines(std::string_view filename) {
    std::ifstream file{filename.data()};
    std::string line;

    int lines = 0;
    while (std::getline(file, line)) {
        ++lines;
    }

    return lines;
}

std::map<std::string, std::vector<SubsystemData>> CategorizeFiles(
    const std::vector<std::string>& files) {
    // First group is name and second is date
    std::regex fileRgx{
        "^\\./(.*?/?[A-Za-z ]+)-"
        "(\\d{4}-\\d{2}-\\d{2}-\\d{2}_\\d{2}_\\d{2})\\.csv$"};

    std::map<std::string, std::vector<SubsystemData>> timestamps;

    // If the file is a CSV with the correct name pattern, add it to the list
    // for the associated timestamp
    for (const auto& file : files) {
        std::smatch match;
        if (!std::regex_search(file, match, fileRgx)) {
            continue;
        }

        // If file is empty or only has header (that is, has no data), ignore
        // it. We ignore the case of one line of data because it might be
        // truncated.
        if (NumLines(file) <= 2) {
            continue;
        }

        std::string timestamp = match[2];
        std::string subsystem = match[1];

        // Strip everything from the filename after the first space
        fs::path path{subsystem};
        auto filename = path.filename().string();
        subsystem =
            path.replace_filename(filename.substr(0, filename.find(" ")))
                .string();

        // Shorten CSV paths from simulation or test runs; the standard paths
        // are rather long
        std::string_view subsystemView = subsystem;
        bool isTest = subsystemView.starts_with("build/test-results/");
        bool isSim = subsystemView.starts_with("build/install/");
        if (isTest || isSim) {
            std::string_view prefix;
            if (isTest) {
                prefix = "test/";
            } else if (isSim) {
                prefix = "sim/";
            }

            subsystem = std::string{prefix} +
                        subsystem.substr(std::min(subsystem.find("debug"),
                                                  subsystem.find("release")));
        }

        // Get subsystem list for the file's timestamp
        auto& subsystems = timestamps[timestamp];

        // Add file to the associated subsystem's file list. A new subsystem is
        // created if none of the existing ones match.
        auto it = std::find_if(
            subsystems.begin(), subsystems.end(),
            [&](const auto& v) { return v.subsystem == subsystem; });
        if (it == subsystems.end()) {
            subsystems.emplace_back(subsystem, timestamp);
            subsystems.back().filenames.push_back(file);
        } else {
            it->filenames.push_back(file);
        }
    }

    return timestamps;
}
