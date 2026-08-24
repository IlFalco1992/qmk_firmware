#!/usr/bin/env sh
qmk compile -j 0 -kb keychron/k8_pro/ansi/rgb -km via
qmk flash -kb keychron/k8_pro/ansi/rgb -km via
