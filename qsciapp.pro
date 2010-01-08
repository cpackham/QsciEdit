TEMPLATE = app
TARGET = qsciedit
CONFIG += qt
QT += xml

unix|macx { include(config.unix) }
win32 { include(config.win32) }

SOURCES = qsciapp.cpp \
	qticonloader.cpp \
	lexersel.cpp \
	finddialog.cpp \
	actions.cpp \
	editorsettings.cpp \
	main.cpp
HEADERS = qsciapp.h \
	qticonloader.h \
	lexersel.h \
	finddialog.h \
	actions.h \
	globals.h \
	editorsettings.h

RESOURCES = qsciapp.qrc

INCLUDEPATH += $${QSCI_HEADER_PATH}

LIBS += -L$${QSCI_LIB_PATH} \
	-lqscintilla2

target.path = $${INSTALL_PATH}/bin
INSTALLS += target

message ("Qsci library path: $${QSCI_LIB_PATH}")
message ("Qsci header path: $${QSCI_HEADER_PATH}")
message ("Install path: $${INSTALL_PATH}")

