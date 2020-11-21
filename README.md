# subsystem-plotter

Finds latest versions of the CSVs for each subsystem, then plots the time domain and X-Y data.

## Building

To build the project, first run `cmake dir` within a terminal from the desired build directory, where "dir" is the relative location of the CMakeLists.txt file. If a release build is desired, append `-DCMAKE_BUILD_TYPE=Release` to the cmake call. Then run `make`.
