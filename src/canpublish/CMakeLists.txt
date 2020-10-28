cmake_minimum_required(VERSION 2.8.3)
project(canpublish)

set(CMAKE_CXX_STANDARD 11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES canpublish
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)


include_directories(
  ${catkin_INCLUDE_DIRS}
)

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/)

add_executable(canpublish canpublish.cpp  thirdparty/include/controlcan.h
               candriver.h candriver.cpp
               carmontioncontrol.cpp carmontioncontrol.h
               comm.h)
target_link_libraries(canpublish  ${QT_LIBRARIES} ${catkin_LIBRARIES})
target_link_libraries(canpublish  libusbcan.so)
