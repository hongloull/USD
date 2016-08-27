project(usd)
cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/defaults
                      ${CMAKE_SOURCE_DIR}/cmake/modules
                      ${CMAKE_SOURCE_DIR}/cmake/macros)

set(DEPS_ROOT /data/share/usd/dependency)

set(PYTHON_ROOT /apps/shared/python/2.7.5)
set(PYTHON_EXECUTABLE ${PYTHON_ROOT}/bin/python)
set(PYTHON_LIBRARY ${PYTHON_ROOT}/lib/libpython2.7.so)
set(PYTHON_INCLUDE_DIR ${PYTHON_ROOT}/include)

set(BOOST_INCLUDE_DIR ${DEPS_ROOT}/boost_1_55_0/include)

set(DOUBLE_CONVERSION_LIBRARY ${DEPS_ROOT}/double-conversion-1.1.5/lib/libdouble-conversion.so)
set(DOUBLE_CONVERSION_INCLUDE_DIR ${DEPS_ROOT}/double-conversion-1.1.5/include)

set(OPENEXR_ROOT_DIR ${DEPS_ROOT}/openexr-2.2.0)
set(OPENEXR_INCLUDE_DIR ${OPENEXR_ROOT_DIR}/include)
set(OPENEXR_LIBRARY_DIR ${OPENEXR_ROOT_DIR}/lib)
set(OPENEXR_Half_LIBRARY ${DEPS_ROOT}/ilmbase-2.2.0/lib/libHalf.so)

set(GLEW_LIBRARY ${DEPS_ROOT}/glew-1.10.0/lib64/libGLEW.so)
set(GLEW_INCLUDE_DIR ${DEPS_ROOT}/glew-1.10.0/include)

set(OPENSUBDIV_ROOT_DIR ${DEPS_ROOT}/OpenSubdiv-3_0_5)
set(OPENSUBDIV_INCLUDE_DIR ${OPENSUBDIV_ROOT_DIR}/include)
set(OPENSUBDIV_LIBRARIES ${OPENSUBDIV_ROOT_DIR}/lib)

set(PTEX_LIBRARY ${DEPS_ROOT}/ptex-2.0.30/lib/libPtex.so)
set(PTEX_INCLUDE_DIR ${DEPS_ROOT}/ptex-2.0.30/include)

#set(PYSIDEUIC4BINARY ${DEPS_ROOT}/pyside-1.2.2/bin/pyside-uic)
set(PYSIDEUICBINARY ${DEPS_ROOT}/pyside-1.2.2/bin/pyside-uic)
set(PYSIDERCC4BINARY ${DEPS_ROOT}/pyside-1.2.2/bin/pyside-rcc)

set(BISON_EXECUTABLE ${DEPS_ROOT}/bison-2.4.1/bin/bison)
set(FLEX_EXECUTABLE ${DEPS_ROOT}/flex-2.5.39/bin/flex)

include(Options)
include(ProjectDefaults)
include(Packages)

# This has to be defined after Packages is included, because it relies on the
# discovered path to the python executable.
set(PXR_PYTHON_SHEBANG "${PYTHON_EXECUTABLE}"
    CACHE
    STRING
    "Replacement path for Python #! line."
)

# CXXDefaults will set a variety of variables for the project.
# Consume them here. This is an effort to keep the most common
# build files readable.
include(CXXDefaults)
add_definitions(${_PXR_CXX_DEFINITIONS})
set(CMAKE_CXX_FLAGS ${_PXR_CXX_FLAGS})

include(Public)

add_subdirectory(pxr)
add_subdirectory(extras)

function(_addIfExists pkg pkgCmakeLists)
    if (EXISTS ${pkgCmakeLists})
        add_subdirectory(${pkg})
    else()
        message(WARNING "Specified third party package '${pkgName}' was not found. Please check that '${pkgCmakeLists}' exists")
    endif()
endfunction()

function(_enableThirdPartyPlugin identifier)
    set(pkg third_party/${identifier})
    set(pkgCmakeLists ${CMAKE_CURRENT_SOURCE_DIR}/${pkg}/CMakeLists.txt)
    _addIfExists(${pkg} ${pkgCmakeLists})
endfunction()

if (${PXR_BUILD_KATANA_PLUGIN})
    if (NOT ${PXR_BUILD_USD_IMAGING})
        message(WARNING "Skipping third_party/katana because PXR_BUILD_USD_IMAGING=OFF")
    else()
        _enableThirdPartyPlugin("katana")
    endif()
endif()

if (${PXR_BUILD_MAYA_PLUGIN})
    if (NOT ${PXR_BUILD_USD_IMAGING})
        message(WARNING "Skipping third_party/maya because PXR_BUILD_USD_IMAGING=OFF")
    else()
        _enableThirdPartyPlugin("maya")
    endif()
endif()
