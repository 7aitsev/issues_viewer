cmake_minimum_required(VERSION 3.16)

project(issues_viewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Network)


qt_add_executable(appissues_viewer
    main.cpp
)

qt_add_qml_module(appissues_viewer
    URI issues_viewer
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES issues_viewer_presenter.h issues_viewer_presenter.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appissues_viewer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appissues_viewer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appissues_viewer
    PRIVATE Qt6::Quick Qt6::QuickControls2 Qt6::Network
)

include(GNUInstallDirs)
install(TARGETS appissues_viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
