cmake_minimum_required(VERSION 3.16)

project(SeekerStone VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appSeekerStone
    main.cpp
)

qt_add_qml_module(appSeekerStone
    URI SeekerStone
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES idleDetector.cpp idleDetector.h
)

target_link_libraries(appSeekerStone
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appSeekerStone
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
