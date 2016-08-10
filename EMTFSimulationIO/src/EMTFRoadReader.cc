#include "L1TMuonSimulations/EMTFSimulationIO/interface/EMTFRoadReader.h"

#include <iostream>
#include <cassert>
#include <stdexcept>


namespace phasetwoemtf {

// _____________________________________________________________________________
EMTFRoadReader::EMTFRoadReader(int verbose)
: CSCStubReader(verbose),
  //
  vr_patternRef   (0),
  vr_sector       (0),
  vr_nstubs       (0),
  vr_patternInvPt (0),
  vr_superstripIds(0),
  vr_stubRefs     (0) {}

EMTFRoadReader::~EMTFRoadReader() {}

void EMTFRoadReader::init(TString src, TString prefix, TString suffix) {
    CSCStubReader::init(src, prefix, suffix);

    // Set branch addresses
    tchain_->SetBranchAddress(prefix + "patternRef"    + suffix, &(vr_patternRef));
    tchain_->SetBranchAddress(prefix + "sector"        + suffix, &(vr_sector));
    tchain_->SetBranchAddress(prefix + "nstubs"        + suffix, &(vr_nstubs));
    tchain_->SetBranchAddress(prefix + "patternInvPt"  + suffix, &(vr_patternInvPt));
    tchain_->SetBranchAddress(prefix + "superstripIds" + suffix, &(vr_superstripIds));
    tchain_->SetBranchAddress(prefix + "stubRefs"      + suffix, &(vr_stubRefs));
}


// _____________________________________________________________________________
EMTFRoadWriter::EMTFRoadWriter(int verbose)
: CSCStubWriter(verbose),
  //
  vr_patternRef     (new std::vector<unsigned>()),
  vr_sector         (new std::vector<unsigned>()),
  vr_nstubs         (new std::vector<unsigned>()),
  vr_patternInvPt   (new std::vector<float>()),
  vr_superstripIds  (new std::vector<std::vector<unsigned> >()),
  vr_stubRefs       (new std::vector<std::vector<std::vector<unsigned> > >()) {}

EMTFRoadWriter::~EMTFRoadWriter() {}

int EMTFRoadWriter::init(TTree* intree, TString out, TString prefix, TString suffix) {
    CSCStubWriter::init(intree, out, prefix, suffix);

    ttree_->Branch(prefix + "patternRef"    + suffix, &(*vr_patternRef));
    ttree_->Branch(prefix + "sector"        + suffix, &(*vr_sector));
    ttree_->Branch(prefix + "nstubs"        + suffix, &(*vr_nstubs));
    ttree_->Branch(prefix + "patternInvPt"  + suffix, &(*vr_patternInvPt));
    ttree_->Branch(prefix + "superstripIds" + suffix, &(*vr_superstripIds));
    ttree_->Branch(prefix + "stubRefs"      + suffix, &(*vr_stubRefs));
    return 0;
}

void EMTFRoadWriter::fill(const std::vector<EMTFRoad>& roads) {
    vr_patternRef   ->clear();
    vr_sector       ->clear();
    vr_nstubs       ->clear();
    vr_patternInvPt ->clear();
    vr_superstripIds->clear();
    vr_stubRefs     ->clear();

    const unsigned nroads = roads.size();
    for (unsigned i=0; i<nroads; ++i) {
        const EMTFRoad& road = roads.at(i);
        vr_patternRef   ->push_back(road.patternRef);
        vr_sector       ->push_back(road.sector);
        vr_nstubs       ->push_back(road.nstubs);
        vr_patternInvPt ->push_back(road.patternInvPt);
        vr_superstripIds->push_back(road.superstripIds);
        vr_stubRefs     ->push_back(road.stubRefs);
    }

    ttree_->Fill();
    assert(vr_patternRef->size() == nroads);
}

}  // namespace phasetwoemtf
