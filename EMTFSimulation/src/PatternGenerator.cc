#include "L1TMuonSimulations/EMTFSimulation/interface/PatternGenerator.h"

PatternGenerator::PatternGenerator(const ProgramOption& po)
: po_(po), nEvents_(po.maxEvents), verbose_(po.verbose)
{

}

PatternGenerator::~PatternGenerator() {}

void PatternGenerator::init() {

}

void PatternGenerator::run() {

}

// _____________________________________________________________________________
// Make pattern bank
void PatternGenerator::makePatterns(TString src) {

}

// _____________________________________________________________________________
// Write pattern bank
void PatternGenerator::writePatterns(TString out) {

}

