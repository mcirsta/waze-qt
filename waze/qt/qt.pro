#-------------------------------------------------
#
# Project created by QtCreator 2011-10-26T18:40:46
#
#-------------------------------------------------

QT       += core gui sql declarative network phonon

TARGET = wazeqt
TEMPLATE = lib
CONFIG += staticlib

maemo5 {
    CONFIG += qdbus
}

MOBILITY = location systeminfo multimedia contacts sensors

DEFINES += USE_QT TOUCH_SCREEN SSD FREEMAP_IL PLAY_CLICK LOCALE_SAFE PORT_RELEASE=\'\"v0.0.12\"\'

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)

INCLUDEPATH += \
    .. \
    ../address_search \
    ../editor \
    ../navigate \
    ../Realtime \
    ../ssd \
    ../websvc_trans

SOURCES += \
    roadmap_main.cc \
    roadmap_fileselection.cc \
    roadmap_canvas.cc \
    qt_progress.cc \
    qt_main.cc \
    qt_dialog.cc \
    qt_canvas.cc \
    roadmap_device.cc \
    roadmap_tile_storage_qtsql.cc \
    roadmap_native_keyboard.cc \
    roadmap_gpsqtm.cc \
    qt/qt_gpsaccessor.cc \
    qt/roadmap_sound.cc \
    qt/qt_sound.cc \
    qt/qt_contactslistmodel.cc \
    qt/roadmap_editbox.cc \
    qt/qt_keyboard_dialog.cpp \
    qt/tts_db_qtsqlite.cc \
    qt/roadmap_qtbrowser.cc \
    qt/roadmap_net.cc \
    qt/qt_network.cc \
    qt/roadmap_spawn.cc \
    qt/roadmap_path.cc \
    qt/roadmap_file.cc \
    qt/roadmap_config.cc \
    qt/qt_config.cc \
    qt/qt_contacts.cpp \
    qt/qt_device.cpp \
    qt/RealtimeNet.cc \
    qt/qt_webaccessor.cc \
    qt/roadmap_httpcopy_async.cc \
    qt/qt_wazesocket.cpp \
    qt/roadmap_speedometer.cc \
    qt/qt_datamodels.cc \
    qt/navigate_bar.cc

HEADERS += \
    qt_progress.h \
    qt_main.h \
    qt_dialog.h \
    qt_canvas.h \
    roadmap_gpsqtm.h \
    qt/qt_gpsaccessor.h \
    qt/qt_sound.h \
    qt/roadmap_qtmain.h \
    qt/qt_contactslistmodel.h \
    qt/qt_keyboard_dialog.h \
    qt/roadmap_qtbrowser.h \
    qt/qt_network.h \
    qt/qt_global.h \
    qt/qt_config.h \
    qt/qt_contacts.h \
    qt/qt_device.h \
    qt/qt_webaccessor.h \
    qt/qt_wazesocket.h \
    qt/qt_datamodels.h






















