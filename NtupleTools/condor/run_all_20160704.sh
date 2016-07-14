#!/bin/bash

# Pack necessary stuff
if [ "$1" == "--no-pack" ]; then
    echo "no pack."
else
    python pack.py
fi

# Condor jobs
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap singlemu emtf 2016 80
python jobify.py NoAnalyzer SingleMuon_NegativeEndCap singlemu emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T singlemu_0T emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_NegativeEndCap_0T singlemu_0T emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_flatPt singlemu_flatPt emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_NegativeEndCap_flatPt singlemu_flatPt emtf 2016 40

python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_3GeV singlemu_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_5GeV singlemu_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_10GeV singlemu_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_20GeV singlemu_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_50GeV singlemu_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_200GeV singlemu_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_1000GeV singlemu_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_3GeV singlemu_0T_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_5GeV singlemu_0T_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_10GeV singlemu_0T_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_20GeV singlemu_0T_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_50GeV singlemu_0T_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_200GeV singlemu_0T_gev emtf 2016 10
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_0T_1000GeV singlemu_0T_gev emtf 2016 10

python jobify.py NoAnalyzer SingleNeutrino_PUSpring16 singlenu emtf 2016 80
#python jobify.py NoAnalyzer SingleNeutrino_PU8to42 singlenu_flatPU8to42 emtf 2016 40
python jobify.py NoAnalyzer SingleNeutrino_PU0to50 singlenu_flatPU emtf 2016 40
#python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU8to42 singlemu_flatPU8to42 emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU0to50 singlemu_flatPU emtf 2016 40

python jobify.py NoAnalyzer SingleNeutrino_PU10 singlenu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleNeutrino_PU20 singlenu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleNeutrino_PU30 singlenu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleNeutrino_PU40 singlenu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleNeutrino_PU45 singlenu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleNeutrino_PU50 singlenu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU10 singlemu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU20 singlemu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU30 singlemu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU40 singlemu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU45 singlemu_pu emtf 2016 40
python jobify.py NoAnalyzer SingleMuon_PositiveEndCap_PU50 singlemu_pu emtf 2016 40

python jobify.py NoAnalyzer SingleMuon_PositiveBarrel singlemu emtf 2016 40

# When the condor jobs are done, do this:
#python retrieve.py

