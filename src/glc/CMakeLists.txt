# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        src/glc/CMakeLists.txt
# Project:     glc: Glich script library.
# Author:      Nick Matthews
# Website:     https://github.com/nickmat/glich
# Created:     8th February 2023
# Copyright:   Copyright (c) 2023, Nick Matthews.
# Licence:     GNU GPLv3
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set( GLC_SOURCES
    glcFile.cpp
    glcMark.cpp
    glcMath.cpp
    glcScript.cpp
    glcTokenStream.cpp
    glcValue.cpp
    glc.cpp
)

set( GLC_HEADERS
    ../../include/glc/glc.h
    ../../include/glc/glcDefs.h
    glcFile.h
    glcFunction.h
    glcMark.h
    glcMath.h
    glcScript.h
    glcScriptStore.h
    glcTokenStream.h
    glcValue.h
)

add_library( glc ${GLC_SOURCES} ${GLC_HEADERS} )

# target_link_libraries( glc PRIVATE utf8proc )
