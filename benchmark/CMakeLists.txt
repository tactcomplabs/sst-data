#
# sst-data/benchmark CMake
#
# Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
#

if(SSTDATA_ENABLE_BENCHMARK)

add_subdirectory(csv)
add_subdirectory(sqlite)

endif()

# -- EOF
