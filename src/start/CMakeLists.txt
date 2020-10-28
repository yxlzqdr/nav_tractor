project(start)
cmake_minimum_required(VERSION 2.8.3)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -Wall -std=c++11")

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

link_directories(
	${PROJECT_SOURCE_DIR}/lib
)

catkin_package()

include_directories(
	${PROJECT_SOURCE_DIR}/src
	${catkin_INCLUDE_DIRS}
)
include_directories(BEFORE ${GLOG_INCLUDE_DIRS}) 

aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)

add_executable(${PROJECT_NAME}_node ${DIR_SRCS})
		
target_link_libraries(${PROJECT_NAME}_node
    ${catkin_LIBRARIES})
