#include "L1TMuonSimulations/NtupleTools/interface/NtupleLegacyCSCTFTriggerTracks.h"

#include "L1TMuonSimulations/NtupleTools/interface/MapCSCTriggerPrimitives.h"

#include "L1Trigger/CSCTrackFinder/interface/CSCTFPtLUT.h"


NtupleLegacyCSCTFTriggerTracks::NtupleLegacyCSCTFTriggerTracks(const edm::ParameterSet& iConfig) :
  stubTag_ (iConfig.getParameter<edm::InputTag>("stubTag")),
  trackTag_(iConfig.getParameter<edm::InputTag>("trackTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    stubToken_ = consumes<CSCCorrelatedLCTDigiCollection>(stubTag_);
    trackToken_ = consumes<L1CSCTrackCollection>(trackTag_);

    //produces<std::vector<float> >             (prefix_ + "phi"             + suffix_);
    //produces<std::vector<float> >             (prefix_ + "eta"             + suffix_);
    //produces<std::vector<float> >             (prefix_ + "pt"              + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "charge"          + suffix_);
    produces<std::vector<bool> >              (prefix_ + "chargeValid"     + suffix_);
    produces<std::vector<bool> >              (prefix_ + "isFineHalo"      + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "quality"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "typeIdx"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "bx"              + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "phiPacked"       + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "etaPacked"       + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "ptPacked"        + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "dataword"        + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "rank"            + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "localPhi"        + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "me1ID"           + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "me2ID"           + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "me3ID"           + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "me4ID"           + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "mb1ID"           + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "iendcap"         + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "isector"         + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "ptLUTAddress"    + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "frontRear"       + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "outputLink"      + suffix_);
    produces<std::vector<bool> >              (prefix_ + "winner"          + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "deltaPhi12"      + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "deltaPhi23"      + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "addressEta"      + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "mode"            + suffix_);
    //produces<std::vector<bool> >              (prefix_ + "sign"            + suffix_);
    //produces<std::vector<bool> >              (prefix_ + "syncErr"         + suffix_);
    //produces<std::vector<bool> >              (prefix_ + "bx0"             + suffix_);
    //produces<std::vector<bool> >              (prefix_ + "bc0"             + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "me1Tbin"         + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "me2Tbin"         + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "me3Tbin"         + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "me4Tbin"         + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "mb1Tbin"         + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "stubRefs"   + suffix_);
    produces<std::vector<float> >             (prefix_ + "trkPt"           + suffix_);
    produces<std::vector<float> >             (prefix_ + "trkPtOld"        + suffix_);
    produces<std::vector<float> >             (prefix_ + "trkPtMatt"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "trkPtGmt"        + suffix_);
    produces<std::vector<float> >             (prefix_ + "trkEta"          + suffix_);
    produces<std::vector<float> >             (prefix_ + "trkPhi"          + suffix_);
    produces<std::vector<int> >               (prefix_ + "trkMode"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "trkModeA"        + suffix_);
    produces<std::vector<int> >               (prefix_ + "trkModeB"        + suffix_);
    produces<std::vector<int> >               (prefix_ + "trkQual"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "trkQualA"        + suffix_);
    produces<std::vector<int> >               (prefix_ + "trkQualB"        + suffix_);
    produces<unsigned>                        (prefix_ + "size"            + suffix_);
}

NtupleLegacyCSCTFTriggerTracks::~NtupleLegacyCSCTFTriggerTracks() {}

void NtupleLegacyCSCTFTriggerTracks::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    // Legacy CSCTF pT look up tables
    edm::ESHandle<L1MuTriggerScales> scalesHandle;
    iSetup.get<L1MuTriggerScalesRcd>().get(scalesHandle);
    scales_ = scalesHandle.product();

    edm::ESHandle<L1MuTriggerPtScale> ptScaleHandle;
    iSetup.get<L1MuTriggerPtScaleRcd>().get(ptScaleHandle);
    ptScale_ = ptScaleHandle.product();
}

