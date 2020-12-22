// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

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
    void Plot();

    std::string subsystem;
    std::string widgetName;
    std::string windowTitle;
    std::vector<std::string> filenames;
    bool isVisible = false;
};
