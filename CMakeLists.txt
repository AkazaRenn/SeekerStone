set(CMAKE_CXX_STANDARD 26)

cmake_minimum_required(VERSION 3.16)

project(SeekerStone VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(SDL3 REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

set(MAIN_SOURCE "src/Main.cpp")

qt_add_executable(appSeekerStone
    ${MAIN_SOURCE}
)

file(GLOB_RECURSE QML_FILES RELATIVE ${CMAKE_SOURCE_DIR} "qml/*.qml")
file(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.hpp" "src/*.cpp")
list(FILTER SOURCE_FILES EXCLUDE REGEX ${MAIN_SOURCE})

qt_add_qml_module(appSeekerStone
    URI SeekerStone
    VERSION 1.0
    QML_FILES ${QML_FILES}
    SOURCES ${SOURCE_FILES}
)

target_link_libraries(appSeekerStone
    PRIVATE
    Qt6::Quick
    SDL3::SDL3
)

include(GNUInstallDirs)
install(TARGETS appSeekerStone
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
