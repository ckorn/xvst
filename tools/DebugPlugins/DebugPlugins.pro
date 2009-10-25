QT += network \
    script \
    scripttools
DEFINES += xVST_DEBUG_PLUGINS_ON
TARGET = DebugPlugins
TEMPLATE = app
DESTDIR = bin
UI_DIR = build/ui
MOC_DIR = build/moc
RCC_DIR = build/rcc
OBJECTS_DIR += build/o/
SOURCES += src/main.cpp \
    src/forms/mainwindow.cpp \
    src/programsettings.cpp
HEADERS += src/forms/mainwindow.h \
    src/programsettings.h
FORMS += ui/mainwindow.ui
macx {
	QMAKE_INFO_PLIST = resources/Info.plist
	ICON += resources/icons/MacOSX.icns
}
win32 {
	RC_FILE += resources/DebugPlugins.rc
}

# include the common xVST classes
include(../Common/xvst-classes.pri)
