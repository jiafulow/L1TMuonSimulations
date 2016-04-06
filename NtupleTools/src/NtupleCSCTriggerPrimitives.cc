#include "L1TMuonSimulations/NtupleTools/interface/NtupleCSCTriggerPrimitives.h"

#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitive.h"  // why deprecate?
//#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
//#include "DataFormats/MuonDetId/interface/RPCDetId.h"


NtupleCSCTriggerPrimitives::NtupleCSCTriggerPrimitives(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    token_ = consumes<CSCCorrelatedLCTDigiCollection>(inputTag_);

    produces<std::vector<uint32_t> > (prefix_ + "geoId"         + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "subsystem"     + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "iendcap"       + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "istation"      + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "iring"         + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "ichamber"      + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "ilayer"        + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "triggerSector" + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "triggerCscId"  + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "trknmb"        + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "valid"         + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "quality"       + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "keywire"       + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "strip"         + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "pattern"       + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "bend"          + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "bx"            + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "mpclink"       + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "bx0"           + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "syncErr"       + suffix_);
    produces<std::vector<uint16_t> > (prefix_ + "cscID"         + suffix_);
    produces<unsigned>               (prefix_ + "size"          + suffix_);
}

NtupleCSCTriggerPrimitives::~NtupleCSCTriggerPrimitives() {}

void NtupleCSCTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> > v_geoId         (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<uint16_t> > v_subsystem     (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_iendcap       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_istation      (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_iring         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_ichamber      (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_ilayer        (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_triggerSector (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_triggerCscId  (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_trknmb        (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_valid         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_quality       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_keywire       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_strip         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_pattern       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_bend          (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_bx            (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_mpclink       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_bx0           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_syncErr       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> > v_cscID         (new std::vector<uint16_t>());
    std::auto_ptr<unsigned>               v_size          (new unsigned(0));


    //__________________________________________________________________________

    edm::Handle<CSCCorrelatedLCTDigiCollection> muonDigis;
    //iEvent.getByLabel(inputTag_, muonDigis);
    if (!token_.isUninitialized())
        iEvent.getByToken(token_, muonDigis);

    if (muonDigis.isValid()) {
        //edm::LogInfo("NtupleCSCTriggerPrimitives") << "Size: " << muonDigis->size();
        edm::LogInfo("NtupleCSCTriggerPrimitives") << "Size: ??";

        std::vector<L1TMuon::TriggerPrimitive> trigPrims;

        unsigned n = 0;

        // Make trigger primitives

        // Loop over chambers
        for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator itr = muonDigis->begin(); itr != muonDigis->end(); ++itr) {

            // Loop over digis
            for (CSCCorrelatedLCTDigiCollection::const_iterator it = (*itr).second.first; it != (*itr).second.second; ++it) {
                //if (!selector_(*it))
                //    continue;

                trigPrims.push_back(L1TMuon::TriggerPrimitive((*itr).first, *it));
            }
        }

        // Loop over trigger primitives
        for (std::vector<L1TMuon::TriggerPrimitive>::const_iterator it = trigPrims.cbegin(); it != trigPrims.cend(); ++it) {

            if (it->subsystem() == L1TMuon::TriggerPrimitive::kCSC) {

                const CSCDetId cscDet = it->detId<CSCDetId>();
                const L1TMuon::TriggerPrimitive::CSCData cscData = it->getCSCData();

                v_geoId         ->push_back(it->rawId().rawId());
                v_subsystem     ->push_back(it->subsystem());
                v_iendcap       ->push_back(cscDet.endcap());
                v_istation      ->push_back(cscDet.station());
                v_iring         ->push_back(cscDet.ring());
                v_ichamber      ->push_back(cscDet.chamber());
                v_ilayer        ->push_back(cscDet.layer());
                v_triggerSector ->push_back(cscDet.triggerSector());
                v_triggerCscId  ->push_back(cscDet.triggerCscId());
                v_trknmb        ->push_back(cscData.trknmb);
                v_valid         ->push_back(cscData.valid);
                v_quality       ->push_back(cscData.quality);
                v_keywire       ->push_back(cscData.keywire);
                v_strip         ->push_back(cscData.strip);    // half-strip precision
                v_pattern       ->push_back(cscData.pattern);
                v_bend          ->push_back(cscData.bend);
                v_bx            ->push_back(cscData.bx);
                v_mpclink       ->push_back(cscData.mpclink);
                v_bx0           ->push_back(cscData.bx0);
                v_syncErr       ->push_back(cscData.syncErr);
                v_cscID         ->push_back(cscData.cscID);

                n++;
            }
        }

        *v_size = v_geoId->size();

    } else {
        edm::LogError("NtupleCSCTriggerPrimitives") << "Cannot get the product: " << inputTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_geoId         , prefix_ + "geoId"         + suffix_);
    iEvent.put(v_subsystem     , prefix_ + "subsystem"     + suffix_);
    iEvent.put(v_iendcap       , prefix_ + "iendcap"       + suffix_);
    iEvent.put(v_istation      , prefix_ + "istation"      + suffix_);
    iEvent.put(v_iring         , prefix_ + "iring"         + suffix_);
    iEvent.put(v_ichamber      , prefix_ + "ichamber"      + suffix_);
    iEvent.put(v_ilayer        , prefix_ + "ilayer"        + suffix_);
    iEvent.put(v_triggerSector , prefix_ + "triggerSector" + suffix_);
    iEvent.put(v_triggerCscId  , prefix_ + "triggerCscId"  + suffix_);
    iEvent.put(v_trknmb        , prefix_ + "trknmb"        + suffix_);
    iEvent.put(v_valid         , prefix_ + "valid"         + suffix_);
    iEvent.put(v_quality       , prefix_ + "quality"       + suffix_);
    iEvent.put(v_keywire       , prefix_ + "keywire"       + suffix_);
    iEvent.put(v_strip         , prefix_ + "strip"         + suffix_);
    iEvent.put(v_pattern       , prefix_ + "pattern"       + suffix_);
    iEvent.put(v_bend          , prefix_ + "bend"          + suffix_);
    iEvent.put(v_bx            , prefix_ + "bx"            + suffix_);
    iEvent.put(v_mpclink       , prefix_ + "mpclink"       + suffix_);
    iEvent.put(v_bx0           , prefix_ + "bx0"           + suffix_);
    iEvent.put(v_syncErr       , prefix_ + "syncErr"       + suffix_);
    iEvent.put(v_cscID         , prefix_ + "cscID"         + suffix_);
    iEvent.put(v_size          , prefix_ + "size"          + suffix_);

}
