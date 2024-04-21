#
# sst-data/benchmark CMake
#
# Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
#

#------- TESTS  ---------------
message(STATUS "SST-DATA Benchmark Setup")

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.py)

if(SSTDATA_ENABLE_BENCHMARK)

find_program(SST_EXECUTABLE sst)
  if (SST_EXECUTABLE)
    message(STATUS "FOUND SST AT ${SST_EXECUTABLE}")
  else ()
    message(FATAL_ERROR "COULD NOT FIND SST")
endif ()

foreach(testSrc ${TEST_SRCS})
  # Extract the file name
  get_filename_component(testName ${testSrc} NAME_WE)

  # Add the test for execution
  add_test(NAME ${testName}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${SST_EXECUTABLE} --add-lib-path=../build/sstdata/src/ ${CMAKE_CURRENT_SOURCE_DIR}/${testName}.py)
  set_tests_properties(${testName}
    PROPERTIES FAIL_REGULAR_EXPRESSION "[^a-z]Error;ERROR;Failed")
endforeach(testSrc)

endif()

# -- EOF
