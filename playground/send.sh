#!/bin/bash

make clean
make
rm /media/sf_ARM_Programming_Shared_Folder/playground.hex
cp playground.hex  /media/sf_ARM_Programming_Shared_Folder/playground.hex
