cmake_minimum_required(VERSION 3.0.0)
#project(arftracksat)
set(CMAKE_CXX_STANDARD 17)

include_directories(sgdp4/)
set(SGPD4_SRC
    "sgdp4/sigutils/util.c"
    "sgdp4/sigutils/log.c"
    "sgdp4/sgdp4.c"
    "sgdp4/tle.c"
    "sgdp4/predict.c"
    "sgdp4/deep.c"
    "sgdp4/coord.c")

set(CONFJSON ../config.json)
set(MAPFILE ../map.json)

set(SOURCE_FILES ${SGPD4_SRC} sat.cpp graphics.cpp main.cpp)
set(CMAKE_DEFAULT_PROJECT_FILE main.cpp)

add_executable(arftracksat ${SOURCE_FILES})

target_link_libraries(arftracksat pthread curl curlpp dl GL GLU glut)

install(TARGETS arftracksat RUNTIME DESTINATION bin)
install(FILES ${CONFJSON} DESTINATION etc/arftracksat/)
install(FILES ${MAPFILE} DESTINATION share/arftracksat/)
