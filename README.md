# sst-data: An advanced SST Statistics I/O library

![sst-data](documentation/imgs/sst-data.png)

## Overview
The `sst-data` library provides alternative `SST::Statistics` output and storage mechanisms beyond what is provided by SST-Core.
The goal of which is to provide SST simulation users statistics storage that is more appropriately organized for large-scale 
secondary and tertiary data analysis.  The backing storage for `sst-data` is designed to support additional visualization 
capabilities through open source tools such as Grafana.

Currently, the `sst-data` library supports the following output mechanisms:
* SQLite3
* Apache Arrow

## Installation

### Prerequisites
The following prerequisites are required for building `sst-data` from source:
* CMake 3.19+
* SST-Core 13.1.0+

At least one of the following I/O packages must be installed:
* SQLite3 (including development libraries)
* Apache Arrow

The following prerequisites are optional, but can be useful for developers and/or users:
* MPI (for supporting MPI-enabled statistics)
* Doxygen (for building documentation)
* CTest (for the internal test harness)

### Building
Building `sst-data` requires the use of CMake with one or more build options.  This is analogous to the following (assuming the `sst-config` executable is in your current `PATH`):

```
mkdir build
cd build
cmake -DSSTDATA_ENABLE_xxx=ON ../
make -j && make uninstall && make install
```

Additional cmake options are as follows:
* `SSTDATA_ENABLE_ARROW` : Enables Apache Arrow support
* `SSTDATA_ENABLE_SQLLITE` : Enables SQLite3 support
* `SSTDATA_ENABLE_MPI` : Enables MPI support
* `SSTDATA_ENABLE_TESTING` : Enables the test harness (`make test` or `ctest` triggers the tests)
* `BUILD_DOCUMENTATION` : Enables documentation

## Using sst-data

Using the `sst-data` infrastructure is a simple process of integrating the library into a standard SST simulation script.  
This can be done by replacing the `sst.setStatisticOutput(..)` directive contents with the `sstdata` library contents.  For example, 
you can accomplish this via the following to enable SQLite3 data storage:

```
sst.setStatisticOutput("sstdata.statsqllite")
```

## License

See the [LICENSE](./LICENSE) file

## Authors
* *John Leidel* - *Chief Scientist* - [Tactical Computing Labs](http://www.tactcomplabs.com)
* *David Donofrio* - *Chief Hardware Architect* - [Tactical Computing Labs](http://www.tactcomplabs.com)
* *Chris Taylor* - *Sr. Principal Research Engineer* - [Tactical Computing Labs](http://www.tactcomplabs.com)
* *Ryan Kabrick* - *Sr. Research Engineer* - [Tactical Computing Labs](http://www.tactcomplabs.com)
