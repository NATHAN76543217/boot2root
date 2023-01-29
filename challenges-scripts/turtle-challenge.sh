#!/bin/bash

if [ $# -lt 1 ]; then
	exit 1
fi

turtlefile=$1
outputfile='turtle_output.py'

echo "Formatting $turtlefile"
sed -E 's/Can you digest the message\? \:\)//g  ;  s/Avance ([0-9]+) spaces/forward(\1)/g; s/Recule ([0-9]+) spaces/backward(\1)/g; s/Tourne droite de ([0-9]+) degrees/right(\1)/g; s/Tourne gauche de ([0-9]+) degrees/left(\1)/g' $turtlefile > $outputfile

echo "Done: $outputfile"
