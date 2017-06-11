#                                               -*- cmake -*-
#
#  LibFTDI1Config.cmake(.in)
#
#  Copyright (C) 2013 Intra2net AG and the libftdi developers
#
#  This file is part of LibFTDI.
#
#  LibFTDI is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# Use the following variables to compile and link against LibFTDI:
#  LIBFTDI_FOUND              - True if LibFTDI was found on your system
#  LIBFTDI_USE_FILE           - The file making LibFTDI usable
#  LIBFTDI_DEFINITIONS        - Definitions needed to build with LibFTDI
#  LIBFTDI_INCLUDE_DIRS       - Directory where ftdi.h can be found
#  LIBFTDI_INCLUDE_DIRS       - List of directories of LibFTDI and it's dependencies
#  LIBFTDI_LIBRARY            - LibFTDI library location
#  LIBFTDI_LIBRARIES          - List of libraries to link against LibFTDI library
#  LIBFTDIPP_LIBRARY          - LibFTDI C++ wrapper library location
#  LIBFTDIPP_LIBRARIES        - List of libraries to link against LibFTDI C++ wrapper
#  LIBFTDI_LIBRARY_DIRS       - List of directories containing LibFTDI' libraries
#  LIBFTDI_ROOT_DIR           - The base directory of LibFTDI
#  LIBFTDI_VERSION_STRING     - A human-readable string containing the version
#  LIBFTDI_VERSION_MAJOR      - The major version of LibFTDI
#  LIBFTDI_VERSION_MINOR      - The minor version of LibFTDI
#  LIBFTDI_VERSION_PATCH      - The patch version of LibFTDI
#  LIBFTDI_PYTHON_MODULE_PATH - Path to the python module

set ( LIBFTDI_FOUND 1 )
set ( LIBFTDI_USE_FILE     "/mingw32/lib/cmake/libftdi1/UseLibFTDI1.cmake" )

set ( LIBFTDI_DEFINITIONS  "" )
set ( LIBFTDI_INCLUDE_DIR  "/mingw32/include/libftdi1" )
set ( LIBFTDI_INCLUDE_DIRS "/mingw32/include/libftdi1" )
set ( LIBFTDI_LIBRARY      "ftdi1" )
set ( LIBFTDI_LIBRARIES    "ftdi1;/mingw32/lib/libusb-1.0.dll.a" )
set ( LIBFTDI_STATIC_LIBRARY      "ftdi1.a" )
set ( LIBFTDI_STATIC_LIBRARIES    "ftdi1.a;/mingw32/lib/libusb-1.0.dll.a" )
set ( LIBFTDIPP_LIBRARY    "ftdipp1" )
set ( LIBFTDIPP_LIBRARIES  "ftdipp1;/mingw32/lib/libusb-1.0.dll.a" )
set ( LIBFTDI_LIBRARY_DIRS "/mingw32/bin" )
set ( LIBFTDI_ROOT_DIR     "/mingw32" )

set ( LIBFTDI_VERSION_STRING "1.3" )
set ( LIBFTDI_VERSION_MAJOR  "1" )
set ( LIBFTDI_VERSION_MINOR  "3" )
set ( LIBFTDI_VERSION_PATCH  "" )

set ( LIBFTDI_PYTHON_MODULE_PATH "/mingw32/lib/python3.5/site-packages" )

