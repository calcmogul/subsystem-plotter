// Copyright (c) Tyler Veness

#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "subsystem_data.hpp"

/**
 * Returns number of lines in a file.
 *
 * @param filename Filename.
 */
int num_lines(std::string_view filename);

/**
 * Returns a map of timestamps to lists of subsystem data. The subsystem for
 * each timestamp has a list of filenames with the same subsystem name (usually
 * a list of files ending in "states", "inputs", and "outputs".
 *
 * @param filenames List of filenames to categorize.
 */
std::map<std::string, std::vector<SubsystemData>> categorize_files(
    const std::vector<std::string>& filenames);
