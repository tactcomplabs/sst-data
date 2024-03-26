#
# ~/sstdata/include CMake
#
# Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
#

set(SSTDataHdrs
  sstdata.h
)

#-- SST ARROW LINK OPTIONS
if(SSTDATA_ENABLE_ARROW)
  set(SSTDataHdrs ${SSTDataHdrs} statArrow.h)
endif()

#-- SST SQLLITE3 LINK OPTIONS
if(SSTDATA_ENABLE_SQLLITE)
  set(SSTDataHdrs ${SSTDataHdrs} statSQLLite.h)
endif()


# EOF
