QSCI_HEADER_PATH = /usr/include/
QSCI_LIB_PATH = /usr/lib

TEMPLATE = app
CONFIG += qt
QT += xml

SOURCES = qsciapp.cpp \
	main.cpp
HEADERS = qsciapp.h

INCLUDEPATH += $${QSCI_HEADER_PATH}/Qsci

LIBS += -L$${QSCI_LIB_PATH}/Qsci \
	-lqscintilla2
