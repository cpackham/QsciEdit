TEMPLATE = app
TARGET = qsciedit
CONFIG += qt debug_and_release
QT += xml

unix|macx { include(config.unix) }
win32 { include(config.win32) }

CONFIG(debug, debug|release) {
	DEFINES += DEBUG
}

SOURCES = qsciapp.cpp \
	qticonloader.cpp \
	lexersel.cpp \
	searchdialog.cpp \
	actions.cpp \
	editorsettings.cpp \
	asciidoclexer.cpp \
	miblexer.cpp \
	qeaction.cpp \
	main.cpp
HEADERS = qsciapp.h \
	qticonloader.h \
	lexersel.h \
	searchdialog.h \
	actions.h \
	globals.h \
	editorsettings.h \
	version.h \
	asciidoclexer.h \
	miblexer.h \
	qeaction.h

RESOURCES = qsciapp.qrc

INCLUDEPATH += $${QSCI_HEADER_PATH}

LIBS += -L$${QSCI_LIB_PATH} \
	-lqscintilla2

target.path = $${INSTALL_PATH}/bin
INSTALLS += target

include (version.mk)

message ("Qsci library path: $${QSCI_LIB_PATH}")
message ("Qsci header path: $${QSCI_HEADER_PATH}")
message ("Install path: $${INSTALL_PATH}")

