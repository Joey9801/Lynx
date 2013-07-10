#!/bin/bash

make clean
make
rm /media/sf_ARM_Programming_Shared_Folder/playground.hex
cp main.hex  /media/sf_ARM_Programming_Shared_Folder/playground.hex
