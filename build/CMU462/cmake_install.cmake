# Install script for directory: D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/CMU462

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/CMU462")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/CMU462/deps/freetype/cmake_install.cmake")
  include("D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/CMU462/deps/glew/cmake_install.cmake")
  include("D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/CMU462/deps/glfw/cmake_install.cmake")
  include("D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/CMU462/src/cmake_install.cmake")

endif()

