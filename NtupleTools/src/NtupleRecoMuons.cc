#include "L1TMuonSimulations/NtupleTools/interface/NtupleRecoMuons.h"


// Reference:
//   https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId

NtupleRecoMuons::NtupleRecoMuons(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  vertexTag_(iConfig.getParameter<edm::InputTag>("vertexTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    token_ = consumes<reco::MuonCollection>(inputTag_);
    vertexToken_ = consumes<reco::VertexCollection>(vertexTag_);

    produces<std::vector<float> >   (prefix_ + "px"                + suffix_);
    produces<std::vector<float> >   (prefix_ + "py"                + suffix_);
    produces<std::vector<float> >   (prefix_ + "pz"                + suffix_);
    //produces<std::vector<float> >   (prefix_ + "E"                 + suffix_);
    produces<std::vector<float> >   (prefix_ + "pt"                + suffix_);
    produces<std::vector<float> >   (prefix_ + "eta"               + suffix_);
    produces<std::vector<float> >   (prefix_ + "phi"               + suffix_);
    //produces<std::vector<float> >   (prefix_ + "M"                 + suffix_);
    //produces<std::vector<float> >   (prefix_ + "vx"                + suffix_);
    //produces<std::vector<float> >   (prefix_ + "vy"                + suffix_);
    //produces<std::vector<float> >   (prefix_ + "vz"                + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "charge"            + suffix_);
    produces<std::vector<float> >   (prefix_ + "staPt"             + suffix_);
    produces<std::vector<float> >   (prefix_ + "staEta"            + suffix_);
    produces<std::vector<float> >   (prefix_ + "staPhi"            + suffix_);
    produces<std::vector<float> >   (prefix_ + "trkPt"             + suffix_);
    produces<std::vector<float> >   (prefix_ + "trkEta"            + suffix_);
    produces<std::vector<float> >   (prefix_ + "trkPhi"            + suffix_);
    produces<std::vector<float> >   (prefix_ + "pfPt"              + suffix_);
    produces<std::vector<float> >   (prefix_ + "pfEta"             + suffix_);
    produces<std::vector<float> >   (prefix_ + "pfPhi"             + suffix_);
    produces<std::vector<float> >   (prefix_ + "trkIso"            + suffix_);
    produces<std::vector<float> >   (prefix_ + "ecalIso"           + suffix_);
    produces<std::vector<float> >   (prefix_ + "hcalIso"           + suffix_);
    produces<std::vector<float> >   (prefix_ + "chHadIso"          + suffix_);
    produces<std::vector<float> >   (prefix_ + "neuHadIso"         + suffix_);
    produces<std::vector<float> >   (prefix_ + "gammaIso"          + suffix_);
    produces<std::vector<float> >   (prefix_ + "puIso"             + suffix_);
    produces<std::vector<float> >   (prefix_ + "d0"                + suffix_);
    produces<std::vector<float> >   (prefix_ + "dz"                + suffix_);
    produces<std::vector<float> >   (prefix_ + "normChi2"          + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nValidTrackerHits" + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nValidPixelHits"   + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nTrackerLayers"    + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nPixelLayers"      + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nValidMuonHits"    + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nValidDTHits"      + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nValidCSCHits"     + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nValidRPCHits"     + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nValidGEMHits"     + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nMuonStations"     + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nDTStations"       + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nCSCStations"      + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nRPCStations"      + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nGEMStations"      + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nMatches"          + suffix_);
    produces<std::vector<int16_t> > (prefix_ + "nMatchedStations"  + suffix_);
    produces<std::vector<bool> >    (prefix_ + "isPFMuon"          + suffix_);
    produces<std::vector<bool> >    (prefix_ + "isGlobalMuon"      + suffix_);
    produces<std::vector<bool> >    (prefix_ + "isTrackerMuon"     + suffix_);
    produces<std::vector<bool> >    (prefix_ + "isStandAloneMuon"  + suffix_);
    produces<std::vector<bool> >    (prefix_ + "isTightMuon"       + suffix_);
    produces<std::vector<bool> >    (prefix_ + "isLooseMuon"       + suffix_);
    produces<unsigned>              (prefix_ + "size"              + suffix_);
}

NtupleRecoMuons::~NtupleRecoMuons() {}

void NtupleRecoMuons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >   v_px               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_py               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_pz               (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >   v_E                (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_pt               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_eta              (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_phi              (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >   v_M                (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >   v_vx               (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >   v_vy               (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >   v_vz               (new std::vector<float>());
    std::auto_ptr<std::vector<int16_t> > v_charge           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<float> >   v_staPt            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_staEta           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_staPhi           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_trkPt            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_trkEta           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_trkPhi           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_pfPt             (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_pfEta            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_pfPhi            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_trkIso           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_ecalIso          (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_hcalIso          (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_chHadIso         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_neuHadIso        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_gammaIso         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_puIso            (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_d0               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_dz               (new std::vector<float>());
    std::auto_ptr<std::vector<float> >   v_normChi2         (new std::vector<float>());
    std::auto_ptr<std::vector<int16_t> > v_nValidTrackerHits(new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nValidPixelHits  (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nTrackerLayers   (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nPixelLayers     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nValidMuonHits   (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nValidDTHits     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nValidCSCHits    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nValidRPCHits    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nValidGEMHits    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nMuonStations    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nDTStations      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nCSCStations     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nRPCStations     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nGEMStations     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nMatches         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> > v_nMatchedStations (new std::vector<int16_t>());
    std::auto_ptr<std::vector<bool> >    v_isPFMuon         (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >    v_isGlobalMuon     (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >    v_isTrackerMuon    (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >    v_isStandAloneMuon (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >    v_isTightMuon      (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >    v_isLooseMuon      (new std::vector<bool>());
    std::auto_ptr<unsigned>              v_size             (new unsigned(0));

    //__________________________________________________________________________
    reco::Vertex pv;
    edm::Handle<reco::VertexCollection> vertices;
    //iEvent.getByLabel(vertexTag_, vertices);
    if (!vertexToken_.isUninitialized())
        iEvent.getByToken(vertexToken_, vertices);

    if (vertices.isValid() && !vertices->empty()) {
        pv = vertices->front();
    } else {
        edm::LogError("NtupleRecoMuons") << "Cannot get the product: " << vertexTag_;
    }

    //__________________________________________________________________________


    edm::Handle<reco::MuonCollection> parts;
    //iEvent.getByLabel(inputTag_, parts);
    if (!token_.isUninitialized())
        iEvent.getByToken(token_, parts);

    if (parts.isValid()) {
        edm::LogInfo("NtupleRecoMuons") << "Size: " << parts->size();

        unsigned n = 0;
        for (reco::MuonCollection::const_iterator it = parts->begin(); it != parts->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!selector_(*it))
                continue;

            // Fill the vectors
            v_px               ->push_back(it->muonBestTrack()->px());
            v_py               ->push_back(it->muonBestTrack()->py());
            v_pz               ->push_back(it->muonBestTrack()->pz());
            //v_E                ->push_back(it->muonBestTrack()->energy());
            v_pt               ->push_back(it->muonBestTrack()->pt());
            v_eta              ->push_back(it->muonBestTrack()->eta());
            v_phi              ->push_back(it->muonBestTrack()->phi());
            //v_M                ->push_back(it->muonBestTrack()->mass());
            //v_vx               ->push_back(it->muonBestTrack()->vx());
            //v_vy               ->push_back(it->muonBestTrack()->vy());
            //v_vz               ->push_back(it->muonBestTrack()->vz());
            v_charge           ->push_back(it->muonBestTrack()->charge());
            v_staPt            ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->pt()  : 0.);
            v_staEta           ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->eta() : 0.);
            v_staPhi           ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->phi() : 0.);
            v_trkPt            ->push_back(it->innerTrack().isNonnull()     ? it->innerTrack()->pt()  : 0.);
            v_trkEta           ->push_back(it->innerTrack().isNonnull()     ? it->innerTrack()->eta() : 0.);
            v_trkPhi           ->push_back(it->innerTrack().isNonnull()     ? it->innerTrack()->phi() : 0.);
            v_pfPt             ->push_back(it->pfP4().pt());
            v_pfEta            ->push_back(it->pfP4().eta());
            v_pfPhi            ->push_back(it->pfP4().phi());
            v_trkIso           ->push_back(it->isolationR03().sumPt);
            v_ecalIso          ->push_back(it->isolationR03().emEt);
            v_hcalIso          ->push_back(it->isolationR03().hadEt);
            v_chHadIso         ->push_back(it->pfIsolationR04().sumChargedHadronPt);
            v_neuHadIso        ->push_back(it->pfIsolationR04().sumPhotonEt);
            v_gammaIso         ->push_back(it->pfIsolationR04().sumNeutralHadronEt);
            v_puIso            ->push_back(it->pfIsolationR04().sumPUPt);
            v_d0               ->push_back(it->innerTrack().isNonnull()     ? (-1)*(it->innerTrack()->dxy(pv.position())) : 0.);  // note: d0 = -dxy
            v_dz               ->push_back(it->innerTrack().isNonnull()     ? it->innerTrack()->dz(pv.position()) : 0.);
            v_normChi2         ->push_back(it->globalTrack().isNonnull()    ? it->globalTrack()-> normalizedChi2() : -999.);
            v_nValidTrackerHits->push_back(it->innerTrack().isNonnull()     ? it->innerTrack()->hitPattern().numberOfValidTrackerHits() : 0);
            v_nValidPixelHits  ->push_back(it->innerTrack().isNonnull()     ? it->innerTrack()->hitPattern().numberOfValidPixelHits() : 0);
            v_nTrackerLayers   ->push_back(it->innerTrack().isNonnull()     ? it->innerTrack()->hitPattern().trackerLayersWithMeasurement() : 0);
            v_nPixelLayers     ->push_back(it->innerTrack().isNonnull()     ? it->innerTrack()->hitPattern().pixelLayersWithMeasurement() : 0);
            v_nValidMuonHits   ->push_back(it->globalTrack().isNonnull()    ? it->globalTrack()->hitPattern().numberOfValidMuonHits() : 0);
            v_nValidDTHits     ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->hitPattern().numberOfValidMuonDTHits() : 0);
            v_nValidCSCHits    ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->hitPattern().numberOfValidMuonCSCHits() : 0);
            v_nValidRPCHits    ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->hitPattern().numberOfValidMuonRPCHits() : 0);
            v_nValidGEMHits    ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->hitPattern().numberOfValidMuonGEMHits() : 0);
            v_nMuonStations    ->push_back(it->globalTrack().isNonnull()    ? it->globalTrack()->hitPattern().muonStationsWithValidHits() : 0);
            v_nDTStations      ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->hitPattern().dtStationsWithValidHits() : 0);
            v_nCSCStations     ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->hitPattern().cscStationsWithValidHits() : 0);
            v_nRPCStations     ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->hitPattern().rpcStationsWithValidHits() : 0);
            v_nGEMStations     ->push_back(it->standAloneMuon().isNonnull() ? it->standAloneMuon()->hitPattern().gemStationsWithValidHits() : 0);
            v_nMatches         ->push_back(it->numberOfMatches());
            v_nMatchedStations ->push_back(it->numberOfMatchedStations());
            v_isPFMuon         ->push_back(it->isPFMuon());
            v_isGlobalMuon     ->push_back(it->isGlobalMuon());
            v_isTrackerMuon    ->push_back(it->isTrackerMuon());
            v_isStandAloneMuon ->push_back(it->isStandAloneMuon());
            v_isTightMuon      ->push_back(muon::isTightMuon(*it, pv));
            v_isLooseMuon      ->push_back(muon::isLooseMuon(*it));

            n++;
        }
        *v_size = v_px->size();

    } else {
        edm::LogError("NtupleRecoMuons") << "Cannot get the product: " << inputTag_;
    }

    //__________________________________________________________________________
    iEvent.put(v_px               , prefix_ + "px"                + suffix_);
    iEvent.put(v_py               , prefix_ + "py"                + suffix_);
    iEvent.put(v_pz               , prefix_ + "pz"                + suffix_);
    //iEvent.put(v_E                , prefix_ + "E"                 + suffix_);
    iEvent.put(v_pt               , prefix_ + "pt"                + suffix_);
    iEvent.put(v_eta              , prefix_ + "eta"               + suffix_);
    iEvent.put(v_phi              , prefix_ + "phi"               + suffix_);
    //iEvent.put(v_M                , prefix_ + "M"                 + suffix_);
    //iEvent.put(v_vx               , prefix_ + "vx"                + suffix_);
    //iEvent.put(v_vy               , prefix_ + "vy"                + suffix_);
    //iEvent.put(v_vz               , prefix_ + "vz"                + suffix_);
    iEvent.put(v_charge           , prefix_ + "charge"            + suffix_);
    iEvent.put(v_staPt            , prefix_ + "staPt"             + suffix_);
    iEvent.put(v_staEta           , prefix_ + "staEta"            + suffix_);
    iEvent.put(v_staPhi           , prefix_ + "staPhi"            + suffix_);
    iEvent.put(v_trkPt            , prefix_ + "trkPt"             + suffix_);
    iEvent.put(v_trkEta           , prefix_ + "trkEta"            + suffix_);
    iEvent.put(v_trkPhi           , prefix_ + "trkPhi"            + suffix_);
    iEvent.put(v_pfPt             , prefix_ + "pfPt"              + suffix_);
    iEvent.put(v_pfEta            , prefix_ + "pfEta"             + suffix_);
    iEvent.put(v_pfPhi            , prefix_ + "pfPhi"             + suffix_);
    iEvent.put(v_trkIso           , prefix_ + "trkIso"            + suffix_);
    iEvent.put(v_ecalIso          , prefix_ + "ecalIso"           + suffix_);
    iEvent.put(v_hcalIso          , prefix_ + "hcalIso"           + suffix_);
    iEvent.put(v_chHadIso         , prefix_ + "chHadIso"          + suffix_);
    iEvent.put(v_neuHadIso        , prefix_ + "neuHadIso"         + suffix_);
    iEvent.put(v_gammaIso         , prefix_ + "gammaIso"          + suffix_);
    iEvent.put(v_puIso            , prefix_ + "puIso"             + suffix_);
    iEvent.put(v_d0               , prefix_ + "d0"                + suffix_);
    iEvent.put(v_dz               , prefix_ + "dz"                + suffix_);
    iEvent.put(v_normChi2         , prefix_ + "normChi2"          + suffix_);
    iEvent.put(v_nValidTrackerHits, prefix_ + "nValidTrackerHits" + suffix_);
    iEvent.put(v_nValidPixelHits  , prefix_ + "nValidPixelHits"   + suffix_);
    iEvent.put(v_nTrackerLayers   , prefix_ + "nTrackerLayers"    + suffix_);
    iEvent.put(v_nPixelLayers     , prefix_ + "nPixelLayers"      + suffix_);
    iEvent.put(v_nValidMuonHits   , prefix_ + "nValidMuonHits"    + suffix_);
    iEvent.put(v_nValidDTHits     , prefix_ + "nValidDTHits"      + suffix_);
    iEvent.put(v_nValidCSCHits    , prefix_ + "nValidCSCHits"     + suffix_);
    iEvent.put(v_nValidRPCHits    , prefix_ + "nValidRPCHits"     + suffix_);
    iEvent.put(v_nValidGEMHits    , prefix_ + "nValidGEMHits"     + suffix_);
    iEvent.put(v_nMuonStations    , prefix_ + "nMuonStations"     + suffix_);
    iEvent.put(v_nDTStations      , prefix_ + "nDTStations"       + suffix_);
    iEvent.put(v_nCSCStations     , prefix_ + "nCSCStations"      + suffix_);
    iEvent.put(v_nRPCStations     , prefix_ + "nRPCStations"      + suffix_);
    iEvent.put(v_nGEMStations     , prefix_ + "nGEMStations"      + suffix_);
    iEvent.put(v_nMatches         , prefix_ + "nMatches"          + suffix_);
    iEvent.put(v_nMatchedStations , prefix_ + "nMatchedStations"  + suffix_);
    iEvent.put(v_isPFMuon         , prefix_ + "isPFMuon"          + suffix_);
    iEvent.put(v_isGlobalMuon     , prefix_ + "isGlobalMuon"      + suffix_);
    iEvent.put(v_isTrackerMuon    , prefix_ + "isTrackerMuon"     + suffix_);
    iEvent.put(v_isStandAloneMuon , prefix_ + "isStandAloneMuon"  + suffix_);
    iEvent.put(v_isTightMuon      , prefix_ + "isTightMuon"       + suffix_);
    iEvent.put(v_isLooseMuon      , prefix_ + "isLooseMuon"       + suffix_);
    iEvent.put(v_size             , prefix_ + "size"              + suffix_);
}
