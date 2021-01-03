#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import os
import re

from dearpygui.core import *
from dearpygui.simple import *

plt.rcParams.update({'figure.max_open_warning': 0})


def num_lines(filename):
    with open(filename) as f:
        i = 0
        for i, l in enumerate(f):
            pass
    return i + 1


def get_file_list():
    """Returns a dictionary with the following structure:

    {
        "timestamp 1": {
            "Subsys 1": [filenames],
            "Subsys 2": [filenames],
        },
        "timestamp 2": {
            "Subsys 1": [filenames],
            "Subsys 2": [filenames],
        },
    }

    'timestamps' maps timestamps to key-value pairs of subsystem names (without
    "states", "inputs", or "outputs") and filename lists.
    """
    # Get list of files in current directory
    files = [
        os.path.join(dp, f)
        for dp, dn, fn in os.walk(".")
        for f in fn
        if f.endswith(".csv")
    ]

    timestamps = {}
    file_rgx = re.compile(
        r"^\..*?/(?P<name>[A-Za-z ]+) (states|inputs|outputs)-"
        r"(?P<date>\d{4}-\d{2}-\d{2}-\d{2}_\d{2}_\d{2})\.csv$")

    # Sorting the file list puts files into the order ["inputs", "outputs",
    # "states"]. This means data series will be loaded in the order of
    # ["inputs", "outputs", "references", "states"] (references are logged
    # before states). This produces the desired dataset layering on plots.
    for f in sorted(files):
        match = file_rgx.search(f)
        if not match:
            continue

        # If file is empty or only has header (that is, has no data), ignore it.
        # We ignore the case of one line of data because it might be truncated.
        if num_lines(f) <= 2:
            continue

        # If the file is a CSV with the correct name pattern, add it to the
        # list for the associated timestamp.
        date = match.group("date")
        name = os.path.basename(match.group("name"))
        if date not in timestamps.keys():
            timestamps[date] = {}
        if name not in timestamps.keys():
            timestamps[date][name] = [f]
        else:
            timestamps[date][name].append(f)

    return timestamps


def open_figs(category, filenames):
    """Creates plots from list of filenames, then returns list of (Figure, Axis)
    object tuples.

    Keyword arguments:
    category -- category to which the filenames belong (subsystem name with
                qualifier)
    filenames -- list of filenames
    """
    figs = []

    # Within each group, make groups of datasets keyed on their unit, then plot
    # each group on its own figure
    unit_rgx = re.compile(r"^(?P<name>[\w\- ]+) \((?P<unit>.*?)\)$")
    unit_groups = {}
    name_groups = {}
    for filename in filenames:
        # Get labels from first row of file
        with open(filename) as f:
            labels = [x.strip('"') for x in f.readline().rstrip().split(",")]

        # Retrieve data from remaining rows of file. "skip_footer=1" skips the
        # last line because it may be incompletely written.
        data = np.genfromtxt(filename,
                             delimiter=",",
                             skip_header=1,
                             skip_footer=1)

        times = data[:, 0:1]

        # Skips label in first column because that's always "Time (s)"
        for i, label in enumerate(labels[1:]):
            match = unit_rgx.search(label)
            name = match.group("name")
            unit = match.group("unit")

            if unit not in unit_groups.keys():
                # Make a new unit group. Tuple entries are as follows:
                # 1. time data column
                # 2. list of data columns
                # 3. list of data labels
                unit_groups[unit] = (times, [], [])
            # "i + 1" skips the time data column
            unit_groups[unit][1].append(data[:, i + 1:i + 2])
            unit_groups[unit][2].append(name)

            # "i + 1" skips the time data column
            name_groups[name] = data[:, i + 1:i + 2]

    # Plot time domain datasets
    print(f'  [vs time] {category} ({", ".join(unit_groups.keys())})')
    for unit, data_tups in unit_groups.items():
        fig, ax = plt.subplots(1, 1)
        ax.set_title(f"{category} ({unit})")

        for i in range(len(data_tups[1])):
            ax.plot(data_tups[0], data_tups[1][i])

        # First label is x axis label (time). The remainder are dataset
        # names.
        ax.set_xlabel("Time (s)")
        ax.set_ylabel(f"Data ({unit})")
        ax.legend(data_tups[2])

        figs.append((fig, ax))

    # Plot X-Y datasets. If the file doesn't have all the required keys, skip
    # it.
    if not (set(["X reference", "Y reference", "X estimate", "Y estimate"]) -
            set(name_groups.keys())):
        print(f'  [y vs x] {category}')
        fig, ax = plt.subplots(1, 1)
        ax.set_title(f"{category} trajectory")

        ax.plot(name_groups["X reference"], name_groups["Y reference"])
        ax.plot(name_groups["X estimate"], name_groups["Y estimate"])

        ax.set_xlabel("X (m)")
        ax.set_ylabel("Y (m)")
        ax.legend(["Reference", "Estimate"])

        # This equalizes the X and Y axes so the trajectories aren't warped
        ax.axis("equal")

        figs.append((fig, ax))
    return figs


def plot_callback(sender, data):
    timestamp = sender[sender.find("##") + 2:]
    subsystem = sender[:sender.find("##")]
    plot_data = get_data(sender)
    if get_value(sender):
        plot_data.figs = open_figs(subsystem, plot_data.filenames)
    else:
        for fig, ax in plot_data.figs:
            plt.close(fig)
        plot_data.figs = []


class PlotData:

    def __init__(self, filenames):
        self.filenames = filenames
        self.figs = []


def show_gui():

    with window(
            "Log Viewer",
            x_pos=0,
            y_pos=0,
            width=800,
            height=600,
    ):

        # {
        #     "timestamp 1": {
        #         "Subsys 1": [filenames],
        #         "Subsys 2": [filenames],
        #     },
        #     "timestamp 2": {
        #         "Subsys 1": [filenames],
        #         "Subsys 2": [filenames],
        #     },
        # }
        timestamps = get_file_list()

        # Sort by latest timestamp first
        for timestamp in sorted(timestamps.keys(), reverse=True):
            with collapsing_header(timestamp, default_open=True):
                for subsystem in timestamps[timestamp].keys():
                    widget_name = f"{subsystem}##{timestamp}"
                    add_checkbox(widget_name, callback=plot_callback)

                    # Add pair of filename list and plot object list as data
                    add_data(widget_name,
                             PlotData(timestamps[timestamp][subsystem]))


show_gui()
start_dearpygui()
