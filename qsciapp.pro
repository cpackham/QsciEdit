QSCI_HEADER_PATH = /usr/include/
QSCI_LIB_PATH = /usr/lib

TEMPLATE = app
CONFIG += qt
QT += xml

SOURCES = qsciapp.cpp \
	qticonloader.cpp \
	lexersel.cpp \
	main.cpp
HEADERS = qsciapp.h \
	qticonloader.h \
	lexersel.h

RESOURCES = qsciapp.qrc

INCLUDEPATH += $${QSCI_HEADER_PATH}/Qsci

LIBS += -L$${QSCI_LIB_PATH}/Qsci \
	-lqscintilla2
