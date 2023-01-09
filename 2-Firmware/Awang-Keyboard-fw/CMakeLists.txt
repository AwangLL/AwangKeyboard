#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
cmake_minimum_required(VERSION 3.20.0)

# SET ROOT
set(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project("Awang Keyboard")

# NORDIC SDK APP START
target_sources(app PRIVATE
  src/main.c
)
# NORDIC SDK APP END

# Include application events and configuration headers
zephyr_library_include_directories(
  src/events
  src/utils
)

zephyr_include_directories(
  configuration
)

# Application sources
add_subdirectory(src/events)
add_subdirectory(src/modules)
add_subdirectory(src/utils)