#!/usr/bin/env bash

cmake -B build
cmake --build build

mkdir -p output
build/PA2 testcases/scene01_basic.txt output/scene01.bmp
build/PA2 testcases/scene04_axes.txt output/scene04.bmp
build/PA2 testcases/scene06_bunny_1k.txt output/scene06.bmp
build/PA2 testcases/scene08_core.txt output/scene08.bmp
build/PA2 testcases/scene09_norm.txt output/scene09.bmp
build/PA2 testcases/scene10_wineglass.txt output/scene10.bmp
