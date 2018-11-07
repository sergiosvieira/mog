#!/bin/bash

array=("*.pro.user" "*.moc" "*.pro.user.*" "*.qbs.user" "*.qbs.user.*" "moc_*.cpp" "moc_*.h" "qrc_*.cpp" "ui_*.h" "*.qmlc" "*.o")
for f in "${array[@]}˜"
do
	find . -iname "$f" -exec rm {} \; -print
done
array=(mog.app html latex rtf)
for f in "${array[@]}"
do
	rm -rf $f
done
