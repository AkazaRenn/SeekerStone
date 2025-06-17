set(CMAKE_CXX_STANDARD 23)
cmake_minimum_required(VERSION 3.16)
project(SeekerStone VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB cmake_files "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.cmake")
foreach(file ${cmake_files})
    include(${file})
endforeach()

pascal_to_camel(${PROJECT_NAME} target_name)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(SDL3 REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(${target_name}
    "main.cpp"
)

target_compile_definitions(${target_name} PRIVATE APP_NAME="${PROJECT_NAME}")

file(GLOB_RECURSE QML_FILES RELATIVE ${CMAKE_SOURCE_DIR} "qml/*.qml")
file(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.hpp" "src/*.cpp")

qt_add_qml_module(${target_name}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES ${QML_FILES}
    SOURCES ${SOURCE_FILES}
)

target_link_libraries(${target_name}
    PRIVATE
    Qt6::Quick
    SDL3::SDL3
)

include(GNUInstallDirs)
install(TARGETS ${target_name}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
