#!/bin/bash

# Pack necessary stuff
if [ "$1" == "--no-pack" ]
then
    echo "no pack."
else
    python pack.py
fi

# Condor jobs
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap SingleMuon mu0 2016 30
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T SingleMuon mu0 2016 30

python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_3GeV SingleMuon mu0 2016 5
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_5GeV SingleMuon mu0 2016 5
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_10GeV SingleMuon mu0 2016 5
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_20GeV SingleMuon mu0 2016 5
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_50GeV SingleMuon mu0 2016 5
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_3GeV SingleMuon mu0 2016 5
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_5GeV SingleMuon mu0 2016 5
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_10GeV SingleMuon mu0 2016 5
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_20GeV SingleMuon mu0 2016 5
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_50GeV SingleMuon mu0 2016 5

# When the condor jobs are done, do this:
#python retrieve.py
