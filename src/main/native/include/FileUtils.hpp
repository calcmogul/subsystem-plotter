// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

class SubsystemData {
public:
    /**
     * Constructs a subsystem data container.
     *
     * @param subsystem Subsystem name.
     * @param timestamp Timestamp of subsystem's associated data.
     */
    SubsystemData(std::string_view subsystem, std::string_view timestamp);

    /**
     * Draw plots.
     */
    void Plot();

    std::string subsystem;
    std::string widgetName;
    std::string windowTitle;
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
 * Returns a map of timestamps to lists of subsystem data. The subsystem for
 * each timestamp has a list of filenames with the same subsystem name (usually
 * a list of files ending in "states", "inputs", and "outputs".
 *
 * @param files List of filenames to categorize.
 */
std::map<std::string, std::vector<SubsystemData>> CategorizeFiles(
    const std::vector<std::string>& files);
