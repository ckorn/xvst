CONFIG += qt warn_on release $$[build_mode]
QT = core gui network
TEMPLATE = app
DESTDIR = bin
UI_DIR = build/ui
MOC_DIR = build/moc
RCC_DIR += build/rcc
FORMS = ui/addvideo.ui \
 ui/checkupdates.ui \
 ui/completedpopup.ui \
 ui/customalphablend.ui \
 ui/downloadlog.ui \
 ui/dragdrop.ui \
 ui/information.ui \
 ui/mainform.ui \
 ui/options.ui \
 ui/updatecenter.ui \
 ui/infoview.ui \
 ui/bugreport.ui \
 ui/credits.ui
HEADERS = src/forms/addvideoimpl.h \
 src/forms/checkupdatesimpl.h \
 src/forms/completedpopupimpl.h \
 src/forms/customalphablendimpl.h \
 src/forms/downloadlogimpl.h \
 src/forms/dragdropimpl.h \
 src/forms/informationimpl.h \
 src/forms/mainformimpl.h \
 src/forms/optionsimpl.h \
 src/forms/updatecenterimpl.h \
 src/forms/infoviewimpl.h \
 src/forms/bugreportimpl.h \
 src/forms/creditsimpl.h \
 src/pakunpak/pakunpak.h \
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
 src/videolistcontroller.h \
 src/trackerreport.h
SOURCES = src/forms/addvideoimpl.cpp \
 src/forms/checkupdatesimpl.cpp \
 src/forms/completedpopupimpl.cpp \
 src/forms/customalphablendimpl.cpp \
 src/forms/downloadlogimpl.cpp \
 src/forms/dragdropimpl.cpp \
 src/forms/informationimpl.cpp \
 src/forms/mainformimpl.cpp \
 src/forms/optionsimpl.cpp \
 src/forms/updatecenterimpl.cpp \
 src/forms/infoviewimpl.cpp \
 src/forms/bugreportimpl.cpp \
 src/forms/creditsimpl.cpp \
 src/pakunpak/pakunpak.cpp \
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
 src/videolistcontroller.cpp \
 src/trackerreport.cpp
TRANSLATIONS = resources/translations/xVST_ca.ts resources/translations/xVST_es.ts resources/translations/xVST_pl.ts resources/translations/xVST_it.ts
RESOURCES = resources/resources.qrc
macx {
 ICON +=  resources/icons/MacOSX.icns
 OBJECTS_DIR +=  build/o/mac
}
unix {
 OBJECTS_DIR +=  build/o/unix
 TARGET =  xvst
}
win32 {
 RC_FILE +=  resources/xVST.rc
 OBJECTS_DIR +=  build/o/win32
}
CONFIG(static_build) {
 DEFINES +=  STATIC_BUILD
}
