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

    // Event setup
    std::unique_ptr<L1TMuon::GeometryTranslator2> theGeometryTranslator_;

    const edm::InputTag chambPhTag_;
    const edm::InputTag chambThTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<L1MuDTChambPhDigi> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<L1MuDTChambPhContainer> chambPhToken_;
    edm::EDGetTokenT<L1MuDTChambThContainer> chambThToken_;
};

#endif
