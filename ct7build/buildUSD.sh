#!/usr/bin/bash
cd ~/workspace/USD
rm -rf build
mkdir build
cd build
source ~/workspace/USD/ct7build/runCMake.sh

mkdir include
ln -s /data/share/usd/dependency/OpenSubdiv-3_0_5/include/opensubdiv ~/workspace/USD/build/include/opensubdiv3
ln -s /data/share/usd/dependency/boost_1_55_0/include/boost ~/workspace/USD/build/include
ln -s /data/share/usd/dependency/double-conversion-1.1.5/include/double-conversion ~/workspace/USD/build/include

optirun make -j 8 install
