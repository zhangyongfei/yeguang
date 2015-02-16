CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

AddSrc(tcmdSrc
${CMAKE_CURRENT_LIST_DIR}/TestCmd.cpp)

AddLibs(cmdlibs
yeguang
log4cplus
#polarssl
)

AppPrj(testcmd tcmdSrc cmdlibs)
