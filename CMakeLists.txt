# MIT License

# Copyright (c) 2021 Florian Eigentler

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
cmake_minimum_required( VERSION 3.15 )
enable_language( CXX )
enable_testing()
project( "raytracing" )
include( CTest )

# hide following contents from ccmake panel
mark_as_advanced( CMAKE_OSX_ARCHITECTURES )
mark_as_advanced( CMAKE_OSX_DEPLOYMENT_TARGET )
mark_as_advanced( CMAKE_OSX_SYSROOT )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED True )

# Compiler settings
if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
    set( ADDITIONAL_CXX_COMPILE_FLAGS "" )
    set( CMAKE_CXX_FLAGS_RELEASE "${ADDITIONAL_CXX_COMPILE_FLAGS} -O3" )
    set( CMAKE_CXX_FLAGS_DEBUG "${ADDITIONAL_CXX_COMPILE_FLAGS} -O0 -g -Wall -Wextra" )
    set( CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} -pg" )
elseif( CMAKE_CXX_COMPILER_ID MATCHES AppleClang )
    set( ADDITIONAL_CXX_COMPILE_FLAGS "" )
    set( CMAKE_CXX_FLAGS_RELEASE "${ADDITIONAL_CXX_COMPILE_FLAGS} -O3" )
    set( CMAKE_CXX_FLAGS_DEBUG "${ADDITIONAL_CXX_COMPILE_FLAGS} -O0 -g -Wall -Wextra" )
    set( CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} -pg" )
else()
   MESSAGE( FATAL_ERROR "Provided unsupported C compiler ${CMAKE_CXX_COMPILER_ID}!" )
endif()

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release" FORCE )
    set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release Profile )
endif()

if( CMAKE_BUILD_TYPE MATCHES "Debug" )
    add_definitions( "-DDEBUG" )
endif()

# System directories
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( CMAKE_MODULE_PATH              ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH} )

# User options
set( ASPECT_RATIO "16./9." CACHE STRING "" )
set( IMAGE_WIDTH "400" CACHE STRING "" )
set( SAMPLES_PER_PIXEL "100" CACHE STRING "" )
set( MAX_DEPTH "100" CACHE STRING "" )

add_definitions( -DASPECT_RATIO=${ASPECT_RATIO} )
add_definitions( -DIMAGE_WIDTH=${IMAGE_WIDTH} )
add_definitions( -DSAMPLES_PER_PIXEL=${SAMPLES_PER_PIXEL} )
add_definitions( -DMAX_DEPTH=${MAX_DEPTH} )

# Add subdirecotries
add_subdirectory( src )

file( MAKE_DIRECTORY results )
add_subdirectory( apps )