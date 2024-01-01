#
# ~/sstdata/src CMake
#
# Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
#

if(SSTDATA_ENABLE_ARROW)
  add_library(sstarrow SHARED
    statArrow.cc
  )

  target_include_directories(sstarrow PRIVATE Arrow::arrow_shared ${SSTDATA_INCLUDE_PATH} ${SST_INSTALL_DIR}/include)
  target_link_directories(sstarrow PRIVATE ${SST_INSTALL_DIR}/lib ${SST_INSTALL_DIR}/lib/sst-elements-library)
  target_link_libraries(sstarrow PRIVATE Arrow::arrow_shared ${SST_INSTALL_DIR}/lib/sstcore ${SST_LDFLAGS})
endif()

# EOF
