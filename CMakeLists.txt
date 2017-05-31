cmake_minimum_required(VERSION 2.6)

project(ngfd-plugin-droid-vibrator)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(NGFD_PLUGIN REQUIRED ngf-plugin)
pkg_check_modules(ANDROID_HEADERS REQUIRED android-headers)
pkg_check_modules(DROID_VIBRATOR REQUIRED libvibrator)
pkg_check_modules(LIBHARDWARE REQUIRED libhardware)

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${NGFD_PLUGIN_INCLUDE_DIRS})
include_directories(${ANDROID_HEADERS_INCLUDE_DIRS})
include_directories(${DROID_VIBRATOR_INCLUDE_DIRS})
include_directories(${LIBHARDWARE_INCLUDE_DIRS})

add_library(ngfd_droid-vibrator SHARED src/droid-vibrator.c)

target_link_libraries(ngfd_droid-vibrator ${NGFD_PLUGIN_LIBRARIES})
target_link_libraries(ngfd_droid-vibrator ${DROID_VIBRATOR_LIBRARIES})
target_link_libraries(ngfd_droid-vibrator ${LIBHARDWARE_LIBRARIES})

install(TARGETS ngfd_droid-vibrator LIBRARY DESTINATION lib/ngf)
install(FILES 50-droid-vibrator.ini DESTINATION share/ngfd/plugins.d)
