#
# ~/sstdata/src CMake
#
# Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
#

#-- SST ARROW BUILD OPTIONS
if(SSTDATA_ENABLE_ARROW)
  set(statArrowSrcs statArrow.cc)
else()
  set(statArrowSrcs "")
endif()

#-- SST SQLLITE3 BUILD OPTIONS
if(SSTDATA_ENABLE_SQLLITE)
  set(statSQLLiteSrcs statSQLLite.cc)
else()
  set(statSQLLiteSrcs "")
endif()

#-- SST DATA TARGET
add_library(sstdata SHARED ${statArrowSrcs} ${statSQLLiteSrcs})

target_include_directories(sstdata PRIVATE ${SSTDATA_INCLUDE_PATH})
target_include_directories(sstdata PRIVATE ${SST_INCLUDE_DIR})

#-- SST ARROW LINK OPTIONS
if(SSTDATA_ENABLE_ARROW)
  target_include_directories(sstdata PRIVATE Arrow::arrow_shared
    ${SSTDATA_INCLUDE_PATH} ${SST_INSTALL_DIR}/include)
  target_link_directories(sstdata PRIVATE ${SST_INSTALL_DIR}/lib
    ${SST_INSTALL_DIR}/lib/sst-elements-library)
  target_link_libraries(sstdata PRIVATE Arrow::arrow_shared
    ${SST_INSTALL_DIR}/lib/sstcore ${SST_LDFLAGS})
endif()

#-- SST SQLLITE3 LINK OPTIONS
if(SSTDATA_ENABLE_SQLLITE)
  target_include_directories(sstdata PRIVATE ${SQLITE3_INCLUDE_DIRS})
  target_link_libraries(sstdata PRIVATE ${SQLITE3_LIBRARIES})
endif()

# EOF
