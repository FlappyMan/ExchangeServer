cmake_minimum_required( VERSION 3.10.2 )

PROJECT(Test)
set(APP_NAME UserCenter)

#find_package(OpenSSL REQUIRED)
#find_package(Threads REQUIRED)

SET(OPENSOUR_LIB_ROOT "/usr/local/UkexServer/lib")   
SET(UBASE_LIB_ROOT "../../BaseLib/")   
SET(UPROTOCOL_LIB_ROOT "../../Protocol/")   
SET(DTO_DIR "./Dto/")
SET(MODEL_DIR "./Model/")
SET(BUSINESS_DIR "./Business/")
SET(BASE_CONFIG_DIR "./BaseConfig/")
SET(MANAGER_DIR "./Manager/")


SET(CMAKE_SOURCE_DIR .)

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS "-g -std=c++11")

include_directories(
    ${OPENSOUR_LIB_ROOT}/jsoncpp/include
    ${OPENSOUR_LIB_ROOT}/openssl/include
    ${OPENSOUR_LIB_ROOT}/mysqlinc
    ${OPENSOUR_LIB_ROOT}/spdlog/include
    ${OPENSOUR_LIB_ROOT}/libuv/include
    ${OPENSOUR_LIB_ROOT}/http-parser
    ${OPENSOUR_LIB_ROOT}/protobuf/src
	${UBASE_LIB_ROOT}
	${UPROTOCOL_LIB_ROOT}
    ${DTO_DIR}
    ${MODEL_DIR}
    ${BUSINESS_DIR}
    ${BASE_CONFIG_DIR}
    ${MANAGER_DIR}
    )

link_directories(
    ${OPENSOUR_LIB_ROOT}/jsoncpp/
    ${OPENSOUR_LIB_ROOT}/openssl/
    ${OPENSOUR_LIB_ROOT}/mysqllib
    ${OPENSOUR_LIB_ROOT}/spdlog
    ${OPENSOUR_LIB_ROOT}/libuv/
    ${OPENSOUR_LIB_ROOT}/http-parser
    ${OPENSOUR_LIB_ROOT}/protobuf/src/.libs
    ${UBASE_LIB_ROOT}
    ${UPROTOCOL_LIB_ROOT}
    ${DTO_DIR}
    ${MODEL_DIR}
    ${BUSINESS_DIR}
    ${BASE_CONFIG_DIR}
    ${MANAGER_DIR}
    )

aux_source_directory(. SRC_LIST)
aux_source_directory(./Dto/ SRC_LIST)
aux_source_directory(./Manager/ SRC_LIST)
aux_source_directory(./Model/ SRC_LIST)
aux_source_directory(./Business/ SRC_LIST)
aux_source_directory(./BaseConfig/ SRC_LIST)

add_executable(${APP_NAME} ${SRC_LIST})
target_link_libraries(${APP_NAME} UProtocol uv_a UBaseLib jsoncpp crypto pthread protobuf http_parser mysqlclient)
