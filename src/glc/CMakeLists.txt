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
    glc.cpp
    glcFile.cpp
    glcHelper.cpp
    glcMark.cpp
    glcMath.cpp
    glcObject.cpp
    glcScript.cpp
    glcTokenStream.cpp
    glcValue.cpp
    hicDatePhrase.cpp
    hicRomanNum.cpp
)

set( GLC_HEADERS
    ../../include/glc/glc.h
    ../../include/glc/glcDefs.h
    ../../include/glc/hicDefs.h
    glcFile.h
    glcFunction.h
    glcHelper.h
    glcMark.h
    glcMath.h
    glcObject.h
    glcScript.h
    glcScriptStore.h
    glcTokenStream.h
    glcValue.h
    hicDatePhrase.h
    hicRomanNum.h
)

add_library( glc ${GLC_SOURCES} ${GLC_HEADERS} )

target_link_libraries( glc PRIVATE utf8api )
