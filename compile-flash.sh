#!/bin/sh

gcc plugin.cpp plugin_flash.cpp tools.cpp -o fake_flashplayer.so -nostart -lbe -I./include/
mergeres resources/resource-flash.rsrc fake_flashplayer.so
mimeset -f fake_flashplayer.so

