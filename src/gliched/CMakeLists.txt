# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Name:        src/hcal/CMakeLists.txt
# Project:     hcal: HistoryCal Application.
# Author:      Nick Matthews
# Website:     http://historycal.org/
# Created:     22nd January 2021
# Copyright:   Copyright (c) 2021..2023, Nick Matthews.
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

add_executable(gliched WIN32 ${GED_SOURCES} ${GED_HEADERS})

target_link_libraries (gliched PUBLIC glc wx::stc wx::net wx::core wx::base)
