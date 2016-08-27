#!/usr/bin/bash
export PYSIDE_LOC=/data/share/usd/dependency/pyside-1.2.2
export PYTHON_LOC=/data/share/usd/dependency/python-2.7.5
export QT_LOC=/data/share/usd/dependency/qt-4.8.1
export PATH=$QT_LOC/gcc/bin:$PYTHON_LOC/bin:$PATH
export LD_LIBRARY_PATH=$PYTHON_LOC/lib:$QT_LOC/gcc/lib:$LD_LIBRARY_PATH

export PATH=$PYSIDE_LOC/bin:$QT_LOC/bin:$PATH
export PYTHONPATH=$PYTHON_LOC/lib/python2.7:$PYSIDE_LOC/lib/python2.7/site-packages:$PYTHONPATH
export LD_LIBRARY_PATH=$PYSIDE_LOC/lib:$QT_LOC/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$PYSIDE_LOC/lib/pkgconfig:$PKG_CONFIG_PATH
# set -DENABLE_ICECC=1 if you're using the icecream distributed compiler
alias runcmake='cmake .. -DCMAKE_INSTALL_PREFIX=$PYSIDE_LOC -DCMAKE_BUILD_TYPE=Debug -DENABLE_ICECC=0'
