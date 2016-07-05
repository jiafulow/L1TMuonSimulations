#ifndef NtupleTools_NtupleCSCTriggerPrimitives_h_
#define NtupleTools_NtupleCSCTriggerPrimitives_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

//#include "DataFormats/CSCDigi/interface/CSCWireDigi.h"
//#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"
//#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
//#include "DataFormats/CSCDigi/interface/CSCStripDigiCollection.h"
//#include "DataFormats/CSCDigi/interface/CSCComparatorDigi.h"
//#include "DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h"
//#include "DataFormats/CSCDigi/interface/CSCALCTDigi.h"
//#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
//#include "DataFormats/CSCDigi/interface/CSCCLCTDigi.h"
//#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"

//#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
//#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
//#include "Geometry/Records/interface/MuonGeometryRecord.h"
//#include "MagneticField/Engine/interface/MagneticField.h"
//#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitive.h"    // why deprecate?
#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitiveFwd.h" // why deprecate?
//#include "L1Trigger/L1TMuon/interface/deprecate/GeometryTranslator.h"      // why deprecate?
#include "L1TMuonSimulations/MuonTools/interface/GeometryTranslator2.h"

#include "L1Trigger/L1TMuonEndCap/interface/PrimitiveConverter.h"


class NtupleCSCTriggerPrimitives : public edm::EDProducer {
  public:
    explicit NtupleCSCTriggerPrimitives(const edm::ParameterSet&);
    ~NtupleCSCTriggerPrimitives();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    // Process CSC digis
    virtual void extractPrimitives(edm::Handle<CSCCorrelatedLCTDigiCollection> cscDigis,
                                   L1TMuon::TriggerPrimitiveCollection& out) const;

    // Event setup
    //const CSCGeometry * theCSCGeometry_;
    //const MagneticField * theMagneticField_;
    std::unique_ptr<L1TMuon::GeometryTranslator2> theGeometryTranslator_;

    // EMTF
    PrimitiveConverter primConv_;

    //const edm::InputTag wireTag_;
    //const edm::InputTag stripTag_;
    //const edm::InputTag compTag_;
    //const edm::InputTag alctTag_;
    //const edm::InputTag clctTag_;
    const edm::InputTag corrlctTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<CSCCorrelatedLCTDigi> selector_;
    const unsigned maxN_;

    //edm::EDGetTokenT<CSCWireDigiCollection>             wireToken_;
    //edm::EDGetTokenT<CSCStripDigiCollection>            stripToken_;
    //edm::EDGetTokenT<CSCComparatorDigiCollection>       compToken_;
    //edm::EDGetTokenT<CSCALCTDigiCollection>             alctToken_;
    //edm::EDGetTokenT<CSCCLCTDigiCollection>             clctToken_;
    edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection>    corrlctToken_;
};

#endif
