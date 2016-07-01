#include "L1TMuonSimulations/NtupleTools/interface/NtupleCSCRegionalCandidates.h"

#include "L1Trigger/L1TMuonEndCap/interface/PhiMemoryImage.h"
#include "L1Trigger/L1TMuonEndCap/interface/EmulatorClasses.h"


NtupleCSCRegionalCandidates::NtupleCSCRegionalCandidates(const edm::ParameterSet& iConfig) :
  corrlctTag_(iConfig.getParameter<edm::InputTag>("corrlctTag")),
  trackTag_(iConfig.getParameter<edm::InputTag>("trackTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    corrlctToken_ = consumes<CSCCorrelatedLCTDigiCollection>(corrlctTag_);
    trackToken_ = consumes<l1t::RegionalMuonCandBxCollection>(trackTag_);

    produces<std::vector<int> >                (prefix_ + "hwPt"             + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwPhi"            + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwEta"            + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwSign"           + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwSignValid"      + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwQual"           + suffix_);
    produces<std::vector<int> >                (prefix_ + "link"             + suffix_);
    produces<std::vector<int> >                (prefix_ + "processor"        + suffix_);
    produces<std::vector<uint16_t> >           (prefix_ + "trackFinderType"  + suffix_);
    produces<std::vector<bool> >               (prefix_ + "hwHF"             + suffix_);
    produces<std::vector<ULong64_t> >          (prefix_ + "dataword"         + suffix_);
    //produces<std::vector<std::map<int,int> > > (prefix_ + "trackAddress"     + suffix_);
    produces<unsigned>                         (prefix_ + "size"             + suffix_);
}

NtupleCSCRegionalCandidates::~NtupleCSCRegionalCandidates() {}

void NtupleCSCRegionalCandidates::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    // regional muon candidates
    std::auto_ptr<std::vector<int> >                v_hwPt            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwPhi           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwEta           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwSign          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwSignValid     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwQual          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_link            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_processor       (new std::vector<int>());
    std::auto_ptr<std::vector<uint16_t> >           v_trackFinderType (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<bool> >               v_hwHF            (new std::vector<bool>());
    std::auto_ptr<std::vector<ULong64_t> >          v_dataword        (new std::vector<ULong64_t>());
    //std::auto_ptr<std::vector<std::map<int,int> > > v_trackAddress    (new std::vector<std::map<int,int> >());
    std::auto_ptr<unsigned>                         v_size            (new unsigned(0));

    // _________________________________________________________________________
    edm::Handle<l1t::RegionalMuonCandBxCollection> tracks;
    //iEvent.getByLabel(trackTag_, tracks);
    if (!trackToken_.isUninitialized())
        iEvent.getByToken(trackToken_, tracks);

    if (tracks.isValid()) {
        //edm::LogInfo("NtupleCSCRegionalCandidates") << "Size: " << tracks->size();
        edm::LogInfo("NtupleCSCRegionalCandidates") << "Size: ??";

        unsigned n = 0;
        for (l1t::RegionalMuonCandBxCollection::const_iterator it = tracks->begin(); it != tracks->end(); ++it) {
            v_hwPt           ->push_back(it->hwPt());
            v_hwPhi          ->push_back(it->hwPhi());
            v_hwEta          ->push_back(it->hwEta());
            v_hwSign         ->push_back(it->hwSign());
            v_hwSignValid    ->push_back(it->hwSignValid());
            v_hwQual         ->push_back(it->hwQual());
            v_link           ->push_back(it->link());
            v_processor      ->push_back(it->processor());
            v_trackFinderType->push_back(it->trackFinderType());
            v_hwHF           ->push_back(it->hwHF());
            v_dataword       ->push_back(it->dataword());

            n++;
        }
        *v_size = v_hwPt->size();

    } else {
        edm::LogError("NtupleCSCRegionalCandidates") << "Cannot get the product: " << trackTag_;
    }

    //__________________________________________________________________________
    iEvent.put(v_hwPt            , prefix_ + "hwPt"             + suffix_);
    iEvent.put(v_hwPhi           , prefix_ + "hwPhi"            + suffix_);
    iEvent.put(v_hwEta           , prefix_ + "hwEta"            + suffix_);
    iEvent.put(v_hwSign          , prefix_ + "hwSign"           + suffix_);
    iEvent.put(v_hwSignValid     , prefix_ + "hwSignValid"      + suffix_);
    iEvent.put(v_hwQual          , prefix_ + "hwQual"           + suffix_);
    iEvent.put(v_link            , prefix_ + "link"             + suffix_);
    iEvent.put(v_processor       , prefix_ + "processor"        + suffix_);
    iEvent.put(v_trackFinderType , prefix_ + "trackFinderType"  + suffix_);
    iEvent.put(v_hwHF            , prefix_ + "hwHF"             + suffix_);
    iEvent.put(v_dataword        , prefix_ + "dataword"         + suffix_);
    //iEvent.put(v_trackAddress    , prefix_ + "trackAddress"     + suffix_);
    iEvent.put(v_size            , prefix_ + "size"             + suffix_);
}
