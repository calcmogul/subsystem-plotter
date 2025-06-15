// Copyright (c) Tyler Veness

#pragma once

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
    void plot();

    std::string subsystem;
    std::string widget_name;
    std::string window_title;
    std::vector<std::string> filenames;
    bool is_visible = false;
};
