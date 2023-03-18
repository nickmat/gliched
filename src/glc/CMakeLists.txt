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
    hicBase.cpp
    hicCreateSch.cpp
    hicDatePhrase.cpp
    hicHelper.cpp
    hicJdn.cpp
    hicJulian.cpp
    hicMath.cpp
    hicRecord.cpp
    hicRomanNum.cpp
    hicScheme.cpp
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
    hicBase.h
    hicCreateSch.h
    hicDatePhrase.h
    hicHelper.h
    hicJdn.h
    hicJulian.h
    hicMath.h
    hicRecord.h
    hicRomanNum.h
    hicScheme.h
)

add_library( glc ${GLC_SOURCES} ${GLC_HEADERS} )

target_link_libraries( glc PRIVATE utf8api )
