cmake_minimum_required(VERSION 2.6)

option(NATIVE_VIBRATOR "Use vibra directly through kernel interface instead of using HAL" OFF)

project(ngfd-plugin-droid-vibrator)

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(NGFD_PLUGIN REQUIRED ngf-plugin)

if(NATIVE_VIBRATOR)
    add_definitions(-DNATIVE_VIBRATOR=1)
else()
    pkg_check_modules(ANDROID_HEADERS REQUIRED android-headers)
    pkg_check_modules(DROID_VIBRATOR REQUIRED libvibrator)
    pkg_check_modules(LIBHARDWARE REQUIRED libhardware)
endif()

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${NGFD_PLUGIN_INCLUDE_DIRS})
if(NOT NATIVE_VIBRATOR)
    include_directories(${ANDROID_HEADERS_INCLUDE_DIRS})
    include_directories(${DROID_VIBRATOR_INCLUDE_DIRS})
    include_directories(${LIBHARDWARE_INCLUDE_DIRS})
endif()

if(NATIVE_VIBRATOR)
    add_library(ngfd_droid-vibrator SHARED src/droid-vibrator.c src/native.c)
    target_link_libraries(ngfd_droid-vibrator ${NGFD_PLUGIN_LIBRARIES})
else()
    add_library(ngfd_droid-vibrator SHARED src/droid-vibrator.c src/hal.c)
    target_link_libraries(ngfd_droid-vibrator ${NGFD_PLUGIN_LIBRARIES})
    target_link_libraries(ngfd_droid-vibrator ${DROID_VIBRATOR_LIBRARIES})
    target_link_libraries(ngfd_droid-vibrator ${LIBHARDWARE_LIBRARIES})
endif()

install(TARGETS ngfd_droid-vibrator LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/ngf)
install(FILES 50-droid-vibrator.ini DESTINATION share/ngfd/plugins.d)
