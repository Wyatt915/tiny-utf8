#!/bin/bash
ulist=~/scripts/UnicodeNamesList.txt
grep -E '^[[:alnum:]]' $ulist | grep -v '<control>' \
    | dmenu -i\
    | (read selection; grep "$selection" $ulist) \
    | awk '{print $1}'\
    | tiny-utf8 \
#   | xclip -selection "clipboard"
    | xargs xdotool type
