#!/bin/bash

find . -iname "*.pro.user" -exec rm {} \; -print
find . -iname "*.moc" -exec rm {} \; -print
find . -iname "*.pro.user.*" -exec rm {} \; -print
find . -iname "*.qbs.user" -exec rm {} \; -print
find . -iname "*.qbs.user.*" -exec rm {} \; -print
find . -iname "moc_*.cpp" -exec rm {} \; -print
find . -iname "moc_*.h" -exec rm {} \; -print
find . -iname "qrc_*.cpp" -exec rm {} \; -print
find . -iname "ui_*.h" -exec rm {} \; -print
find . -iname "*.qmlc" -exec rm {} \; -print
find . -iname "*.o" -exec rm {} \; -print
rm -rf mog.app
