CONFIG += qt warn_on console debug_and_release
QT = core gui network
DESTDIR = bin
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR += build/rcc
TEMPLATE = app
FORMS = ui/addvideo.ui \
 ui/completedpopup.ui \
 ui/customalphablend.ui \
 ui/downloadlog.ui \
 ui/dragdrop.ui \
 ui/information.ui \
 ui/mainform.ui \
 ui/options.ui
SOURCES = src/forms/addvideoimpl.cpp \
 src/forms/completedpopupimpl.cpp \
 src/forms/customalphablendimpl.cpp \
 src/forms/dragdropimpl.cpp \
 src/forms/informationimpl.cpp \
 src/forms/mainformimpl.cpp \
 src/forms/optionsimpl.cpp \
 src/forms/downloadlogimpl.cpp \
 src/main.cpp \
 src/options.cpp \
 src/progressbardelegate.cpp \
 src/tools.cpp \
 src/videolistcontroller.cpp \
 src/videoinformation.cpp \
 src/videodownload.cpp \
 src/http.cpp \
 src/videoconvert.cpp \
 src/videoitem.cpp \
 src/sessionmanager.cpp \
 src/languages.cpp \
 src/updates.cpp
HEADERS = src/forms/addvideoimpl.h \
 src/forms/completedpopupimpl.h \
 src/forms/customalphablendimpl.h \
 src/forms/dragdropimpl.h \
 src/forms/informationimpl.h \
 src/forms/mainformimpl.h \
 src/forms/optionsimpl.h \
 src/forms/downloadlogimpl.h \
 src/options.h \
 src/progressbardelegate.h \
 src/tools.h \
 src/videolistcontroller.h \
 src/videoinformation.h \
 src/videodownload.h \
 src/http.h \
 src/videoconvert.h \
 src/videoitem.h \
 src/programversion.h \
 src/sessionmanager.h \
 src/languages.h \
 src/updates.h
RESOURCES += resources/resources.qrc
RC_FILE += resources/xVST.rc
TRANSLATIONS += resources/translations/xVST_ca.ts resources/translations/xVST_es.ts
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
