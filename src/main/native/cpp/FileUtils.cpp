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
    // First group is name, second group is data type, and third group is date
    std::regex fileRgx{
        "^\\..*?/([A-Za-z ]+) (states|inputs|outputs)-"
        "(\\d{4}-\\d{2}-\\d{2}-\\d{2}_\\d{2}_\\d{2})\\.csv$"};

    std::map<std::string, std::vector<SubsystemData>> timestamps;

    // Sorting the file list puts files into the order["inputs", "outputs",
    // "states"]. This means data series will be loaded in the order of
    // ["inputs", "outputs", "references", "states"] (references are logged
    // before states). This produces the desired dataset layering on plots.
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

        // If the file is a CSV with the correct name pattern, add it to the
        // list for the associated timestamp.
        std::string timestamp = match[3];
        std::string subsystem = fs::path{match[1]}.filename();

        auto& subsystems = timestamps[timestamp];
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
