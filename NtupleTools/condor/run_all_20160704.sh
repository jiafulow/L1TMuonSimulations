#!/bin/bash

# Pack necessary stuff
if [ "$1" == "--no-pack" ]; then
    echo "no pack."
else
    python pack.py
fi

# Condor jobs
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap SingleMuon mu0 2016 40
python jobify.py NoAnalyzer SingleMuon_NegativeEndCap SingleMuon mu0 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T SingleMuon mu0 2016 40
python jobify.py NoAnalyzer SingleMuon_NegativeEndCap_0T SingleMuon mu0 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_flatPt SingleMuon mu0 2016 40
python jobify.py NoAnalyzer SingleMuon_NegativeEndCap_flatPt SingleMuon mu0 2016 40

python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_3GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_5GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_10GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_20GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_50GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_200GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_1000GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_3GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_5GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_10GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_20GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_50GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_200GeV SingleMuon mu0 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_1000GeV SingleMuon mu0 2016 10

python jobify.py NoAnalyzer SingleNeutrino_PUSpring16 SingleNeutrino nuS16 2016 80

python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU10 SingleMuon mu10 2016 80
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU20 SingleMuon mu20 2016 80
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU30 SingleMuon mu30 2016 80
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU40 SingleMuon mu40 2016 80
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU45 SingleMuon mu45 2016 80
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU50 SingleMuon mu50 2016 80

# When the condor jobs are done, do this:
#python retrieve.py
