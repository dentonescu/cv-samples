#!/bin/sh
py="python3"
if [ -z "$(which $py)" ]; then
 py="python"
 if [ -z "$(which $py)" ]; then
  echo "ERROR: It appears that Python is either not installed or not on the path."
  exit 1
 fi
fi
cmd="$py slideshow-server.py publicdomainpictures.net/"
echo $cmd
eval $cmd
