cmake_minimum_required( VERSION 3.10.2 )

PROJECT(UProtocol)
SET(LIB_NAME UProtocol)
MESSAGE(STATUS "Creating UProtocol Static Lib...")


SET(OPENSOUR_LIB_ROOT "/usr/local/UkexServer/lib")
SET(UBASE_LIB_ROOT "../BaseLib/")

SET(CMAKE_SOURCE_DIR .)
SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS "-g -std=c++11")

include_directories(
    ${OPENSOUR_LIB_ROOT}/jsoncpp/include
    ${OPENSOUR_LIB_ROOT}/mysqlinc
    ${OPENSOUR_LIB_ROOT}/libuv/include
    ${OPENSOUR_LIB_ROOT}/http-parser
    ${OPENSOUR_LIB_ROOT}/protobuf/src
	${UBASE_LIB_ROOT}
	${UPROTOCOL_LIB_ROOT}
    )

aux_source_directory(. SRC_LIST)
aux_source_directory(./proto/ SRC_LIST)
add_library(${LIB_NAME} STATIC ${SRC_LIST})

target_link_libraries(${LIB_NAME} protobuf jsoncpp)

