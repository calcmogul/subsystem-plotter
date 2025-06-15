// Copyright (c) Tyler Veness

#include "file_utils.hpp"

#include <algorithm>
#include <fstream>
#include <map>
#include <regex>
#include <string>
#include <vector>

int num_lines(std::string_view filename) {
    std::ifstream file{filename.data()};
    std::string line;

    int lines = 0;
    while (std::getline(file, line)) {
        ++lines;
    }

    return lines;
}

std::map<std::string, std::vector<SubsystemData>> categorize_files(
    const std::vector<std::string>& filenames) {
    // First group is subsystem name up to first space, and second group is
    // timestamp
    std::regex fileRgx{
        "^\\./(.*?/?[A-Za-z]+) [A-Za-z ]+-"
        "(\\d{4}-\\d{2}-\\d{2}-\\d{2}_\\d{2}_\\d{2})\\.csv$"};

    std::map<std::string, std::vector<SubsystemData>> timestamps;

    // If the file is a CSV with the correct name pattern, add it to the list
    // for the associated timestamp
    for (const auto& filename : filenames) {
        std::smatch match;
        if (!std::regex_search(filename, match, fileRgx)) {
            continue;
        }

        // If file is empty or only has header (that is, has no data), ignore
        // it. We ignore the case of one line of data because it might be
        // truncated.
        if (num_lines(filename) <= 2) {
            continue;
        }

        std::string timestamp = match[2];
        std::string subsystem = match[1];

        // Shorten CSV paths from simulation or test runs; the standard paths
        // are rather long
        std::string_view subsystem_view = subsystem;
        bool is_test = subsystem_view.starts_with("build/test-results/");
        bool is_sim = subsystem_view.starts_with("build/install/");
        if (is_test || is_sim) {
            std::string_view prefix;
            if (is_test) {
                prefix = "test/";
            } else if (is_sim) {
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
            subsystems.back().filenames.push_back(filename);
        } else {
            it->filenames.push_back(filename);
        }
    }

    return timestamps;
}
