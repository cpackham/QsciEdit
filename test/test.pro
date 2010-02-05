TEMPLATE = app
TARGET = test
CONFIG += qt debug
CONFIG += qtestlib
QT += testlib

QSCI_HEADER_PATH = /usr/include/Qsci
QSCI_LIB_PATH = /usr/lib/Qsci
 
SOURCES = ../lexersel.cpp ../miblexer.cpp ../asciidoclexer.cpp testlexersel.cpp
HEADERS = ../lexersel.h ../miblexer.h ../asciidoclexer.h
 
INCLUDEPATH += $${QSCI_HEADER_PATH} ../

LIBS += -L$${QSCI_LIB_PATH} \
 	-lqscintilla2

message ("Qsci library path: $${QSCI_LIB_PATH}")
message ("Qsci header path: $${QSCI_HEADER_PATH}")
