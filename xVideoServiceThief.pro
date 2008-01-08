CONFIG += console debug_and_release qt warn_on
RCC_DIR += build/rcc
RC_FILE += resources/xVST.rc

TEMPLATE = app
DESTDIR = bin
MOC_DIR = build/moc
QT = core gui network
UI_DIR = build/ui

FORMS = ui/addvideo.ui \
 ui/completedpopup.ui \
 ui/customalphablend.ui \
 ui/downloadlog.ui \
 ui/dragdrop.ui \
 ui/information.ui \
 ui/mainform.ui \
 ui/options.ui

HEADERS = src/forms/addvideoimpl.h \
 src/forms/completedpopupimpl.h \
 src/forms/customalphablendimpl.h \
 src/forms/downloadlogimpl.h \
 src/forms/dragdropimpl.h \
 src/forms/informationimpl.h \
 src/forms/mainformimpl.h \
 src/forms/optionsimpl.h \
 src/http.h \
 src/languages.h \
 src/options.h \
 src/programversion.h \
 src/progressbardelegate.h \
 src/sessionmanager.h \
 src/tools.h \
 src/updates.h \
 src/videoconvert.h \
 src/videodownload.h \
 src/videoinformation.h \
 src/videoitem.h \
 src/videolistcontroller.h
 
SOURCES = src/forms/addvideoimpl.cpp \
 src/forms/completedpopupimpl.cpp \
 src/forms/customalphablendimpl.cpp \
 src/forms/downloadlogimpl.cpp \
 src/forms/dragdropimpl.cpp \
 src/forms/informationimpl.cpp \
 src/forms/mainformimpl.cpp \
 src/forms/optionsimpl.cpp \
 src/http.cpp \
 src/languages.cpp \
 src/main.cpp \
 src/options.cpp \
 src/progressbardelegate.cpp \
 src/sessionmanager.cpp \
 src/tools.cpp \
 src/updates.cpp \
 src/videoconvert.cpp \
 src/videodownload.cpp \
 src/videoinformation.cpp \
 src/videoitem.cpp \
 src/videolistcontroller.cpp
 
TRANSLATIONS += resources/translations/xVST_ca.ts resources/translations/xVST_es.ts

RESOURCES += resources/resources.qrc

macx {
 ICON +=  resources/icons/MacOSX.icns
 OBJECTS_DIR +=  build/o/mac
}

unix {
 OBJECTS_DIR +=  build/o/unix
}

win32 {
 OBJECTS_DIR +=  build/o/win32
}
