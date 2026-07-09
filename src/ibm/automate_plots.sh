#!/usr/bin/env bash

find . -iname "sim_group_forage*" -print0 | xargs -0 -P4 -I% ./plot_output.r %
