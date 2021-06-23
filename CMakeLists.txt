####################################################################################################################################
# Raytracing library
# (c) 2021 | Florian Eigentler
####################################################################################################################################
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

####################################################################################################################################
# Compiler settings
#-----------------------------------------------------------------------------------------------------------------------------------
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

####################################################################################################################################
# System directories
#-----------------------------------------------------------------------------------------------------------------------------------
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( CMAKE_MODULE_PATH              ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH} )

####################################################################################################################################
# User options
#-----------------------------------------------------------------------------------------------------------------------------------
set( ASPECT_RATIO "16./9." CACHE STRING "" )
set( IMAGE_WIDTH "400" CACHE STRING "" )
set( SAMPLES_PER_PIXEL "5" CACHE STRING "" )
set( MAX_DEPTH "5" CACHE STRING "" )

add_definitions( -DASPECT_RATIO=${ASPECT_RATIO} )
add_definitions( -DIMAGE_WIDTH=${IMAGE_WIDTH} )
add_definitions( -DSAMPLES_PER_PIXEL=${SAMPLES_PER_PIXEL} )
add_definitions( -DMAX_DEPTH=${MAX_DEPTH} )

####################################################################################################################################
# Add subdirecotries
#-----------------------------------------------------------------------------------------------------------------------------------
add_subdirectory( src )

file( MAKE_DIRECTORY results )
add_subdirectory( apps )