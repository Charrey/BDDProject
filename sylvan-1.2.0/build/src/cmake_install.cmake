# Install script for directory: /home/hans/sylvan-1.2.0/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsylvan.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsylvan.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsylvan.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/hans/sylvan-1.2.0/build/src/libsylvan.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsylvan.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsylvan.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsylvan.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/hans/sylvan-1.2.0/src/lace.h"
    "/home/hans/sylvan-1.2.0/src/sylvan.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_bdd.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_cache.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_config.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_common.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_gmp.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_int.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_ldd.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_ldd_int.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_mt.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_mtbdd.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_mtbdd_int.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_obj.hpp"
    "/home/hans/sylvan-1.2.0/src/sylvan_stats.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_table.h"
    "/home/hans/sylvan-1.2.0/src/sylvan_tls.h"
    )
endif()

