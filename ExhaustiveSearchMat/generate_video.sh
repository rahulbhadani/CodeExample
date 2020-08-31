#!/bin/bash

search_dir="./"
for entry in "$search_dir"/*
do
  #echo "$entry" | cut -d'/' -f 3
  SUBSTR=$(echo $entry | cut -d'/' -f 3)
  
  command=$(echo ffmpeg -framerate 1 -pattern_type glob -i \'${SUBSTR}/*.png\' ${SUBSTR}_SignalLen_SignalPos.mp4)
  echo $command >> ffpmeg
done