void NtupleLegacyCSCTFTriggerTracks::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    //std::auto_ptr<std::vector<float> >             v_phi             (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >             v_eta             (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >             v_pt              (new std::vector<float>());
    std::auto_ptr<std::vector<int16_t> >           v_charge          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<bool> >              v_chargeValid     (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v_isFineHalo      (new std::vector<bool>());
    std::auto_ptr<std::vector<uint16_t> >          v_quality         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_typeIdx         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<int> >               v_bx              (new std::vector<int>());
    std::auto_ptr<std::vector<unsigned> >          v_phiPacked       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_etaPacked       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_ptPacked        (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_dataword        (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_rank            (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_localPhi        (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_me1ID           (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_me2ID           (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_me3ID           (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_me4ID           (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_mb1ID           (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_iendcap         (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_isector         (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_ptLUTAddress    (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_frontRear       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_outputLink      (new std::vector<unsigned>());
    std::auto_ptr<std::vector<bool> >              v_winner          (new std::vector<bool>());
    //std::auto_ptr<std::vector<unsigned> >          v_deltaPhi12      (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<unsigned> >          v_deltaPhi23      (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<unsigned> >          v_addressEta      (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<unsigned> >          v_mode            (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<bool> >              v_sign            (new std::vector<bool>());
    //std::auto_ptr<std::vector<bool> >              v_syncErr         (new std::vector<bool>());
    //std::auto_ptr<std::vector<bool> >              v_bx0             (new std::vector<bool>());
    //std::auto_ptr<std::vector<bool> >              v_bc0             (new std::vector<bool>());
    //std::auto_ptr<std::vector<unsigned> >          v_me1Tbin         (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<unsigned> >          v_me2Tbin         (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<unsigned> >          v_me3Tbin         (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<unsigned> >          v_me4Tbin         (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<unsigned> >          v_mb1Tbin         (new std::vector<unsigned>());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v_stubRefs        (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<float> >             v_trkPt           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_trkPtOld        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_trkPtMatt       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_trkPtGmt        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_trkEta          (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_trkPhi          (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v_trkMode         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_trkModeA        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_trkModeB        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_trkQual         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_trkQualA        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_trkQualB        (new std::vector<int>());
    std::auto_ptr<unsigned>                        v_size            (new unsigned(0));


    //__________________________________________________________________________
    edm::Handle<CSCCorrelatedLCTDigiCollection> stubs;
    //iEvent.getByLabel(stubTag_, stubs);
    if (!stubToken_.isUninitialized())
        iEvent.getByToken(stubToken_, stubs);

    MapCSCTriggerPrimitives stubMap;
    stubMap.setup(stubs);


    //__________________________________________________________________________
    // Utilities
    edm::ParameterSet ptLUTConfig;
    ptLUTConfig.addParameter<bool>("ReadPtLUT", false);
    ptLUTConfig.addParameter<bool>("isBinary",  false);
    CSCTFPtLUT ptLUT(ptLUTConfig, scales_, ptScale_);

    // David Curry unconvered this from somewhere, sometime
    static const float ptscaleOld[31] =  { 0,
                     1.5,   2.0,   2.5,   3.0,   3.5,   4.0,
                     4.5,   5.0,   6.0,   7.0,   8.0,  10.0,  12.0,  14.0,
                     16.0,  18.0,  20.0,  25.0,  30.0,  35.0,  40.0,  45.0,
                     50.0,  60.0,  70.0,  80.0,  90.0, 100.0, 120.0, 140.0 };

    // From http://www.phys.ufl.edu/~mrcarver/forAD/L1TMuonUpgradedTrackFinder.h
    static const float ptscaleMatt[33] = {
        -1.,   0.0,   1.5,   2.0,   2.5,   3.0,   3.5,   4.0,
        4.5,   5.0,   6.0,   7.0,   8.0,  10.0,  12.0,  14.0,
        16.0,  18.0,  20.0,  25.0,  30.0,  35.0,  40.0,  45.0,
        50.0,  60.0,  70.0,  80.0,  90.0, 100.0, 120.0, 140.0, 1.E6 };


    //__________________________________________________________________________
    edm::Handle<L1CSCTrackCollection> tracks;
    //iEvent.getByLabel(trackTag_, tracks);
    if (!trackToken_.isUninitialized())
        iEvent.getByToken(trackToken_, tracks);

    if (tracks.isValid()) {
        edm::LogInfo("NtupleLegacyCSCTFTriggerTracks") << "Size: " << tracks->size();

        unsigned n = 0;

        for(L1CSCTrackCollection::const_iterator it = tracks->begin(); it != tracks->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!selector_(*it))
                continue;

            const csc::L1Track& it1 = it->first;
            const CSCCorrelatedLCTDigiCollection& it2 = it->second;

            // Get stubRefs
            std::vector<unsigned> myStubRefs;
            for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator itr = it2.begin(); itr != it2.end(); ++itr) {
                for (CSCCorrelatedLCTDigiCollection::const_iterator it = (*itr).second.first; it != (*itr).second.second; ++it) {
                    MapCSCTriggerPrimitives::StubRef stubRef = std::make_tuple((*itr).first.rawId(), it->getKeyWG(), it->getStrip(),
                        it->getPattern(), it->getBend(), it->getBX(), it->getCSCID());
                    unsigned myStubRef = stubMap.get(stubRef);

                    myStubRefs.push_back(myStubRef);
                }
            }

            // Extra from Andrew Brinkerhoff's EMTFAnalyzer
            const auto& lt = it;
            const auto& LCTs = lt -> second;
            float eta = 0.9 + 0.05*(lt->first.eta_packed()) + 0.025;
            unsigned sector = lt->first.sector();
            float phi = (0.05217*lt->first.localPhi()) + (sector-1)*1.1 + 0.0218;//*(3.14159265359/180)
            if(phi > 3.14159) phi -= 6.28318;

            unsigned pti = 0, quality = 0;
            lt->first.decodeRank(lt->first.rank(),pti,quality);//
            float ptOld = ptscaleOld[pti+1];
            float ptMatt = ptscaleMatt[pti+1];
            float ptGmt = ptscaleMatt[pti];
            int qualA = quality;

            // PtAddress gives an handle on other parameters
            ptadd thePtAddress(lt->first.ptLUTAddress());

            //Pt needs some more workaround since it is not in the unpacked data
            ptdat thePtData  = ptLUT.Pt(thePtAddress);

            int pt_bit = -999;

            // front or rear bit?
            int qualB = -99;
            if (thePtAddress.track_fr) {
                pt_bit = thePtData.front_rank&0x1f;
                qualB = (thePtData.front_rank >> 5) & 0x3;
                //csctf_.trChargeValid.push_back(thePtData.charge_valid_front);
            } else {
                pt_bit = thePtData.rear_rank&0x1f;
                qualB = (thePtData.rear_rank>>5) & 0x3;
                //csctf_.trChargeValid.push_back(thePtData.charge_valid_rear);
            }

            // convert the Pt in human readable values (GeV/c)
            float pt = ptScale_->getPtScale()->getLowEdge(pt_bit);

            // int qual = lt->first.cscMode();
            int qual = lt->first.mode();

            // For EMTF mode definition
            int mode = 0;
            if(lt->first.me1ID())
                mode |= 8;
            if(lt->first.me2ID())
                mode |= 4;
            if(lt->first.me3ID())
                mode |= 2;
            if(lt->first.me4ID())
                mode |= 1;

            int modeA = 0;
            if(lt->first.me1ID() > 0)
                modeA |= 8;
            if(lt->first.me2ID() > 0)
                modeA |= 4;
            if(lt->first.me3ID() > 0)
                modeA |= 2;
            if(lt->first.me4ID() > 0)
                modeA |= 1;

            int modeB = 0;
            auto chamber = LCTs.begin();
            auto chend  = LCTs.end();
            for( ; chamber != chend; ++chamber ) {
                CSCDetId id = (*chamber).first;
                if(id.station() == 1)
                    modeB |= 8;
                if(id.station() == 2)
                    modeB |= 4;
                if(id.station() == 3)
                    modeB |= 2;
                if(id.station() == 4)
                    modeB |= 1;
            }

            //float phi1 = scales_->getPhiScale()->getLowEdge( lt->first.phi_packed() );
            //float eta1 = scales_->getRegionalEtaScale(2)->getCenter( lt->first.eta_packed() );


            // Fill the vectors
            //v_phi             ->push_back(it1.phiValue());  // got warning: L1MuRegionalCand::phiValue requested physical value is invalid
            //v_eta             ->push_back(it1.etaValue());  // got warning: L1MuRegionalCand::etaValue requested physical value is invalid
            //v_pt              ->push_back(it1.ptValue());   // got warning: L1MuRegionalCand::ptValue requested physical value is invalid
            v_charge          ->push_back(it1.chargeValue());
            v_chargeValid     ->push_back(it1.chargeValid());
            v_isFineHalo      ->push_back(it1.isFineHalo());
            v_quality         ->push_back(it1.quality());
            v_typeIdx         ->push_back(it1.type_idx());
            v_bx              ->push_back(it1.bx());
            v_phiPacked       ->push_back(it1.phi_packed());
            v_etaPacked       ->push_back(it1.eta_packed());
            v_ptPacked        ->push_back(it1.pt_packed());
            v_dataword        ->push_back(it1.getDataWord());
            v_rank            ->push_back(it1.rank());
            v_localPhi        ->push_back(it1.localPhi());
            v_me1ID           ->push_back(it1.me1ID());
            v_me2ID           ->push_back(it1.me2ID());
            v_me3ID           ->push_back(it1.me3ID());
            v_me4ID           ->push_back(it1.me4ID());
            v_mb1ID           ->push_back(it1.mb1ID());
            v_iendcap         ->push_back(it1.endcap());
            v_isector         ->push_back(it1.sector());
            v_ptLUTAddress    ->push_back(it1.ptLUTAddress());
            v_frontRear       ->push_back(it1.front_rear());
            v_outputLink      ->push_back(it1.outputLink());
            v_winner          ->push_back(it1.winner());
            //v_deltaPhi12      ->push_back(it1.deltaPhi12());
            //v_deltaPhi23      ->push_back(it1.deltaPhi23());
            //v_addressEta      ->push_back(it1.addressEta());
            //v_mode            ->push_back(it1.mode());
            //v_sign            ->push_back(it1.sign());
            //v_syncErr         ->push_back(it1.synch_err());
            //v_bx0             ->push_back(it1.bx0());
            //v_bc0             ->push_back(it1.bc0());
            //v_me1Tbin         ->push_back(it1.me1Tbin());
            //v_me2Tbin         ->push_back(it1.me2Tbin());
            //v_me3Tbin         ->push_back(it1.me3Tbin());
            //v_me4Tbin         ->push_back(it1.me4Tbin());
            //v_mb1Tbin         ->push_back(it1.mb1Tbin());
            v_stubRefs        ->push_back(myStubRefs);
            v_trkPt           ->push_back(pt);
            v_trkPtOld        ->push_back(ptOld);
            v_trkPtMatt       ->push_back(ptMatt);
            v_trkPtGmt        ->push_back(ptGmt);
            v_trkEta          ->push_back(eta);
            v_trkPhi          ->push_back(phi);
            v_trkMode         ->push_back(mode);
            v_trkModeA        ->push_back(modeA);
            v_trkModeB        ->push_back(modeB);
            v_trkQual         ->push_back(qual);
            v_trkQualA        ->push_back(qualA);
            v_trkQualB        ->push_back(qualB);

            n++;
        }
        *v_size = v_charge->size();

    } else {
        edm::LogError("NtupleLegacyCSCTFTriggerTracks") << "Cannot get the product: " << trackTag_;
    }


    //__________________________________________________________________________
    //iEvent.put(v_phi             , prefix_ + "phi"             + suffix_);
    //iEvent.put(v_eta             , prefix_ + "eta"             + suffix_);
    //iEvent.put(v_pt              , prefix_ + "pt"              + suffix_);
    iEvent.put(v_charge          , prefix_ + "charge"          + suffix_);
    iEvent.put(v_chargeValid     , prefix_ + "chargeValid"     + suffix_);
    iEvent.put(v_isFineHalo      , prefix_ + "isFineHalo"      + suffix_);
    iEvent.put(v_quality         , prefix_ + "quality"         + suffix_);
    iEvent.put(v_typeIdx         , prefix_ + "typeIdx"         + suffix_);
    iEvent.put(v_bx              , prefix_ + "bx"              + suffix_);
    iEvent.put(v_phiPacked       , prefix_ + "phiPacked"       + suffix_);
    iEvent.put(v_etaPacked       , prefix_ + "etaPacked"       + suffix_);
    iEvent.put(v_ptPacked        , prefix_ + "ptPacked"        + suffix_);
    iEvent.put(v_dataword        , prefix_ + "dataword"        + suffix_);
    iEvent.put(v_rank            , prefix_ + "rank"            + suffix_);
    iEvent.put(v_localPhi        , prefix_ + "localPhi"        + suffix_);
    iEvent.put(v_me1ID           , prefix_ + "me1ID"           + suffix_);
    iEvent.put(v_me2ID           , prefix_ + "me2ID"           + suffix_);
    iEvent.put(v_me3ID           , prefix_ + "me3ID"           + suffix_);
    iEvent.put(v_me4ID           , prefix_ + "me4ID"           + suffix_);
    iEvent.put(v_mb1ID           , prefix_ + "mb1ID"           + suffix_);
    iEvent.put(v_iendcap         , prefix_ + "iendcap"         + suffix_);
    iEvent.put(v_isector         , prefix_ + "isector"         + suffix_);
    iEvent.put(v_ptLUTAddress    , prefix_ + "ptLUTAddress"    + suffix_);
    iEvent.put(v_frontRear       , prefix_ + "frontRear"       + suffix_);
    iEvent.put(v_outputLink      , prefix_ + "outputLink"      + suffix_);
    iEvent.put(v_winner          , prefix_ + "winner"          + suffix_);
    //iEvent.put(v_deltaPhi12      , prefix_ + "deltaPhi12"      + suffix_);
    //iEvent.put(v_deltaPhi23      , prefix_ + "deltaPhi23"      + suffix_);
    //iEvent.put(v_addressEta      , prefix_ + "addressEta"      + suffix_);
    //iEvent.put(v_mode            , prefix_ + "mode"            + suffix_);
    //iEvent.put(v_sign            , prefix_ + "sign"            + suffix_);
    //iEvent.put(v_syncErr         , prefix_ + "syncErr"         + suffix_);
    //iEvent.put(v_bx0             , prefix_ + "bx0"             + suffix_);
    //iEvent.put(v_bc0             , prefix_ + "bc0"             + suffix_);
    //iEvent.put(v_me1Tbin         , prefix_ + "me1Tbin"         + suffix_);
    //iEvent.put(v_me2Tbin         , prefix_ + "me2Tbin"         + suffix_);
    //iEvent.put(v_me3Tbin         , prefix_ + "me3Tbin"         + suffix_);
    //iEvent.put(v_me4Tbin         , prefix_ + "me4Tbin"         + suffix_);
    //iEvent.put(v_mb1Tbin         , prefix_ + "mb1Tbin"         + suffix_);
    iEvent.put(v_stubRefs        , prefix_ + "stubRefs"        + suffix_);
    iEvent.put(v_trkPt           , prefix_ + "trkPt"           + suffix_);
    iEvent.put(v_trkPtOld        , prefix_ + "trkPtOld"        + suffix_);
    iEvent.put(v_trkPtMatt       , prefix_ + "trkPtMatt"       + suffix_);
    iEvent.put(v_trkPtGmt        , prefix_ + "trkPtGmt"        + suffix_);
    iEvent.put(v_trkEta          , prefix_ + "trkEta"          + suffix_);
    iEvent.put(v_trkPhi          , prefix_ + "trkPhi"          + suffix_);
    iEvent.put(v_trkMode         , prefix_ + "trkMode"         + suffix_);
    iEvent.put(v_trkModeA        , prefix_ + "trkModeA"        + suffix_);
    iEvent.put(v_trkModeB        , prefix_ + "trkModeB"        + suffix_);
    iEvent.put(v_trkQual         , prefix_ + "trkQual"         + suffix_);
    iEvent.put(v_trkQualA        , prefix_ + "trkQualA"        + suffix_);
    iEvent.put(v_trkQualB        , prefix_ + "trkQualB"        + suffix_);
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);
}
