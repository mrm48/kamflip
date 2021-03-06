cmake_minimum_required(VERSION 3.0)

project(kamflip)

set(QT_MIN_VERSION "5.11.0")
set(KF5_MIN_VERSION "5.68.0")

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS 
    Core    # QCommandLineParser, QStringLiteral
    Widgets # QApplication, QAction
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons      # KAboutData
    I18n            # KLocalizedString
    XmlGui          # KXmlGuiWindow, KActionCollection
    ConfigWidgets   # KStandardActions
    WidgetsAddons   # KMessageBox
)
    

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
    
set(kamflip_SRCS main.cpp mainwindow.cpp)

add_executable(kamflip ${kamflip_SRCS})

target_link_libraries(kamflip
    Qt5::Widgets
    KF5::CoreAddons
    KF5::I18n
    KF5::XmlGui
    KF5::ConfigWidgets
    KF5::WidgetsAddons
)

install(TARGETS kamflip ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kamflipui.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/kamflip)
