#!/bin/bash

pid=`pgrep emtf`

maxRss=0

while true
do
  metrics=`ps --no-headers -o pid,cputime,rss,vsize,args  -ww -p ${pid}`
  if [ $? -ne 0 ] ; then break ; fi # make sure process is still alive
  
  rss=`echo $metrics|awk '{print $3}'`
  if [ $rss -gt $maxRss ]; then maxRss=$rss; fi
  
  echo -e "maxRss=$maxRss"
  
  sleep 5
done

