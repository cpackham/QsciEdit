QSCI_HEADER_PATH = /usr/include/
QSCI_LIB_PATH = /usr/lib

TEMPLATE = app
TARGET = qsciedit
CONFIG += qt
QT += xml

SOURCES = qsciapp.cpp \
	qticonloader.cpp \
	lexersel.cpp \
	finddialog.cpp \
	actions.cpp \
	main.cpp
HEADERS = qsciapp.h \
	qticonloader.h \
	lexersel.h \
	finddialog.h \
	actions.h \
	globals.h

RESOURCES = qsciapp.qrc

INCLUDEPATH += $${QSCI_HEADER_PATH}/Qsci

LIBS += -L$${QSCI_LIB_PATH}/Qsci \
	-lqscintilla2
