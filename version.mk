# This makefile generates a version.cpp file based on version.cpp.in
# replacing some tags with values computed at build time.

# For tarball builds we want to be able to pass the version information in
VERSION=$$(VERSION)
isEmpty( VERSION ) {
	VERSION=$$system(git describe --always)
}

VERSION_CPP_IN = version.cpp.in
version.output = ${QMAKE_FILE_BASE}
version.commands = sed -e \"s/@@VERSION@@/$${VERSION}/g\" \
		-e \"s/@@DATE@@/`date`/g\" < ${QMAKE_FILE_NAME} > ${QMAKE_FILE_OUT}
version.input = VERSION_CPP_IN
version.CONFIG += no_link
version.variable_out = SOURCES

win32{
        version.commands = copy ${QMAKE_FILE_NAME} ${QMAKE_FILE_OUT}
}

QMAKE_EXTRA_COMPILERS += version

