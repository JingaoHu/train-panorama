#!/bin/bash
./record $1
cp $1 ../data/
./resize $1
./mosaic
cp result.bmp ../result/$1".bmp"
eog result.bmp
