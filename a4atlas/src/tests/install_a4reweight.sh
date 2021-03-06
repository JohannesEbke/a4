#! /usr/bin/env bash

set -e -u

export A4_LOG_LEVEL=5

source ${BINDIR}/this_a4.sh
export PATH=$PATH:.:${SRCDIR}/a4io/src/tests

# NOTE: This test is not valid unless "./waf install" has been done.

a4reweight --run-number --per run -l 1 -x data/xsdata data/salient-realworld.a4

