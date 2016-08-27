#!/usr/bin/bash
export DEPS_ROOT=/data/share/usd/dependency

export MAYA_LOCATION=/apps/shared/maya/2016
export MAYA_tbb_LIBRARY=$DEPS_ROOT/tbb-4.3.1/libtbb.so

export KATANA_LOCATION=/apps/shared/katana/2.1.1
export KATANA_API_LOCATION=$KATANA_LOCATION
#export KATANA_API_BASE_DIR=$KATANA_API_LOCATION
export KATANA_API_INCLUDE_DIR=$KATANA_API_LOCATION/plugin_apis/include
# export KATANA_API_INCLUDE_DIRS=$KATANA_API_LOCATION/include
export KATANA_API_SOURCE_DIR=$KATANA_API_LOCATION/plugin_apis/src

export ALEMBIC_LOCATION=/apps/shared/maya/2016/devkit/Alembic
#export ALEMBIC_DIR=/apps/shared/maya/2016/devkit/Alembic
#export ALEMBIC_LIBRARIES=$ALEMBIC_LOCATION/
#export ALEMBIC_LIBRARY_DIR=$ALEMBIC_LOCATION/lib
#export ALEMBIC_INCLUDE_DIR=$ALEMBIC_LOCATION/include

export HDF5_LOCATION=$DEPS_ROOT/hdf5-1.8.17
#export HDF5_LIBRARIES=$HDF5_LOCATION/lib/libhdf5.so
#export HDF5_HL_LIBRARIES=$HDF5_LOCATION/lib/libhdf5_hl.so
#export HDF5_INCLUDE_DIRS=$HDF5_LOCATION/include

export OPENEXR_ROOT_DIR=$DEPS_ROOT/openexr-2.2.0
export OPENEXR_LIBRARY_DIR=$OPENEXR_ROOT_DIR/lib
export LD_LIBRARY_PATH=/apps/shared/python/2.7.5/lib:$DEPS_ROOT/tbb-4.3.1:$DEPS_ROOT/openexr-2.2.0/lib:$DEPS_ROOT/qt-4.8.1/lib:$DEPS_ROOT/pyside-1.2.2/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH:$DEPS_ROOT/pyside-1.2.2/lib/python2.7/site-packages

source $DEPS_ROOT/tbb-4.3.1/tbbvars.sh

# https://github.com/PixarAnimationStudios/USD/issues/16
cmake -DTBB_tbb_LIBRARY=$DEPS_ROOT/tbb-4.3.1/libtbb.so -DOPENEXR_LOCATION=$DEPS_ROOT/openexr-2.2.0 -DOPENSUBDIV_LOCATION=$DEPS_ROOT/OpenSubdiv-3_0_5 -DPTEX_INCLUDE_DIR=$DEPS_ROOT/ptex-2.0.30 -DOIIO_BASE_DIR=$DEPS_ROOT/oiio-Release-1.5.11 -DBOOST_ROOT=$DEPS_ROOT/boost_1_55_0 -DQT_QMAKE_EXECUTABLE=$DEPS_ROOT/qt-4.8.1/bin/qmake -DCMAKE_INSTALL_PREFIX=/apps/shared/usd/0.7.0 -DPXR_BUILD_MAYA_PLUGIN=TRUE -DPXR_BUILD_KATANA_PLUGIN=TRUE -DPXR_BUILD_ALEMBIC_PLUGIN=FALSE -DPXR_MAYA_TBB_BUG_WORKAROUND=TRUE ..
