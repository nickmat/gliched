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
    hicDatePhrase.cpp
    hicElement.cpp
    hicFormat.cpp
    hicFormatText.cpp
    hicFormatUnit.cpp
    hicGrammar.cpp
    hicGregorian.cpp
    hicHelper.cpp
    hicJulian.cpp
    hicLexicon.cpp
    hicLibScripts.cpp
    hicMath.cpp
    hicOptional.cpp
    hicRecord.cpp
    hicRomanNum.cpp
    hicScheme.cpp
    hicScript.cpp
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
    glcStore.h
    glcTokenStream.h
    glcValue.h
    hicBase.h
    hicDatePhrase.h
    hicElement.h
    hicFormat.h
    hicFormatText.h
    hicFormatUnit.h
    hicGrammar.h
    hicGregorian.h
    hicHelper.h
    hicJdn.h
    hicJulian.h
    hicLexicon.h
    hicLibScripts.h
    hicMath.h
    hicOptional.h
    hicRecord.h
    hicRomanNum.h
    hicScheme.h
    hicScript.h
)

add_library( glc ${GLC_SOURCES} ${GLC_HEADERS} )

target_link_libraries( glc PRIVATE utf8api )
