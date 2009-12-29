#!/bin/sh
VERSION=$(git --no-pager describe HEAD 2> /dev/null || git --no-pager rev-parse --short HEAD)
cat > version.txt <<EOF
*QsciEdit version $VERSION*
EOF
