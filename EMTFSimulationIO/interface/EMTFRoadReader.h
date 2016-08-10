#ifndef EMTFSimulationIO_EMTFRoadReader_h_
#define EMTFSimulationIO_EMTFRoadReader_h_

#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/EMTFRoad.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/CSCStubReader.h"


namespace phasetwoemtf {

// _____________________________________________________________________________
class EMTFRoadReader : public CSCStubReader {
public:
    EMTFRoadReader(int verbose=1);
    ~EMTFRoadReader();

    void init(TString src, TString prefix="", TString suffix="");  // override

    // Roads
    std::vector<unsigned> *                             vr_patternRef;
    std::vector<unsigned> *                             vr_sector;
    std::vector<unsigned> *                             vr_nstubs;
    std::vector<float> *                                vr_patternInvPt;
    std::vector<std::vector<unsigned> > *               vr_superstripIds;
    std::vector<std::vector<std::vector<unsigned> > > * vr_stubRefs;
};


// _____________________________________________________________________________
class EMTFRoadWriter : public CSCStubWriter {
public:
    EMTFRoadWriter(int verbose=1);
    ~EMTFRoadWriter();

    int init(TTree* intree, TString out, TString prefix="", TString suffix="");  // override

    void fill(const std::vector<EMTFRoad>& roads);  // override

protected:
    // Roads
    std::unique_ptr<std::vector<unsigned> >                             vr_patternRef;
    std::unique_ptr<std::vector<unsigned> >                             vr_sector;
    std::unique_ptr<std::vector<unsigned> >                             vr_nstubs;
    std::unique_ptr<std::vector<float> >                                vr_patternInvPt;
    std::unique_ptr<std::vector<std::vector<unsigned> > >               vr_superstripIds;
    std::unique_ptr<std::vector<std::vector<std::vector<unsigned> > > > vr_stubRefs;
};

}  // namespace phasetwoemtf

#endif
