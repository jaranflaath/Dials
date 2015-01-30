Dials Watchface
=================

A watchface for Pebble based on dials displaying seconds, minutes, hours and days.

Using JetBrains CLion EAP
=========================

If you're using CLion (or possibly any other C-IDE), you need to add the following CMakeLists.txt file to the project: 

 cmake_minimum_required(VERSION 2.8.4)
 project(Dials)
 include_directories(PATH/TO/PEBBLESDK/Pebble/include)
 set(SOURCE_FILES src/main.c )
 add_executable(Dials ${SOURCE_FILES})
