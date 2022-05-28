cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

include_directories(x86_64-linux-gnu/qt5/SigDigger/)

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
    "SatViewerWidgetFactory.cpp")

set(CONFJSON ../config.json)
set(MAPFILE ../map.json)

set(SOURCE_FILES ${SGPD4_SRC} ${COMMON_SRC} ${MAIN_SRC})
set(CMAKE_DEFAULT_PROJECT_FILE main.cpp)

add_library(satviewer_plugin ${SOURCE_FILES})

target_link_libraries(satviewer_plugin pthread curl curlpp dl GL GLU glut)