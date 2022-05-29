cmake_minimum_required(VERSION 3.0.0)
#project(arftracksat)
set(CMAKE_CXX_STANDARD 17)

include_directories(../common/sgdp4/)
set(SGPD4_SRC
    "../common/sgdp4/sigutils/util.c"
    "../common/sgdp4/sigutils/log.c"
    "../common/sgdp4/sgdp4.c"
    "../common/sgdp4/tle.c"
    "../common/sgdp4/predict.c"
    "../common/sgdp4/deep.c"
    "../common/sgdp4/coord.c")

include_directories(../common/)
set(
    COMMON_SRC
    "../common/sat.cpp"
    "../common/graphics.cpp"
)

set(MAIN_SRC
    "util.cpp"
    "main.cpp")

set(CONFJSON ../config.json)
set(MAPFILE ../map.json)

set(SOURCE_FILES ${SGPD4_SRC} ${COMMON_SRC} ${MAIN_SRC})
set(CMAKE_DEFAULT_PROJECT_FILE main.cpp)

add_executable(arftracksat ${SOURCE_FILES})

target_link_libraries(arftracksat pthread curl curlpp dl GL GLU glut)

install(TARGETS arftracksat RUNTIME DESTINATION bin)
install(FILES ${CONFJSON} DESTINATION etc/arftracksat/)
install(FILES ${MAPFILE} DESTINATION share/arftracksat/)
