#!/bin/bash
# @author Lukas Gottschall
PID=`ps aux | grep $1 | grep -v grep | awk '{ print $2 }'`
PATH=`ls -ald --color=never /proc/$PID/exe | awk '{ print $10 }'`
echo $PATH
