message($$PWD)

SOURCES += \
	../../src/tools.cpp \
    ../../src/toolsscriptclass.cpp \
    ../../src/http.cpp \
    ../../src/httpscriptclass.cpp \
    ../../src/searchvideos.cpp \
    ../../src/searchvideosscriptclass.cpp \
    ../../src/videoinformation.cpp \
    ../../src/options.cpp \
    ../../src/videoitem.cpp \
	../../src/languages.cpp \
	../../src/serviceskeychain.cpp \
	../../src/forms/loginpromptimpl.cpp

HEADERS += \
	xvst-classes.h \
	../../src/tools.h \
    ../../src/toolsscriptclass.h \
    ../../src/http.h \
    ../../src/httpscriptclass.h \
    ../../src/searchvideos.h \
    ../../src/searchvideosscriptclass.h \
    ../../src/videoinformation.h \
    ../../src/options.h \
    ../../src/videoitem.h \
	../../src/languages.h \
	../../src/serviceskeychain.h \
	../../src/forms/loginpromptimpl.h

FORMS += \
	../../ui/loginprompt.ui
