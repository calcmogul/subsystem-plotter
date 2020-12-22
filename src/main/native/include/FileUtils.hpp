// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

/**
 * Returns number of lines in a file.
 *
 * @param filename Filename.
 */
int NumLines(std::string_view filename);

class PlotData {
public:
    PlotData() = default;
    explicit PlotData(const std::vector<std::string>& filenames)
        : filenames(filenames) {}

    std::string widgetName;
    std::vector<std::string> filenames;
    bool isVisible = false;
};

/**
 * Returns a map with the following structure:
 *
 * {
 *     "timestamp 1": {
 *         "Subsys 1": [filenames],
 *         "Subsys 2": [filenames],
 *     },
 *     "timestamp 2": {
 *         "Subsys 1": [filenames],
 *         "Subsys 2": [filenames],
 *     },
 * }
 *
 * It maps timestamps to key-value pairs of subsystem names (without "states",
 * "inputs", or "outputs") and filename lists.
 *
 * @param files List of filenames to categorize.
 */
std::map<std::string, std::map<std::string, PlotData>> CategorizeFiles(
    const std::vector<std::string>& files);
