#!/bin/bash

SITE="T2_US_Florida"
#SITE="T3_US_FNALLPC"

USERNAME=`voms-proxy-info -identity | sed -e "s@.*CN=\(.*\)/CN=[0-9].*@\1@"`
LFN="/store/user/$USERNAME/"
PFN=`curl -s --insecure "https://cmsweb.cern.ch/phedex/datasvc/json/prod/lfn2pfn?protocol=srmv2&node=$SITE&lfn=$LFN" | sed -e "s@.*\(srm://.*$LFN\).*@\1@"`

echo $PFN
