cmake_minimum_required(VERSION 3.16)

project(TLHoN_IC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)# Core LinguistTools)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        file.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TLHoN_IC
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resource.qrc
        stylefromfile.cpp stylefromfile.h
        jsonparser.h jsonparser.cpp
        tab_stats.cpp tab_stats.h tab_stats.ui
        tab_equipment.h tab_equipment.cpp tab_equipment.ui
        tab_settings.h tab_settings.cpp tab_settings.ui
        tab_about.h tab_about.cpp tab_about.ui

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TLHoN_IC APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TLHoN_IC SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TLHoN_IC
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TLHoN_IC PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TLHoN_IC)
endif()
set_target_properties(TLHoN_IC PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TLHoN_IC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TLHoN_IC)
endif()

# add_custom_target(lupdate
#     COMMAND ${Qt6_LUPDATE_EXECUTABLE} ${CMAKE_SOURCE_DIR} -ts ${CMAKE_SOURCE_DIR}/translations/TLHoN_IC_language.ts
#     COMMENT "Updating translation files"
# )
