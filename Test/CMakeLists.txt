cmake_minimum_required( VERSION 3.10.2 )

PROJECT(Test)

#find_package(OpenSSL REQUIRED)
#find_package(Threads REQUIRED)

SET(OPENSOUR_LIB_ROOT "/usr/local/UkexServer/lib")   
SET(UBASE_LIB_ROOT "../BaseLib/")   
SET(UPROTOCOL_LIB_ROOT "../Protocol/")   

SET(CMAKE_SOURCE_DIR .)


set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_FLAGS "-std=c++11 -g")


include_directories(
	${OPENSOUR_LIB_ROOT}/jsoncpp/include
	${OPENSOUR_LIB_ROOT}/openssl/include
	${OPENSOUR_LIB_ROOT}/mysqlinc
	${OPENSOUR_LIB_ROOT}/libuv/include
	${OPENSOUR_LIB_ROOT}/http-parser
	${OPENSOUR_LIB_ROOT}/protobuf/src
	${UBASE_LIB_ROOT}
	${UPROTOCOL_LIB_ROOT}
)

link_directories(
	${OPENSOUR_LIB_ROOT}/jsoncpp/
    ${OPENSOUR_LIB_ROOT}/openssl/
	${OPENSOUR_LIB_ROOT}/mysqllib
	${OPENSOUR_LIB_ROOT}/libuv/
	${OPENSOUR_LIB_ROOT}/http-parser
	${OPENSOUR_LIB_ROOT}/protobuf/src/.libs
    ${UBASE_LIB_ROOT}
	${UPROTOCOL_LIB_ROOT}
)


aux_source_directory(. SRC_LIST)

add_executable(grant grant.cpp)
target_link_libraries(grant UProtocol UBaseLib uv_a jsoncpp crypto pthread mysqlclient protobuf)

# add_executable(gong gong.cpp)
# target_link_libraries(gong UProtocol UBaseLib uv_a jsoncpp crypto pthread mysqlclient protobuf)

# add_executable(uv uv.cpp)
# target_link_libraries(uv UProtocol UBaseLib uv_a jsoncpp crypto pthread mysqlclient protobuf http_parser)

add_executable(http http.cpp)
target_link_libraries(http UProtocol UBaseLib uv_a jsoncpp crypto pthread mysqlclient protobuf http_parser)

add_executable(wuyoupeng wuyoupeng.cpp)
target_link_libraries(wuyoupeng UProtocol UBaseLib uv_a jsoncpp crypto pthread mysqlclient protobuf http_parser)

add_executable(mysqltest MySqlTest.cpp)
target_link_libraries(mysqltest UProtocol UBaseLib uv_a jsoncpp crypto pthread mysqlclient protobuf http_parser)
