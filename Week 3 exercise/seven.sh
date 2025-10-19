#!/bin/bash
tr ',' '\n' < ../stop_words.txt > /tmp/stop_words.tmp; cat "$1" | tr '[:upper:]' '[:lower:]' | sed -E "s/'s|n't|'re|'ve|'ll|'d|'m\b//g" | tr -cs '[:alpha:]' '\n' | grep -vwFf /tmp/stop_words.tmp | sort | uniq -c | sort -rn | head -25 | awk '{printf "%s  -  %s\n", $2, $1}'; rm /tmp/stop_words.tmp
