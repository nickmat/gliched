# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        src/gliched/CMakeLists.txt
# Project:     gliched: Glich Script Language IDE.
# Author:      Nick Matthews
# Created:     19th September 2023
# Copyright:   Copyright (c) 2023, Nick Matthews.
# Licence:     GNU GPLv3
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set(GED_HEADERS
  fbGedFrame.h
  gedApp.h
  gedFrame.h
)
set(GED_SOURCES
  fbGedFrame.cpp
  gedApp.cpp
  gedFrame.cpp
)

add_executable(gliched WIN32 ${GED_SOURCES} ${GED_HEADERS} gliched.rc)

target_link_libraries (gliched PUBLIC glc wx::stc wx::net wx::core wx::base)
