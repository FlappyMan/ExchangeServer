cmake_minimum_required( VERSION 3.10.2 )

PROJECT(UBaseLib)
MESSAGE(STATUS "Project: UBaseLib") 

set(LIB_NAME UBaseLib)
SET(OPENSOUR_LIB_ROOT "/usr/local/UkexServer/lib")   

set( CMAKE_CXX_FLAGS "-std=c++11 -g" )
SET(CMAKE_SOURCE_DIR .) 


include_directories(
${OPENSOUR_LIB_ROOT}/jsoncpp/include
${OPENSOUR_LIB_ROOT}/openssl/include
${OPENSOUR_LIB_ROOT}/spdlog/include
${OPENSOUR_LIB_ROOT}/mysqlinc
${OPENSOUR_LIB_ROOT}/http-parser
)


aux_source_directory(. SRC_LIST)
add_library(${LIB_NAME} STATIC ${SRC_LIST}) 
#target_include_directories(${LIB_NAME} PUBLIC ${OPENSOUR_LIB_ROOT}/jsoncpp/include)
