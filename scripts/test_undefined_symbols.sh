#!/bin/sh
#
# scrripts/test_undefined_symbols.sh
#
# Copyright (C) 2017-2024 Tactical Computing Laboratories, LLC
# All Rights Reserved
# contact@tactcomplabs.com
# See LICENSE in the top level directory for licensing details
#

exec >&2

if ! [ -f "$1" ]; then
  cat<<EOF
Usage: $0 <sharedlib>

Tests for unresolved SST-DATA symbols in shared library
EOF
  exit 1
fi

if [ -z ${NO_COLOR+x} ] && tty -s; then
  RED="\033[91m"
  END="\033[0m"
else
  RED=
  END=
fi

printf "\n${RED}"
if ldd -r "$1" 2>&1 | c++filt | grep "\bundefined\b.*\bSST::Statistics::"; then
  rc=1
else
  rc=0
fi
printf "${END}"

[ "$rc" -ne 0 ] && echo ""

exit $rc
