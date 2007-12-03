#!/bin/sh

gcc plugin.cpp plugin_mplayer.cpp tools.cpp -o fake_mplayer.so -nostart -lbe -I./include/
mergeres resources/resource-mplayer.rsrc fake_mplayer.so
mimeset -f fake_mplayer.so

