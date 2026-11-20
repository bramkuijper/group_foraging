#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

find . -iregex ".*sim_group_forage_.*[0-9]" -print0 | xargs -0 -P4 -I% $SCRIPT_DIR/plot_output.r %
find . -iregex ".*sim_group_forage_.*[0-9].*dynamics" -print0 | xargs -0 -P4 -I% $SCRIPT_DIR/plot_dynamics.r %

rm -rf Rplots.pdf
