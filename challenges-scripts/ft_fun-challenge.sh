#!/bin/bash

pcap_folder="./pcap"

outfile="solved.c"

if [ $# -gt 0 ]; then
	pcap_folder=$1 ;
fi


fun=""

i=0
for file in $pcap_folder/*.pcap ; do
    file_content=$(cat $file | sed 's/\n//g' | sed 's/\\n/\\\\n/g' | sed 's+\*+\\\*+g' ) ;

    file_n0=$(printf %.3d $(echo $file_content | sed 's+.*//file\(.*[0-9]\)+\1+g') );

    fun=$fun$file_n0' '$file_content'\n'

    i=$(echo $i + 1 | bc)
    printf "\rSorting Files [$i/$(ls $pcap_folder/*.pcap | wc -l | tr -d ' ')] ($file)"
done

echo "\nExecuting script."

echo $fun | sort | sed 's/\\\*/\*/g' | cut -d' ' -f2- > $outfile
gcc $outfile -o out
password=$(./out | grep PASSWORD | cut -d' ' -f4)

rm $outfile
rm out


echo found password:'\t"'$password'"'

sha=$(printf $password | shasum -a 256 | cut -d' ' -f1)

echo found sha:'\t"'$sha'"'
