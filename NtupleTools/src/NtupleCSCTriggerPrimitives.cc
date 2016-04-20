#include "L1TMuonSimulations/NtupleTools/interface/NtupleCSCTriggerPrimitives.h"

//#include "DataFormats/MuonDetId/interface/DTChamberId.h"
//#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/CSCTriggerNumbering.h"

#include "L1Trigger/CSCCommonTrigger/interface/CSCConstants.h"
//#include "L1Trigger/CSCCommonTrigger/interface/CSCPatternLUT.h"

#include "L1Trigger/L1TMuonEndCap/interface/PrimitiveConverter.h"
//#include "L1Trigger/L1TMuonEndCap/interface/BXAnalyzer.h"
//#include "L1Trigger/L1TMuonEndCap/interface/ZoneCreation.h"
//#include "L1Trigger/L1TMuonEndCap/interface/PatternRecognition.h"


NtupleCSCTriggerPrimitives::NtupleCSCTriggerPrimitives(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    theGeometryTranslator_.reset(new L1TMuon::GeometryTranslator());

    token_ = consumes<CSCCorrelatedLCTDigiCollection>(inputTag_);

    produces<std::vector<uint32_t> >          (prefix_ + "geoId"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "subsystem"       + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "iendcap"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "istation"        + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "iring"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "ichamber"        + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "ilayer"          + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "isector"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "isubsector"      + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "triggerSector"   + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "triggerCscId"    + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "trknmb"          + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "valid"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "quality"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "keywire"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "strip"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "pattern"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "bend"            + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "bx"              + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "mpclink"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "bx0"             + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "syncErr"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "cscID"           + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalPhi"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalTheta"     + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalEta"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalRho"       + suffix_);
    produces<std::vector<int> >               (prefix_ + "convPhi"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "convTheta"       + suffix_);
    produces<std::vector<int> >               (prefix_ + "convPhit"        + suffix_);
    produces<std::vector<int> >               (prefix_ + "convPhzvl"       + suffix_);
    produces<std::vector<int> >               (prefix_ + "convZhit"        + suffix_);
    produces<std::vector<std::vector<int> > > (prefix_ + "convZoneCont"    + suffix_);
    produces<std::vector<float> >             (prefix_ + "convGlobalPhi"   + suffix_);
    produces<std::vector<float> >             (prefix_ + "convGlobalTheta" + suffix_);
    produces<std::vector<float> >             (prefix_ + "convGlobalEta"   + suffix_);
    produces<std::vector<float> >             (prefix_ + "convGlobalRho"   + suffix_);
    produces<unsigned>                        (prefix_ + "size"            + suffix_);
}

NtupleCSCTriggerPrimitives::~NtupleCSCTriggerPrimitives() {}

void NtupleCSCTriggerPrimitives::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    /// Geometry setup
    edm::ESHandle<CSCGeometry> cscGeometryHandle;
    iSetup.get<MuonGeometryRecord>().get(cscGeometryHandle);
    if (!cscGeometryHandle.isValid()) {
        edm::LogWarning("NtupleCSCTriggerPrimitives") << "Unable to get MuonGeometryRecord!";
    } else {
        theCSCGeometry_ = cscGeometryHandle.product();
    }

    /// Magnetic field setup
    edm::ESHandle<MagneticField> magneticFieldHandle;
    iSetup.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);
    if (!magneticFieldHandle.isValid()) {
        edm::LogWarning("NtupleCSCTriggerPrimitives") << "Unable to get IdealMagneticFieldRecord!";
    } else {
        theMagneticField_ = magneticFieldHandle.product();
    }

    theGeometryTranslator_->checkAndUpdateGeometry(iSetup);
}


namespace {
float getConvGlobalPhi(unsigned int isector, int iphi) {
    if (iphi == -999)  return -999.;
    float fphi = (iphi*0.0166666) + (isector%6)*60.0 + 13.0;
    fphi *= M_PI / 180.0;
    if (fphi > M_PI) fphi -= 2*M_PI;  // convert from [0,2pi] to [-pi,pi]
    return fphi;
}
float getConvGlobalTheta(unsigned int isector, int itheta) {
    if (itheta == -999)  return -999.;
    float ftheta = (itheta*0.2851562) + 8.5;
    ftheta *= M_PI / 180.0;
    return ftheta;
}
float getConvGlobalEta(unsigned int isector, int itheta) {
    if (itheta == -999)  return -999.;
    float ftheta = getConvGlobalTheta(isector, itheta);
    float feta = - std::log(std::tan(ftheta/2.0));
    if (isector/6 == 1) feta = -feta;
    return feta;
}
//float getConvGlobalRho()
double getThetaFromEta(double eta) {
    double cottheta = std::sinh(eta);
    return 1.0/std::atan(cottheta);
}
}

void NtupleCSCTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v_geoId           (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_subsystem       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_iendcap         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_istation        (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_iring           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_ichamber        (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_ilayer          (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_isector         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_isubsector      (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_triggerSector   (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_triggerCscId    (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_trknmb          (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_valid           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_quality         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_keywire         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_strip           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_pattern         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_bend            (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_bx              (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_mpclink         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_bx0             (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_syncErr         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_cscID           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<float> >             v_globalPhi       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalTheta     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalEta       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalRho       (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v_convPhi         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_convTheta       (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_convPhit        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_convPhzvl       (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_convZhit        (new std::vector<int>());
    std::auto_ptr<std::vector<std::vector<int> > > v_convZoneCont    (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<float> >             v_convGlobalPhi   (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_convGlobalTheta (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_convGlobalEta   (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_convGlobalRho   (new std::vector<float>());
    std::auto_ptr<unsigned>                        v_size            (new unsigned(0));


    //__________________________________________________________________________

    edm::Handle<CSCCorrelatedLCTDigiCollection> muonDigis;
    //iEvent.getByLabel(inputTag_, muonDigis);
    if (!token_.isUninitialized())
        iEvent.getByToken(token_, muonDigis);

    if (muonDigis.isValid()) {
        //edm::LogInfo("NtupleCSCTriggerPrimitives") << "Size: " << muonDigis->size();
        edm::LogInfo("NtupleCSCTriggerPrimitives") << "Size: ??";


        // Make trigger primitives

        std::vector<L1TMuon::TriggerPrimitive> trigPrims;

        // Loop over chambers
        for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator itr = muonDigis->begin(); itr != muonDigis->end(); ++itr) {

            // Loop over digis
            for (CSCCorrelatedLCTDigiCollection::const_iterator it = (*itr).second.first; it != (*itr).second.second; ++it) {
                //if (!selector_(*it))
                //    continue;

                trigPrims.push_back(L1TMuon::TriggerPrimitive((*itr).first, *it));
            }
        }

        unsigned n = 0;

        // Loop over trigger primitives
        for (std::vector<L1TMuon::TriggerPrimitive>::const_iterator it = trigPrims.cbegin(); it != trigPrims.cend(); ++it) {
            if (n >= maxN_)
                break;

            if (it->subsystem() == L1TMuon::TriggerPrimitive::kCSC) {

                const CSCDetId cscDet = it->detId<CSCDetId>();
                const L1TMuon::TriggerPrimitive::CSCData cscData = it->getCSCData();

                const unsigned int isector = (cscDet.endcap()-1)*6 + (cscDet.triggerSector()-1);
                const unsigned int isubsector = (cscDet.station() != 1) ? 0 : ((cscDet.chamber()%6 > 2) ? 1 : 2);

                // Global coordinates
                const double globalPhi = theGeometryTranslator_->calculateGlobalPhi(*it);
                const double globalEta = theGeometryTranslator_->calculateGlobalEta(*it);
                //const double globalTheta = getThetaFromEta(globalEta);

                // Global coordinates (more ...)
                const CSCChamber* chamb(theCSCGeometry_->chamber(cscDet));
                const CSCLayer* layer(chamb->layer(CSCConstants::KEY_ALCT_LAYER));
                const LocalPoint& coarse_lp = layer->geometry()->stripWireGroupIntersection(cscData.strip, cscData.keywire);
                const GlobalPoint& coarse_gp = layer->surface().toGlobal(coarse_lp);
                const double globalTheta = coarse_gp.theta();
                const double globalRho = coarse_gp.mag();

                // Sanity checks
                assert(CSCTriggerNumbering::triggerSectorFromLabels(cscDet) == cscDet.triggerSector());
                assert(CSCTriggerNumbering::triggerCscIdFromLabels(cscDet) == cscDet.triggerCscId());
                assert(CSCTriggerNumbering::triggerSubSectorFromLabels(cscDet) == (int) isubsector);
                assert(cscData.cscID == cscDet.triggerCscId());

                // Get ConvertedHit
                std::vector<L1TMuon::TriggerPrimitive> tester;
                tester.push_back(*it);
                const std::vector<ConvertedHit>& convHits = PrimConv(tester, isector);

                ConvertedHit convHit;
                if (convHits.size()) {
                    convHit = convHits.front();
                } else {
                    convHit.SetNull();  // why fail?
                }


                v_geoId           ->push_back(it->rawId().rawId());
                v_subsystem       ->push_back(it->subsystem());
                v_iendcap         ->push_back(cscDet.endcap());
                v_istation        ->push_back(cscDet.station());
                v_iring           ->push_back(cscDet.ring());
                v_ichamber        ->push_back(cscDet.chamber());
                v_ilayer          ->push_back(cscDet.layer());
                v_isector         ->push_back(isector);
                v_isubsector      ->push_back(isubsector);
                v_triggerSector   ->push_back(cscDet.triggerSector());
                v_triggerCscId    ->push_back(cscDet.triggerCscId());
                v_trknmb          ->push_back(cscData.trknmb);
                v_valid           ->push_back(cscData.valid);
                v_quality         ->push_back(cscData.quality);
                v_keywire         ->push_back(cscData.keywire);  // wire group 0-111
                v_strip           ->push_back(cscData.strip);    // half-strip precision 0-159
                v_pattern         ->push_back(cscData.pattern);  // 0-10
                v_bend            ->push_back(cscData.bend);
                v_bx              ->push_back(cscData.bx);
                v_mpclink         ->push_back(cscData.mpclink);
                v_bx0             ->push_back(cscData.bx0);
                v_syncErr         ->push_back(cscData.syncErr);
                v_cscID           ->push_back(cscData.cscID);
                v_globalPhi       ->push_back(globalPhi);
                v_globalTheta     ->push_back(globalTheta);
                v_globalEta       ->push_back(globalEta);
                v_globalRho       ->push_back(globalRho);
                v_convPhi         ->push_back(convHit.Phi());
                v_convTheta       ->push_back(convHit.Theta());
                v_convPhit        ->push_back(convHit.Ph_hit());
                v_convPhzvl       ->push_back(convHit.Phzvl());
                v_convZhit        ->push_back(convHit.Zhit());
                v_convZoneCont    ->push_back(convHit.ZoneContribution());
                v_convGlobalPhi   ->push_back(getConvGlobalPhi(isector, convHit.Phi()));
                v_convGlobalTheta ->push_back(getConvGlobalTheta(isector, convHit.Theta()));
                v_convGlobalEta   ->push_back(getConvGlobalEta(isector, convHit.Theta()));
                v_convGlobalRho   ->push_back(0.);

                n++;
            }
        }

        *v_size = v_geoId->size();

    } else {
        edm::LogError("NtupleCSCTriggerPrimitives") << "Cannot get the product: " << inputTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_geoId           , prefix_ + "geoId"           + suffix_);
    iEvent.put(v_subsystem       , prefix_ + "subsystem"       + suffix_);
    iEvent.put(v_iendcap         , prefix_ + "iendcap"         + suffix_);
    iEvent.put(v_istation        , prefix_ + "istation"        + suffix_);
    iEvent.put(v_iring           , prefix_ + "iring"           + suffix_);
    iEvent.put(v_ichamber        , prefix_ + "ichamber"        + suffix_);
    iEvent.put(v_ilayer          , prefix_ + "ilayer"          + suffix_);
    iEvent.put(v_isector         , prefix_ + "isector"         + suffix_);
    iEvent.put(v_isubsector      , prefix_ + "isubsector"      + suffix_);
    iEvent.put(v_triggerSector   , prefix_ + "triggerSector"   + suffix_);
    iEvent.put(v_triggerCscId    , prefix_ + "triggerCscId"    + suffix_);
    iEvent.put(v_trknmb          , prefix_ + "trknmb"          + suffix_);
    iEvent.put(v_valid           , prefix_ + "valid"           + suffix_);
    iEvent.put(v_quality         , prefix_ + "quality"         + suffix_);
    iEvent.put(v_keywire         , prefix_ + "keywire"         + suffix_);
    iEvent.put(v_strip           , prefix_ + "strip"           + suffix_);
    iEvent.put(v_pattern         , prefix_ + "pattern"         + suffix_);
    iEvent.put(v_bend            , prefix_ + "bend"            + suffix_);
    iEvent.put(v_bx              , prefix_ + "bx"              + suffix_);
    iEvent.put(v_mpclink         , prefix_ + "mpclink"         + suffix_);
    iEvent.put(v_bx0             , prefix_ + "bx0"             + suffix_);
    iEvent.put(v_syncErr         , prefix_ + "syncErr"         + suffix_);
    iEvent.put(v_cscID           , prefix_ + "cscID"           + suffix_);
    iEvent.put(v_globalPhi       , prefix_ + "globalPhi"       + suffix_);
    iEvent.put(v_globalTheta     , prefix_ + "globalTheta"     + suffix_);
    iEvent.put(v_globalEta       , prefix_ + "globalEta"       + suffix_);
    iEvent.put(v_globalRho       , prefix_ + "globalRho"       + suffix_);
    iEvent.put(v_convPhi         , prefix_ + "convPhi"         + suffix_);
    iEvent.put(v_convTheta       , prefix_ + "convTheta"       + suffix_);
    iEvent.put(v_convPhit        , prefix_ + "convPhit"        + suffix_);
    iEvent.put(v_convPhzvl       , prefix_ + "convPhzvl"       + suffix_);
    iEvent.put(v_convZhit        , prefix_ + "convZhit"        + suffix_);
    iEvent.put(v_convZoneCont    , prefix_ + "convZoneCont"    + suffix_);
    iEvent.put(v_convGlobalPhi   , prefix_ + "convGlobalPhi"   + suffix_);
    iEvent.put(v_convGlobalTheta , prefix_ + "convGlobalTheta" + suffix_);
    iEvent.put(v_convGlobalEta   , prefix_ + "convGlobalEta"   + suffix_);
    iEvent.put(v_convGlobalRho   , prefix_ + "convGlobalRho"   + suffix_);
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);

}
