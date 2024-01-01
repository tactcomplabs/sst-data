#
# ~/sstdata/include CMake
#
# Copyright (C) 2017-2023 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
#

set(SSTDataHdrs
  sstdata.h
)

install(DIRECTORY "."
        DESTINATION "include/sstdata"
        FILES_MATCHING
        PATTERN "sstdata.h")

# EOF
