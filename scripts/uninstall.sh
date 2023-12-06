#!/bin/bash
#
# Unregisters SST-Data from the SST infrastructure
#

#-- unregister it
sst-register -u sstdata

#-- forcible remove it from the local script
CONFIG=~/.sst/sstsimulator.conf
if test -f "$CONFIG"; then
  echo "Removing configuration from local config=$CONFIG"
  sed -i.bak '/sstdata/d' $CONFIG
fi

