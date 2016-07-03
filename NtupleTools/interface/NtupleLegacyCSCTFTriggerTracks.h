#ifndef NtupleTools_NtupleLegacyCSCTFTriggerTracks_h_
#define NtupleTools_NtupleLegacyCSCTFTriggerTracks_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1CSCTrackCollection.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerScales.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerScalesRcd.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerPtScale.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerPtScaleRcd.h"

class NtupleLegacyCSCTFTriggerTracks : public edm::EDProducer {
  public:
    explicit NtupleLegacyCSCTFTriggerTracks(const edm::ParameterSet&);
    ~NtupleLegacyCSCTFTriggerTracks();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    // Event setup
    const L1MuTriggerScales * scales_;
    const L1MuTriggerPtScale * ptScale_;

    const edm::InputTag stubTag_, trackTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<L1CSCTrack> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> stubToken_;
    edm::EDGetTokenT<L1CSCTrackCollection> trackToken_;
};

#endif
