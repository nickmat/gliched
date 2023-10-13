# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        CMakeLists.txt
# Project:     gliched: Glich Script Language IDE.
# Author:      Nick Matthews
# Website:     http://historycal.org/
# Created:     19th September 2023
# Copyright:   Copyright (c) 2023, Nick Matthews.
# Licence:     GNU GPLv3
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

cmake_minimum_required( VERSION 3.25 )

project( gliched )

option( BUILD_SHARED_LIBS "Set static libs" OFF )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED On )

# Leave this here for when we want to check for memory leaks.
#add_compile_options(-fsanitize=address)

include_directories( include )

add_subdirectory( 3rdparty/glich )
include_directories( 3rdparty/glich/include )
add_subdirectory( 3rdparty/wxWidgets )
add_subdirectory( src/gliched )
