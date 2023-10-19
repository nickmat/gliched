# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        src/gliched/CMakeLists.txt
# Project:     gliched: Glich Script Language IDE.
# Author:      Nick Matthews
# Created:     19th September 2023
# Copyright:   Copyright (c) 2023, Nick Matthews.
# Licence:     GNU GPLv3
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

set(GE_HEADERS
  fbGeFrame.h
  geApp.h
  geEdit.h
  geFrame.h
  geVersion.h
)

set(GE_SOURCES
  fbGeFrame.cpp
  geApp.cpp
  geEdit.cpp
  geFrame.cpp
  geVersion.cpp
)

add_executable(gliched WIN32 ${GE_SOURCES} ${GE_HEADERS} gliched.rc)

target_link_libraries (gliched PUBLIC glc wx::stc wx::net wx::core wx::base)
