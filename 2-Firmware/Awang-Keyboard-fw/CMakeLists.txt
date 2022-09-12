# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(AwangKeyboard)

target_sources(app PRIVATE 
src/Keyboard/Led.cpp
src/Keyboard/Matrix.cpp
src/Keyboard/Power.cpp
src/Drivers/Usb.cpp
src/Drivers/Gpio.cpp
src/Drivers/Nvs.cpp
src/Drivers/Timer.cpp
src/main.cpp
)