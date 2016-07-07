#!/bin/bash

# Pack necessary stuff
if [ "$1" == "--no-pack" ]; then
    echo "no pack."
else
    python pack.py
fi

# Condor jobs
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_foo singlemu emtf 2016 10

# When the condor jobs are done, do this:
#python retrieve.py
