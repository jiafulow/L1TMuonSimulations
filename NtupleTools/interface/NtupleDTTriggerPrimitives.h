#ifndef NtupleTools_NtupleDTTriggerPrimitives_h_
#define NtupleTools_NtupleDTTriggerPrimitives_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"

#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitive.h"    // why deprecate?
#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitiveFwd.h" // why deprecate?
//#include "L1Trigger/L1TMuon/interface/deprecate/GeometryTranslator.h"      // why deprecate?
#include "L1Trigger/L1TMuon/interface/deprecate/DTBunchCrossingCleaner.h"  // why deprecate
#include "L1TMuonSimulations/MuonTools/interface/GeometryTranslator2.h"


class NtupleDTTriggerPrimitives : public edm::EDProducer {
  public:
    explicit NtupleDTTriggerPrimitives(const edm::ParameterSet&);
    ~NtupleDTTriggerPrimitives();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    // Process DT digis
    virtual void extractPrimitives(edm::Handle<L1MuDTChambPhContainer> phiDigis,
                                   edm::Handle<L1MuDTChambThContainer> thetaDigis,
                                   L1TMuon::TriggerPrimitiveCollection& out) const;
    L1TMuon::TriggerPrimitive processDigis(const L1MuDTChambPhDigi&,
                                           const int &segment_number) const;
    L1TMuon::TriggerPrimitive processDigis(const L1MuDTChambThDigi&,
                                           const int bti_group) const;
    L1TMuon::TriggerPrimitive processDigis(const L1MuDTChambPhDigi&,
                                           const L1MuDTChambThDigi&,
                                           const int bti_group) const;
    int findBTIGroupForThetaDigi(const L1MuDTChambThDigi&,
                                 const int position) const;
    const int bx_min, bx_max;
    std::unique_ptr<L1TMuon::DTBunchCrossingCleaner> _bxc;

    // Event setup
    std::unique_ptr<L1TMuon::GeometryTranslator2> theGeometryTranslator_;

    const edm::InputTag dtPhTag_;
    const edm::InputTag dtThTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<L1MuDTChambPhDigi> selectorPh_;
    StringCutObjectSelector<L1MuDTChambThDigi> selectorTh_;
    const unsigned maxN_;

    edm::EDGetTokenT<L1MuDTChambPhContainer> dtPhToken_;
    edm::EDGetTokenT<L1MuDTChambThContainer> dtThToken_;
};

#endif
