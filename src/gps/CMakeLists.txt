cmake_minimum_required(VERSION 3.0.2)
project(gps)

# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -Wall -std=c++11")
set(CMAKE_CXX_STANDARD 11)


SET(CMAKE_BUILD_TYPE "Release")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  nav_msgs
)

catkin_package(
  INCLUDE_DIRS include
)

link_directories(
	${PROJECT_SOURCE_DIR}/lib
)

include_directories(
  include
	${PROJECT_SOURCE_DIR}/src
	${catkin_INCLUDE_DIRS}
)

aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)

add_executable(${PROJECT_NAME}_node ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME}_node
    ${catkin_LIBRARIES})

