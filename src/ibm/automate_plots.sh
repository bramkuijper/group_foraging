#!/usr/bin/env bash

find . -iregex ".*sim_group_forage_.*[0-9]" -print0 | xargs -0 -P4 -I% ./plot_output.r %
find . -iregex ".*sim_group_forage_.*[0-9].*dynamics" -print0 | xargs -0 -P4 -I% ./plot_dynamics.r %

rm -rf Rplots.pdf
