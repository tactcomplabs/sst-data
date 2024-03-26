#!/bin/bash
#
# scripts/uninstall.sh
#
# Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
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

