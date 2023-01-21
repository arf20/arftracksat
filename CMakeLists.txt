cmake_minimum_required(VERSION 3.16.0)
project(arftracksat VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories(common/sgdp4/ common/sgdp4/sigutils/ common/sgdp4/sigutils/util/)

file (GLOB SGPD4_SRC
    "common/sgdp4/sigutils/*.c"
    "common/sgdp4/sigutils/*.c"
    "common/sgdp4/*.c"
)
file (GLOB CORE_SRC "core/*.cpp")
file (GLOB REPRESENTATION_SRC "representation/*.cpp")
file (GLOB LEGACY_UI_SRC "representation/legacy_gl_ui/*.cpp")
file (GLOB MAIN_SRC "src/*.cpp")
file (GLOB CONFJSON "config.json")
file (GLOB ASSETS "assets/*")

set(SOURCE_FILES ${SGPD4_SRC} ${COMMON_SRC} ${COMMON_SRC} ${CORE_SRC} ${LEGACY_UI_SRC} ${REPRESENTATION_SRC} ${MAIN_SRC})
set(CMAKE_DEFAULT_PROJECT_FILE main.cpp)

add_executable(arftracksat ${SOURCE_FILES})

target_link_libraries(arftracksat pthread curl curlpp dl GL GLU glut)

install(TARGETS arftracksat RUNTIME DESTINATION bin)
install(FILES ${CONFJSON} DESTINATION etc/arftracksat/)
install(FILES ${ASSETS} DESTINATION share/arftracksat/)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
