#include "L1TMuonSimulations/NtupleTools/interface/NtupleL1TMuons.h"


NtupleL1TMuons::NtupleL1TMuons(const edm::ParameterSet& iConfig) :
  trackTag_(iConfig.getParameter<edm::InputTag>("trackTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    trackToken_ = consumes<l1t::MuonBxCollection>(trackTag_);

    produces<std::vector<float> >              (prefix_ + "pt"               + suffix_);
    produces<std::vector<float> >              (prefix_ + "eta"              + suffix_);
    produces<std::vector<float> >              (prefix_ + "phi"              + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwPt"             + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwEta"            + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwPhi"            + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwQual"           + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwCharge"         + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwChargeValid"    + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwIso"            + suffix_);
    produces<std::vector<int> >                (prefix_ + "tfMuonIndex"      + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwTag"            + suffix_);
    produces<std::vector<bool> >               (prefix_ + "debug"            + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwIsoSum"         + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwDPhiExtra"      + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwDEtaExtra"      + suffix_);
    produces<std::vector<int> >                (prefix_ + "hwRank"           + suffix_);
    produces<std::vector<int> >                (prefix_ + "bx"               + suffix_);
    produces<unsigned>                         (prefix_ + "size"             + suffix_);
}

NtupleL1TMuons::~NtupleL1TMuons() {}

void NtupleL1TMuons::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >              v_pt              (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              v_eta             (new std::vector<float>());
    std::auto_ptr<std::vector<float> >              v_phi             (new std::vector<float>());
    std::auto_ptr<std::vector<int> >                v_hwPt            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwEta           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwPhi           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwQual          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwCharge        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwChargeValid   (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwIso           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_tfMuonIndex     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwTag           (new std::vector<int>());
    std::auto_ptr<std::vector<bool> >               v_debug           (new std::vector<bool>());
    std::auto_ptr<std::vector<int> >                v_hwIsoSum        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwDPhiExtra     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwDEtaExtra     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_hwRank          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                v_bx              (new std::vector<int>());
    std::auto_ptr<unsigned>                         v_size            (new unsigned(0));

    // _________________________________________________________________________
    edm::Handle<l1t::MuonBxCollection> tracks;
    //iEvent.getByLabel(trackTag_, tracks);
    if (!trackToken_.isUninitialized())
        iEvent.getByToken(trackToken_, tracks);

    if (tracks.isValid()) {
        edm::LogInfo("NtupleL1TMuons") << "Size: " << tracks->size();

        unsigned n = 0;
        for (int ibx = tracks->getFirstBX(); ibx != tracks->getLastBX()+1; ++ibx) {

            for (l1t::MuonBxCollection::const_iterator it = tracks->begin(ibx); it != tracks->end(ibx); ++it) {

                if (n >= maxN_)
                    break;
                if (!selector_(*it))
                    continue;


                // Fill the vectors
                v_pt             ->push_back(it->pt());
                v_eta            ->push_back(it->eta());
                v_phi            ->push_back(it->phi());
                v_hwPt           ->push_back(it->hwPt());
                v_hwEta          ->push_back(it->hwEta());
                v_hwPhi          ->push_back(it->hwPhi());
                v_hwQual         ->push_back(it->hwQual());
                v_hwCharge       ->push_back(it->hwCharge());
                v_hwChargeValid  ->push_back(it->hwChargeValid());
                v_hwIso          ->push_back(it->hwIso());
                v_tfMuonIndex    ->push_back(it->tfMuonIndex());
                v_hwTag          ->push_back(it->hwTag());
                v_debug          ->push_back(it->debug());
                v_hwIsoSum       ->push_back(it->hwIsoSum());
                v_hwDPhiExtra    ->push_back(it->hwDPhiExtra());
                v_hwDEtaExtra    ->push_back(it->hwDEtaExtra());
                v_hwRank         ->push_back(it->hwRank());
                v_bx             ->push_back(ibx);

                n++;
            }
        }
        *v_size = v_hwPt->size();

    } else {
        edm::LogError("NtupleL1TMuons") << "Cannot get the product: " << trackTag_;
    }

    //__________________________________________________________________________
    iEvent.put(v_pt              , prefix_ + "pt"               + suffix_);
    iEvent.put(v_eta             , prefix_ + "eta"              + suffix_);
    iEvent.put(v_phi             , prefix_ + "phi"              + suffix_);
    iEvent.put(v_hwPt            , prefix_ + "hwPt"             + suffix_);
    iEvent.put(v_hwEta           , prefix_ + "hwEta"            + suffix_);
    iEvent.put(v_hwPhi           , prefix_ + "hwPhi"            + suffix_);
    iEvent.put(v_hwQual          , prefix_ + "hwQual"           + suffix_);
    iEvent.put(v_hwCharge        , prefix_ + "hwCharge"         + suffix_);
    iEvent.put(v_hwChargeValid   , prefix_ + "hwChargeValid"    + suffix_);
    iEvent.put(v_hwIso           , prefix_ + "hwIso"            + suffix_);
    iEvent.put(v_tfMuonIndex     , prefix_ + "tfMuonIndex"      + suffix_);
    iEvent.put(v_hwTag           , prefix_ + "hwTag"            + suffix_);
    iEvent.put(v_debug           , prefix_ + "debug"            + suffix_);
    iEvent.put(v_hwIsoSum        , prefix_ + "hwIsoSum"         + suffix_);
    iEvent.put(v_hwDPhiExtra     , prefix_ + "hwDPhiExtra"      + suffix_);
    iEvent.put(v_hwDEtaExtra     , prefix_ + "hwDEtaExtra"      + suffix_);
    iEvent.put(v_hwRank          , prefix_ + "hwRank"           + suffix_);
    iEvent.put(v_bx              , prefix_ + "bx"               + suffix_);
    iEvent.put(v_size            , prefix_ + "size"             + suffix_);
}
