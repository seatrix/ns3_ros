cmake_minimum_required(VERSION 2.8.3)
project(ns3_ros CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

SET(MODULES_DIR "modules" )

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
		roscpp
		rospy
		std_msgs
		actionlib
		actionlib_msgs
		roslaunch
		message_generation
    std_srvs
    tf
)


#http://stackoverflow.com/questions/7787823/cmake-how-to-get-the-name-of-all-subdirectories-of-a-directory
function(GETPROJECTS result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*/CMakeLists.txt)
  SET(dirlist "")
  FOREACH(file ${children})
        get_filename_component(subdir ${file} DIRECTORY)
        LIST(APPEND dirlist ${subdir})
  ENDFOREACH()
  SET(${result} ${dirlist} PARENT_SCOPE)
endfunction()

#BEGIN RESOLVE GIT REVISION
#https://bravenewmethod.com/2017/07/14/git-revision-as-compiler-definition-in-build-with-cmake/
#message(STATUS "Resolving GIT Version...")
#set(_build_version "unknown")
#find_package(Git)
#if(GIT_FOUND)
#  execute_process(
#    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
#    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#    OUTPUT_VARIABLE _build_version
#    ERROR_QUIET
#    OUTPUT_STRIP_TRAILING_WHITESPACE
#  )
#  message( STATUS "GIT hash: ${_build_version}")
#else()
#  message(STATUS "GIT not found")
#endif()
#string(TIMESTAMP _time_stamp)
#END RESOLVE GIT REVISION


####### HANDLE DEPENDENCIES ######
#SET(DEPS_DIR ${MODULES_DIR})
#SET(TMP0 ${CMAKE_CURRENT_SOURCE_DIR}/${DEPS_DIR})
#GETPROJECTS(DEPS ${TMP0})
#message(STATUS "Dependencies found in ${TMP0}: ${DEPS}")
#FOREACH(subdir ${DEPS})
#if(GIT_FOUND)
#  SET(SUBPROJECT_PATH ${DEPS_DIR}/${subdir})
#  SET(SUBPROJECT_FULL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${SUBPROJECT_PATH})
#  execute_process(
#    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
#    WORKING_DIRECTORY ${SUBPROJECT_FULL_PATH}
#    OUTPUT_VARIABLE _sub_build_version
#    ERROR_QUIET
#    OUTPUT_STRIP_TRAILING_WHITESPACE
#  )
#  SET(SUBPROJECT_GIT_NAME ${subdir}_${_sub_build_version})
#  message(STATUS "Subproject generated git name: " ${SUBPROJECT_GIT_NAME})
#  if (TARGET ${subdir})
#      message(WARNING "Subproject ${SUBPROJECT_GIT_NAME} (${subdir}) has already been built!")
#  else()
#     add_subdirectory(${SUBPROJECT_PATH})
#  endif()
#else()
# message(STATUS "GIT not found")
#endif()
#ENDFOREACH()



