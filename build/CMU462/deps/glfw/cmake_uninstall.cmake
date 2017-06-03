
if (NOT EXISTS "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/CMU462/deps/glfw/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/CMU462/deps/glfw/install_manifest.txt\"")
endif()

file(READ "D:/Desktop/CMU/03 Fall 2016/02 Computer Graphics (15-662)/01 Assignments/05 HW5/Scotty3D-master/build/CMU462/deps/glfw/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")

foreach (file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if (EXISTS "$ENV{DESTDIR}${file}")
    exec_program("D:/Programs/CMake/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
  elseif (IS_SYMLINK "$ENV{DESTDIR}${file}")
    EXEC_PROGRAM("D:/Programs/CMake/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing symlink \"$ENV{DESTDIR}${file}\"")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()

