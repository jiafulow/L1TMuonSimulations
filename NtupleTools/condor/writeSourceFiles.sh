#!/bin/bash

#ls -v $1/*.root | sed -e 's@^/eos/uscms@root://cmsxrootd-site.fnal.gov/@'
ls -v $1/*.root | sed -e 's@^/cms/data@root://xrootd2.ihepa.ufl.edu/@'
