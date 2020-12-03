#!/bin/bash
set -x
for f in $(ls files/before)
do
    if test -f files/after/$f
    then
       diff -u files/before/$f files/after/$f > files/diffs/$f 
    fi

done
