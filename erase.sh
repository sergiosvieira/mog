#!/bin/bash

array=(*.pro.user *.moc *.pro.user.* *.qbs.user *.qbs.user.* moc_*.cpp moc_*.h qrc_*.cpp ui_*.h *.qmlc *.o mog.app html latex rtf)
for f in "${array[@]}"
do
	rm -rf $f
done
