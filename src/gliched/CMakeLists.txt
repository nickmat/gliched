# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        src/gliched/CMakeLists.txt
# Project:     gliched: Glich Script Language IDE.
# Author:      Nick Matthews
# Created:     19th September 2023
# Copyright:   Copyright (c) 2023..2026, Nick Matthews.
# Licence:     GNU GPLv3
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set(GE_HEADERS
  geEditor.h
  geImages.h
  geMainFrame.h
)

set(GE_SOURCES
  geApp.cpp
  geEditor.cpp
  geMainFrame.cpp
)

add_executable(gliched WIN32 ${GE_SOURCES} ${GE_HEADERS} gliched.rc)

target_link_libraries (gliched PUBLIC hic glc wx::aui wx::stc wx::net wx::core wx::base)
