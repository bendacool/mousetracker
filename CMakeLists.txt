cmake_minimum_required(VERSION 3.16)

project(MouseTracker VERSION 0.1 LANGUAGES CXX)

# Qt için gerekli otomatik işlemleri etkinleştir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt5 veya Qt6 Widgets bileşenini bul
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# easylogging++ için include dizini ekle
include_directories(${CMAKE_SOURCE_DIR}/libs/easyloggingpp/src)

# Kaynak ve başlık dosyaları
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    settingsdialog.cpp
    settingsdialog.h
    visualizationwindow.cpp
    visualizationwindow.h
)

# Yürütülebilir dosyayı oluştur
add_executable(MouseTracker ${PROJECT_SOURCES}
    settingsdialog.ui
    settings.h
    settings.cpp)

# Qt kütüphanelerini bağla
target_link_libraries(MouseTracker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# easylogging++ kaynağını ekle
target_sources(MouseTracker PRIVATE ${CMAKE_SOURCE_DIR}/libs/easyloggingpp/src/easylogging++.cc)

# MacOS ve Windows için yapılandırma
set_target_properties(MouseTracker PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Kurulum dizinlerini tanımla
include(GNUInstallDirs)
install(TARGETS MouseTracker
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
