CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

AddSrc(YeGuangSrc
${CMAKE_CURRENT_LIST_DIR}/PidFile.cpp
${CMAKE_CURRENT_LIST_DIR}/PidFile.h
${CMAKE_CURRENT_LIST_DIR}/Daemon.cpp
${CMAKE_CURRENT_LIST_DIR}/Daemon.h
)

AddHDir(${CMAKE_CURRENT_LIST_DIR})