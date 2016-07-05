#ifndef NtupleTools_NtupleRPCTriggerPrimitives_h_
#define NtupleTools_NtupleRPCTriggerPrimitives_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/RPCDigi/interface/RPCDigi.h"
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"

#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitive.h"    // why deprecate?
#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitiveFwd.h" // why deprecate?
//#include "L1Trigger/L1TMuon/interface/deprecate/GeometryTranslator.h"      // why deprecate?
#include "L1TMuonSimulations/MuonTools/interface/GeometryTranslator2.h"


class NtupleRPCTriggerPrimitives : public edm::EDProducer {
  public:
    explicit NtupleRPCTriggerPrimitives(const edm::ParameterSet&);
    ~NtupleRPCTriggerPrimitives();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    // Process RPC digis
    virtual void extractPrimitives(edm::Handle<RPCDigiCollection> rpcDigis,
                                   L1TMuon::TriggerPrimitiveCollection& out) const;

    // Event setup
    std::unique_ptr<L1TMuon::GeometryTranslator2> theGeometryTranslator_;

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<RPCDigi> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<RPCDigiCollection> token_;
};

#endif
