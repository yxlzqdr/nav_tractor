# Install script for directory: /home/green/nav_tractor/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/green/nav_tractor/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
        file(MAKE_DIRECTORY "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
      endif()
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin")
        file(WRITE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin" "")
      endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/green/nav_tractor/install/_setup_util.py")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/green/nav_tractor/install" TYPE PROGRAM FILES "/home/green/nav_tractor/build/catkin_generated/installspace/_setup_util.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/green/nav_tractor/install/env.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/green/nav_tractor/install" TYPE PROGRAM FILES "/home/green/nav_tractor/build/catkin_generated/installspace/env.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/green/nav_tractor/install/setup.bash;/home/green/nav_tractor/install/local_setup.bash")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/green/nav_tractor/install" TYPE FILE FILES
    "/home/green/nav_tractor/build/catkin_generated/installspace/setup.bash"
    "/home/green/nav_tractor/build/catkin_generated/installspace/local_setup.bash"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/green/nav_tractor/install/setup.sh;/home/green/nav_tractor/install/local_setup.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/green/nav_tractor/install" TYPE FILE FILES
    "/home/green/nav_tractor/build/catkin_generated/installspace/setup.sh"
    "/home/green/nav_tractor/build/catkin_generated/installspace/local_setup.sh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/green/nav_tractor/install/setup.zsh;/home/green/nav_tractor/install/local_setup.zsh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/green/nav_tractor/install" TYPE FILE FILES
    "/home/green/nav_tractor/build/catkin_generated/installspace/setup.zsh"
    "/home/green/nav_tractor/build/catkin_generated/installspace/local_setup.zsh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/green/nav_tractor/install/.rosinstall")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/green/nav_tractor/install" TYPE FILE FILES "/home/green/nav_tractor/build/catkin_generated/installspace/.rosinstall")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/green/nav_tractor/build/gtest/cmake_install.cmake")
  include("/home/green/nav_tractor/build/car_navigation/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/navigation/cmake_install.cmake")
  include("/home/green/nav_tractor/build/pub_global_plan/cmake_install.cmake")
  include("/home/green/nav_tractor/build/start/cmake_install.cmake")
  include("/home/green/nav_tractor/build/canpublish/cmake_install.cmake")
  include("/home/green/nav_tractor/build/gps/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/map_server/cmake_install.cmake")
  include("/home/green/nav_tractor/build/gps_common/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/amcl/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/robot_pose_ekf/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/fake_localization/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/voxel_grid/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/costmap_2d/cmake_install.cmake")
  include("/home/green/nav_tractor/build/costmap_converter-master/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/nav_core/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/base_local_planner/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/carrot_planner/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/clear_costmap_recovery/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/dwa_local_planner/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/move_slow_and_clear/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/navfn/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/global_planner/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/rotate_recovery/cmake_install.cmake")
  include("/home/green/nav_tractor/build/navigation-kinetic-devel/move_base/cmake_install.cmake")
  include("/home/green/nav_tractor/build/teb_local_planner-kinetic-devel/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/green/nav_tractor/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
