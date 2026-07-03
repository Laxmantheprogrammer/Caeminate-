cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(maybe_caeminate)
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB MACRO_FILES
    ${PROJECT_SOURCE_DIR}/macros/*.mac
)
file(COPY ${MACRO_FILES}
     DESTINATION ${PROJECT_BINARY_DIR}/macros)

add_executable(maybe_caeminate
    maybe_caeminate.cc
    ${sources}
)
target_link_libraries(maybe_caeminate ${Geant4_LIBRARIES})