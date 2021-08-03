cmake_minimum_required(VERSION 3.13)
project(arftracksat)
set(CMAKE_CXX_STANDARD 17)

include_directories(sgp4/ sgp4/sgp4/services sgp4/sgp4/wrappers)
file(GLOB SGP4_SRC sgp4/sgp4/services/*.c sgp4/sgp4/wrappers/*.c)
file(GLOB SGP4_LIB sgp4/lib/*.so)
file(GLOB SGP4_LIB_5 sgp4/lib/*.5)
file(GLOB SGP4_LIC sgp4/lib/SGP4_Open_License.txt)
file(GLOB CONFJSON ../config.json)

#message(STATUS "${SGP4_SRC}")

set(SOURCE_FILES main.cpp sat.cpp ${SGP4_SRC})
set(CMAKE_DEFAULT_PROJECT_FILE main.cpp)

add_executable(arftracksat ${SOURCE_FILES} ${SGP4_SRC})

target_link_libraries(arftracksat curl curlpp dl ${SGP4_LIB} ${SGP4_LIB_5})

install(TARGETS arftracksat RUNTIME DESTINATION bin)
install(FILES ${SGP4_LIB} ${SGP4_LIB_5} DESTINATION lib)
#install(FILES ${SGP4_LIB} ${SGP4_LIB_5} DESTINATION /etc/arftracksat/sgp4lib/)
install(FILES ${SGP4_LIC} DESTINATION share/doc/arftracksat/)
install(FILES ${CONFJSON} DESTINATION etc/arftracksat/)
