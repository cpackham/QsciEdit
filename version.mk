# This makefile generates a version.cpp file based on version.cpp.in
# replacing some tags with values computed at build time.

VERSION_CPP_IN = version.cpp.in
version.output = ${QMAKE_FILE_BASE}
version.commands = sed -e \"s/@@VERSION@@/`git describe 2> /dev/null \
	|| git rev-parse --short HEAD`/g\" \
	-e \"s/@@DATE@@/`date --utc`/g\" < ${QMAKE_FILE_NAME} > ${QMAKE_FILE_OUT}
version.input = VERSION_CPP_IN
version.CONFIG += no_link
version.variable_out = SOURCES

QMAKE_EXTRA_COMPILERS += version

