CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Solution(yeguang ON)

include(./command/CMakeLists.txt)
include(./config/CMakeLists.txt)
include(./daemon/CMakeLists.txt)
include(./logger/CMakeLists.txt)
include(./ipc/CMakeLists.txt)
include(./test/CMakeLists.txt)
include(./protocol/CMakeLists.txt)

StaticLib(yeguang YeGuangSrc)

