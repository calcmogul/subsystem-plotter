// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

class SubsystemData {
public:
    SubsystemData() = default;
    explicit SubsystemData(const std::vector<std::string>& filenames)
        : filenames(filenames) {}

    /**
     * Draw plots.
     */
    void Plot();

    std::string_view timestamp;
    std::string_view subsystem;
    std::string widgetName;
    std::vector<std::string> filenames;
    bool isVisible = false;
};

/**
 * Returns number of lines in a file.
 *
 * @param filename Filename.
 */
int NumLines(std::string_view filename);

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
std::map<std::string, std::map<std::string, SubsystemData>> CategorizeFiles(
    const std::vector<std::string>& files);