SET(NS3_BUILD ${CMAKE_CURRENT_SOURCE_DIR}/modules/ns-3-dev/build/)
execute_process(
  COMMAND mkdir -p ${NS3_BUILD}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

SET(NS3_LIBS
  libns3.30.1-antenna-debug.so
  libns3.30.1-antenna-test-debug.so
  libns3.30.1-aodv-debug.so
  libns3.30.1-aodv-test-debug.so
  libns3.30.1-applications-debug.so
  libns3.30.1-applications-test-debug.so
  libns3.30.1-bridge-debug.so
  libns3.30.1-buildings-debug.so
  libns3.30.1-buildings-test-debug.so
  libns3.30.1-config-store-debug.so
  libns3.30.1-core-debug.so
  libns3.30.1-core-test-debug.so
  libns3.30.1-csma-debug.so
  libns3.30.1-csma-layout-debug.so
  libns3.30.1-dsdv-debug.so
  libns3.30.1-dsdv-test-debug.so
  libns3.30.1-dsr-debug.so
  libns3.30.1-dsr-test-debug.so
  libns3.30.1-energy-debug.so
  libns3.30.1-energy-test-debug.so
  libns3.30.1-fd-net-device-debug.so
  libns3.30.1-flow-monitor-debug.so
  libns3.30.1-flow-monitor-test-debug.so
  libns3.30.1-internet-apps-debug.so
  libns3.30.1-internet-apps-test-debug.so
  libns3.30.1-internet-debug.so
  libns3.30.1-internet-test-debug.so
  libns3.30.1-lr-wpan-debug.so
libns3.30.1-lr-wpan-test-debug.so
libns3.30.1-lte-debug.so
libns3.30.1-lte-test-debug.so
libns3.30.1-mesh-debug.so
libns3.30.1-mesh-test-debug.so
libns3.30.1-mobility-debug.so
libns3.30.1-mobility-test-debug.so
libns3.30.1-mpi-debug.so
libns3.30.1-netanim-debug.so
libns3.30.1-netanim-test-debug.so
libns3.30.1-network-debug.so
libns3.30.1-network-test-debug.so
libns3.30.1-nix-vector-routing-debug.so
libns3.30.1-olsr-debug.so
libns3.30.1-olsr-test-debug.so
libns3.30.1-point-to-point-debug.so
libns3.30.1-point-to-point-layout-debug.so
libns3.30.1-point-to-point-test-debug.so
libns3.30.1-propagation-debug.so
libns3.30.1-propagation-test-debug.so
libns3.30.1-sixlowpan-debug.so
libns3.30.1-sixlowpan-test-debug.so
libns3.30.1-spectrum-debug.so
libns3.30.1-spectrum-test-debug.so
libns3.30.1-stats-debug.so
libns3.30.1-stats-test-debug.so
libns3.30.1-tap-bridge-debug.so
libns3.30.1-test-debug.so
libns3.30.1-test-test-debug.so
libns3.30.1-topology-read-debug.so
libns3.30.1-topology-read-test-debug.so
libns3.30.1-traffic-control-debug.so
libns3.30.1-traffic-control-test-debug.so
libns3.30.1-uan-debug.so
libns3.30.1-uan-test-debug.so
libns3.30.1-virtual-net-device-debug.so
libns3.30.1-wave-debug.so
libns3.30.1-wave-test-debug.so
libns3.30.1-wifi-debug.so
libns3.30.1-wifi-test-debug.so
libns3.30.1-wimax-debug.so
libns3.30.1-wimax-test-debug.so
)

#Catkin do not export well relative paths to libraries: https://github.com/ros/catkin/issues/535
#link_directories(${NS3_BUILD})
#So it's better to specify the full path to each .so if the the *.so are not in the common directories (see ldconfig)
#message("NS3 modules: ${NS3_LIBS}")
SET(ns3liblist "")
FOREACH(lib ${NS3_LIBS})
SET(path ${NS3_BUILD}/${lib})
LIST(APPEND ns3liblist ${path})
ENDFOREACH()

SET(${PROJECT_NAME}_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/ns-3-dev/build
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/exprtk
    ${${DCCOMMS_UTILS_GIT}_INCLUDE_DIRS}
    ${catkin_INCLUDE_DIRS}
    )

SET(${PROJECT_NAME}_CXX_COMPILE_FLAGS "-fPIC -std=c++14 ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${${PROJECT_NAME}_CXX_COMPILE_FLAGS}")

###################################
## catkin specific configuration ##
###################################

## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS}
  LIBRARIES ${PROJECT_NAME} ${ns3liblist}
  CATKIN_DEPENDS
        roscpp
        rospy
        std_msgs
        actionlib_msgs
        message_runtime
#  DEPENDS system_lib
)

roslaunch_add_file_check(launch)

