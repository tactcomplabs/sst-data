#
# ~/sstdata/src CMake
#
# Copyright (C) 2017-2023 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
#

add_library(sstArrow SHARED
  statArrow.cc
)

target_include_directories(sstArrow PRIVATE Arrow::arrow_shared ${SSTDATA_INCLUDE_PATH} ${SST_INSTALL_DIR}/include)
target_link_directories(sstArrow PRIVATE ${SST_INSTALL_DIR}/lib ${SST_INSTALL_DIR}/lib/sst-elements-library)
target_link_libraries(sstArrow PRIVATE Arrow::arrow_shared ${SST_INSTALL_DIR}/lib/sstcore ${SST_LDFLAGS})

# EOF
