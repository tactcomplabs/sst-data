#
# sst-data CMake
#
# Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
#

#-- Prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "DO NOT BUILD in-tree.")
endif()

# Minimum required version of CMake and project information
cmake_minimum_required(VERSION 3.19)
project(sstdata CXX)

#-- SST Setup
execute_process(COMMAND sst-config --prefix
                OUTPUT_VARIABLE SST_INSTALL_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(SST_INCLUDE_DIR "${SST_INSTALL_DIR}/include")
if(NOT (EXISTS "${SST_INSTALL_DIR}"))
  message(FATAL_ERROR " SST_INSTALL_DIR (${SST_INSTALL_DIR}) is invalid.")
endif()
include_directories(SST_SRC ${SST_INSTALL_DIR})

# SST Configuration Sanity Check
find_program(SST sst)

find_program(SST_CONFIG sst-config)
if(NOT SST OR NOT SST_CONFIG)
  message(FATAL_ERROR "No SST binary or sst-config binary found in path")
endif()

# SST Environment Derivation
execute_process(COMMAND sst-config --CXX
                OUTPUT_VARIABLE CXX
                OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(COMMAND sst-config --ELEMENT_CXXFLAGS
                OUTPUT_VARIABLE SST_CXXFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(COMMAND sst-config --ELEMENT_LDFLAGS
                OUTPUT_VARIABLE SST_LDFLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(COMMAND sst-config SST_ELEMENT_LIBRARY SST_ELEMENT_LIBRARY_LIBDIR
                OUTPUT_VARIABLE SST_ELEMENT_LIBRARY_LIBDIR
                OUTPUT_STRIP_TRAILING_WHITESPACE
)

#-- Compiler Options
# Compiler Options
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(WERROR_FLAG "")
  set(FP_MODE_FLAG "-ffp-model=strict")
else()
  set(WERROR_FLAG "-Werror")
  set(FP_MODE_FLAG "-frounding-math")
endif()

set(LDFLAGS "${SST_LDFLAGS}")
set(CMAKE_CXX_FLAGS "-std=c++17 ${FP_MODE_FLAG} -O2 -Wall -Wextra ${WERROR_FLAG} -Wvla -Wuninitialized -Wfloat-conversion -Wdouble-promotion -Wno-unused-parameter -Wno-deprecated-declarations ${CMAKE_CXX_FLAGS} -I./ ${LDFLAGS} ${SSTDATA_COMPILER_MACROS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall ${SSTDATA_COMPILER_MACROS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall ${SSTDATA_COMPILER_MACROS}")

#-- SST-Data Paths
set(SSTDATA_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/sstdata/include" CACHE PATH "SST-Data include path")

#---------------------------------------------
# TARGET OPTIONS
#---------------------------------------------
option(SSTDATA_ENABLE_ARROW "Enable SSTData Arrow Backend" OFF)
option(SSTDATA_ENABLE_SQLLITE "Enable SSTData SQLLite Backend" OFF)
option(SSTDATA_ENABLE_MPI "Enable MPI Support" OFF)

if(SSTDATA_ENABLE_MPI)
  find_package(MPI)
  if(MPI_FOUND)
    add_definitions(-DSSTDATA_MPI=1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${MPI_CXX_INCLUDE_DIRS}")
    message(STATUS "[MPI] Found MPI CXX at ${MPI_CXX_COMPILER}")
    message(STATUS "[MPI] Setting CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
  endif()
endif()

#-- Arrow Dependency
if(SSTDATA_ENABLE_ARROW)
  find_package(Arrow REQUIRED)
  if(NOT Arrow_FOUND)
    message(FATAL_ERROR "Arrow not found!")
  endif()
endif()

#-- SQLLite3 Dependency
if(SSTDATA_ENABLE_SQLLITE)
  find_package(SQLite3)
  message(STATUS "[SQLITE3] Found header path at ${SQLite3_INCLUDE_DIRS}")
  message(STATUS "[SQLITE3] Found libraries at ${SQLite3_LIBRARIES}")
  if(NOT SQLITE3_FOUND)
    message(FATAL_ERROR "SQLLite3 not found!")
  endif()
endif()

#-- Documentation Options
option(BUILD_DOCUMENTATION "Create and install the doxygen-ized API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is required to build the documentation.")
  endif()

  set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/sstdata.conf)

  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    COMMAND ${BASH_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cleanmanpages.sh ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/man/man3
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/html DESTINATION share/doc)
endif()

#-- Testing Options
option(SSTDATA_ENABLE_TESTING "Enable Testing" ON)
if(SSTDATA_ENABLE_TESTING)
  enable_testing()
endif()

#-- Custom Commands
add_custom_target(uninstall COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/uninstall.sh")

#-- Subdirectories
add_subdirectory(sstdata)
add_subdirectory(test)

# EOF
