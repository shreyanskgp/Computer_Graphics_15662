# Install script for directory: D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Scotty3D")
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

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/scotty3d.exe")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master" TYPE EXECUTABLE FILES "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/Debug/scotty3d.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/scotty3d.exe")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master" TYPE EXECUTABLE FILES "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/Release/scotty3d.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/scotty3d.exe")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master" TYPE EXECUTABLE FILES "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/MinSizeRel/scotty3d.exe")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/scotty3d.exe")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master" TYPE EXECUTABLE FILES "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/RelWithDebInfo/scotty3d.exe")
  endif()
endif()