#SET(S100BRIDGE s100_bridge)
#add_executable(${S100BRIDGE} src/bridges/s100/s100_main.cpp)
#target_include_directories(${S100BRIDGE} PUBLIC ${${PROJECT_NAME}_INCLUDE_DIRS})
#target_link_libraries(${S100BRIDGE} ${DCCOMMS_UTILS_GIT} ${catkin_LIBRARIES})


SET(${PROJECT_NAME}_CPP_FILES
        #src/simulator/ROSCommsDevice.cpp
        #src/simulator/ROSCommsSimulator.cpp
        #src/simulator/AcousticROSCommsDevice.cpp
        #src/simulator/CustomROSCommsDevice.cpp
        #src/simulator/AcousticCommsChannel.cpp
        #src/simulator/CustomCommsChannel.cpp
        #src/simulator/PacketBuilderLoader.cpp
        #src/simulator/NetsimPacket.cpp
        #src/simulator/CommsChannel.cpp
        #src/simulator/NetsimLogFormatter.cpp
        src/simulator/NetsimTime.cpp
        #src/simulator/first_try.cpp
)


#https://stackoverflow.com/questions/15972898/cmake-how-to-run-a-add-custom-command-before-everything-else
#./waf configure --build-profile=debug --enable-examples --enable-tests --enable-sudo
#./waf build
#SET(WAF_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/modules/ns-3-dev/waf)
SET(WAF_EXECUTABLE ./waf)
add_custom_target(
  ns3
  COMMAND ${WAF_EXECUTABLE} configure --build-profile=debug --enable-examples --enable-tests #--enable-sudo
  COMMAND ${WAF_EXECUTABLE} build
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/modules/ns-3-dev/"
)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_CPP_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}  ${ns3liblist} ${DCCOMMS_UTILS_GIT} ${catkin_LIBRARIES})
add_dependencies(${PROJECT_NAME} ns3)

add_executable (ns3_ros1 src/simulator/first_try.cpp)
target_link_libraries(ns3_ros1 ${PROJECT_NAME})


#add_executable (netsim_example1 src/simulator/main/netsim_example1.cpp)
#target_link_libraries(netsim_example1 ${PROJECT_NAME})

#add_executable (netsim_example2 src/simulator/main/netsim_example2.cpp)
#target_link_libraries(netsim_example2 ${PROJECT_NAME})

#add_executable (netsim_example3 src/simulator/main/netsim_example3.cpp)
#target_link_libraries(netsim_example3 ${PROJECT_NAME})

#add_executable (netsim_example4 src/simulator/main/netsim_example4.cpp)
#target_link_libraries(netsim_example4 ${PROJECT_NAME})

#target_compile_options(pruebas PUBLIC "")
#https://cmake.org/cmake/help/v3.4/command/find_library.html
#https://stackoverflow.com/questions/31438916/cmake-cannot-find-library-using-link-directories

#add_executable (ns3-testing testing/simulator/ns3-testing.cpp)
#target_link_libraries(ns3-testing ${PROJECT_NAME})
#target_include_directories(ns3-testing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#add_executable (ns3-as-testing testing/simulator/ns3-as-testing.cpp)
#target_link_libraries(ns3-as-testing ${PROJECT_NAME})
#target_include_directories(ns3-as-testing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#add_executable (ns3-as-VBF testing/simulator/ns3-as-VBF.cpp)
#target_link_libraries(ns3-as-VBF ${PROJECT_NAME})
#target_include_directories(ns3-as-VBF PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
###############################################
############ END netsim BUILD #################
###############################################

SET(NS3_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/ns-3-dev/build/ns3)
set(EXPRTK_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/modules/exprtk/exprtk.hpp)

#install(DIRECTORY 
#  src/include/${PROJECT_NAME}
#  ${NS3_HEADERS_DIR}
#  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
#)
#install(FILES
#  ${EXPRTK_HEADERS}
#  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
#)
#install(FILES
# ${ns3liblist}
# DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#)

#install(TARGETS ${PROJECT_NAME}
#  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#)
message("End ${PROJECT_NAME}")
