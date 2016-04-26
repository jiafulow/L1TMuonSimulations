#include "L1TMuonSimulations/NtupleTools/interface/NtupleCSCRegionalCandidates.h"

#include "L1Trigger/L1TMuonEndCap/interface/PhiMemoryImage.h"
#include "L1Trigger/L1TMuonEndCap/interface/EmulatorClasses.h"

#define NUM_SECTORS 12
using namespace L1TMuon;

#include "L1Trigger/L1TMuonEndCap/interface/PrimitiveConverter.h"
#include "L1Trigger/L1TMuonEndCap/interface/BXAnalyzer.h"
#include "L1Trigger/L1TMuonEndCap/interface/ZoneCreation.h"
#include "L1Trigger/L1TMuonEndCap/interface/PatternRecognition.h"
#include "L1Trigger/L1TMuonEndCap/interface/SortSector.h"
#include "L1Trigger/L1TMuonEndCap/interface/Matching.h"
#include "L1Trigger/L1TMuonEndCap/interface/Deltas.h"
#include "L1Trigger/L1TMuonEndCap/interface/BestTracks.h"
#include "L1Trigger/L1TMuonEndCap/interface/PtAssignment.h"
#include "L1Trigger/L1TMuonEndCap/interface/MakeRegionalCand.h"


NtupleCSCRegionalCandidates::NtupleCSCRegionalCandidates(const edm::ParameterSet& iConfig) :
  corrlctTag_(iConfig.getParameter<edm::InputTag>("corrlctTag")),
  trackTag_(iConfig.getParameter<edm::InputTag>("trackTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    corrlctToken_ = consumes<CSCCorrelatedLCTDigiCollection>(corrlctTag_);
    trackToken_ = consumes<l1t::RegionalMuonCandBxCollection>(trackTag_);

    // convHits
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "phi"              + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "theta"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "phit"             + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "phzvl"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "station"          + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "sub"              + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "id"               + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "quality"          + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "pattern"          + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "wire"             + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "strip"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "zhit"             + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "bx"               + suffix_);
    produces<std::vector<std::vector<int> > >      (prefix_ + "convHits@" + "zoneCont"         + suffix_);
    produces<std::vector<std::vector<int> > >      (prefix_ + "convHits@" + "bxgroupCont"      + suffix_);
    produces<std::vector<int> >                    (prefix_ + "convHits@" + "isector"          + suffix_);
    produces<unsigned>                             (prefix_ + "convHits@" + "size"             + suffix_);
    // zones
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "zones@"    + "convHitRefs"      + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "zones@"    + "zhitStations"     + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "zones@"    + "zhitSuperstrips"  + suffix_);
    produces<std::vector<int> >                    (prefix_ + "zones@"    + "izone"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "zones@"    + "ibxgroup"         + suffix_);
    produces<std::vector<int> >                    (prefix_ + "zones@"    + "isector"          + suffix_);
    produces<unsigned>                             (prefix_ + "zones@"    + "size"             + suffix_);
    // roads
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "rank"             + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "layer"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "straightness"     + suffix_);
    produces<std::vector<bool> >                   (prefix_ + "roads@"    + "isDuplicate"      + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "roads@"    + "convHitRefs"      + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "roads@"    + "dedupConvHitRefs" + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "roads@"    + "phiConvHitRefs"   + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "roads@"    + "thetaConvHitRefs" + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "dphiMask"         + suffix_);  // FIXME
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "dphi1"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "dphi2"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "dthetaMask"       + suffix_);  // FIXME
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "dtheta1"          + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "dtheta2"          + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "precisePhi"       + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "preciseTheta"     + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "preciseRank"      + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "ibesttrack"       + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "iwinner"          + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "isuperstrip"      + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "izone"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "ibxgroup"         + suffix_);
    produces<std::vector<int> >                    (prefix_ + "roads@"    + "isector"          + suffix_);
    produces<unsigned>                             (prefix_ + "roads@"    + "size"             + suffix_);
    // internal tracks
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "phi"              + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "theta"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "rank"             + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "dphi1"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "dphi2"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "dtheta1"          + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "dtheta2"          + suffix_);
    produces<std::vector<std::vector<int> > >      (prefix_ + "itracks@"  + "phis"             + suffix_);
    produces<std::vector<std::vector<int> > >      (prefix_ + "itracks@"  + "thetas"           + suffix_);
    produces<std::vector<float> >                  (prefix_ + "itracks@"  + "pt"               + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "me1address"       + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "me2address"       + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "combaddress"      + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "mode"             + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "ebx"              + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "sebx"             + suffix_);
    produces<std::vector<bool> >                   (prefix_ + "itracks@"  + "isEndcap"         + suffix_);
    produces<std::vector<float> >                  (prefix_ + "itracks@"  + "globalPhi"        + suffix_);
    produces<std::vector<float> >                  (prefix_ + "itracks@"  + "globalTheta"      + suffix_);
    produces<std::vector<float> >                  (prefix_ + "itracks@"  + "globalEta"        + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "ibesttrack"       + suffix_);
    produces<std::vector<int> >                    (prefix_ + "itracks@"  + "isector"          + suffix_);
    produces<std::vector<unsigned> >               (prefix_ + "itracks@"  + "roadRef"          + suffix_);
    produces<unsigned>                             (prefix_ + "itracks@"  + "size"             + suffix_);
    // regional muon candidates
    produces<std::vector<int> >                    (prefix_ + "emuons@"   + "hwPt"             + suffix_);
    produces<std::vector<int> >                    (prefix_ + "emuons@"   + "hwPhi"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "emuons@"   + "hwEta"            + suffix_);
    produces<std::vector<int> >                    (prefix_ + "emuons@"   + "hwSign"           + suffix_);
    produces<std::vector<int> >                    (prefix_ + "emuons@"   + "hwSignValid"      + suffix_);
    produces<std::vector<int> >                    (prefix_ + "emuons@"   + "hwQual"           + suffix_);
    produces<std::vector<int> >                    (prefix_ + "emuons@"   + "link"             + suffix_);
    produces<std::vector<int> >                    (prefix_ + "emuons@"   + "processor"        + suffix_);
    produces<std::vector<uint16_t> >               (prefix_ + "emuons@"   + "trackFinderType"  + suffix_);
    produces<std::vector<bool> >                   (prefix_ + "emuons@"   + "hwHF"             + suffix_);
    produces<std::vector<ULong64_t> >              (prefix_ + "emuons@"   + "dataword"         + suffix_);
    //produces<std::vector<std::map<int,int> > >     (prefix_ + "emuons@"   + "trackAddress"     + suffix_);
    produces<unsigned>                             (prefix_ + "emuons@"   + "size"             + suffix_);
}

NtupleCSCRegionalCandidates::~NtupleCSCRegionalCandidates() {}

namespace {
typedef std::tuple<int, int, int, int> ConvHitKey;
ConvHitKey make_convHitKey(int sector, int id, int strip, int wire) {
    return std::make_tuple(sector, id, strip, wire);
}

typedef std::tuple<int, int, int, int> RoadKey;
RoadKey make_roadKey(int sector, int bxgroup, int zone, int superstrip) {
    return std::make_tuple(sector, bxgroup, zone, superstrip);
}

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
}  // end anonymous namespace

void NtupleCSCRegionalCandidates::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    // convHits
    std::auto_ptr<std::vector<int> >                    v1_phi             (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_theta           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_phit            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_phzvl           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_station         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_sub             (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_id              (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_quality         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_pattern         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_wire            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_strip           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_zhit            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v1_bx              (new std::vector<int>());
    std::auto_ptr<std::vector<std::vector<int> > >      v1_zoneCont        (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<int> > >      v1_bxgroupCont     (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<int> >                    v1_isector         (new std::vector<int>());
    std::auto_ptr<unsigned>                             v1_size            (new unsigned(0));
    // zones
    std::auto_ptr<std::vector<std::vector<unsigned> > > v2_convHitRefs     (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v2_zhitStations    (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v2_zhitSuperstrips (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<int> >                    v2_izone           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v2_ibxgroup        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v2_isector         (new std::vector<int>());
    std::auto_ptr<unsigned>                             v2_size            (new unsigned(0));
    // roads
    std::auto_ptr<std::vector<int> >                    v3_rank            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_layer           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_straightness    (new std::vector<int>());
    std::auto_ptr<std::vector<bool> >                   v3_isDuplicate     (new std::vector<bool>());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v3_convHitRefs     (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v3_dedupConvHitRefs(new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v3_phiConvHitRefs  (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v3_thetaConvHitRefs(new std::vector<std::vector<unsigned> >());
    std::auto_ptr<std::vector<int> >                    v3_dphiMask        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_dphi1           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_dphi2           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_dthetaMask      (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_dtheta1         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_dtheta2         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_precisePhi      (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_preciseTheta    (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_preciseRank     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_ibesttrack      (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_iwinner         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_isuperstrip     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_izone           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_ibxgroup        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v3_isector         (new std::vector<int>());
    std::auto_ptr<unsigned>                             v3_size            (new unsigned(0));
    // internal tracks
    std::auto_ptr<std::vector<int> >                    v4_phi             (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_theta           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_rank            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_dphi1           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_dphi2           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_dtheta1         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_dtheta2         (new std::vector<int>());
    std::auto_ptr<std::vector<std::vector<int> > >      v4_phis            (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<int> > >      v4_thetas          (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<float> >                  v4_pt              (new std::vector<float>());
    std::auto_ptr<std::vector<int> >                    v4_me1address      (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_me2address      (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_combaddress     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_mode            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_ebx             (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_sebx            (new std::vector<int>());
    std::auto_ptr<std::vector<bool> >                   v4_isEndcap        (new std::vector<bool>());
    std::auto_ptr<std::vector<float> >                  v4_globalPhi       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v4_globalTheta     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >                  v4_globalEta       (new std::vector<float>());
    std::auto_ptr<std::vector<int> >                    v4_ibesttrack      (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v4_isector         (new std::vector<int>());
    std::auto_ptr<std::vector<unsigned> >               v4_roadRef         (new std::vector<unsigned>());
    std::auto_ptr<unsigned>                             v4_size            (new unsigned(0));
    // regional muon candidates
    std::auto_ptr<std::vector<int> >                    v5_hwPt            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v5_hwPhi           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v5_hwEta           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v5_hwSign          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v5_hwSignValid     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v5_hwQual          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v5_link            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >                    v5_processor       (new std::vector<int>());
    std::auto_ptr<std::vector<uint16_t> >               v5_trackFinderType (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<bool> >                   v5_hwHF            (new std::vector<bool>());
    std::auto_ptr<std::vector<ULong64_t> >              v5_dataword        (new std::vector<ULong64_t>());
    //std::auto_ptr<std::vector<std::map<int,int> > >     v5_trackAddress    (new std::vector<std::map<int,int> >());
    std::auto_ptr<unsigned>                             v5_size            (new unsigned(0));
    //std::auto_ptr<unsigned>                        v_size            (new unsigned(0));

    std::auto_ptr<l1t::RegionalMuonCandBxCollection> OutputCands(  // MODIFIED
        new l1t::RegionalMuonCandBxCollection);                    // MODIFIED

    std::map<ConvHitKey, unsigned>  convHitRefMap;
    std::map<RoadKey, unsigned>     roadRefMap;


    //__________________________________________________________________________

    edm::Handle<CSCCorrelatedLCTDigiCollection> corrlcts;
    //iEvent.getByLabel(corrlctTag_, corrlcts);
    if (!corrlctToken_.isUninitialized())
        iEvent.getByToken(corrlctToken_, corrlcts);

    if (corrlcts.isValid()) {
        //edm::LogInfo("NtupleCSCRegionalCandidates") << "Size: " << corrlcts->size();
        edm::LogInfo("NtupleCSCRegionalCandidates") << "Size: ??";

        // _____________________________________________________________________
        // Begin planting from L1TMuonEndCapTrackProducer.cc

    // bool verbose = false;

    // std::cout<<"Start Upgraded Track Finder Producer::::: event =
    // "<<ev.id().event()<<"\n\n";

    // fprintf (write,"12345\n"); //<-- part of printing text file to send
    // verilog code, not needed if George's package is included

    // std::auto_ptr<L1TMuon::InternalTrackCollection> FoundTracks (new
    // L1TMuon::InternalTrackCollection);
    //std::auto_ptr<l1t::RegionalMuonCandBxCollection> OutputCands(  // MODIFIED
    //    new l1t::RegionalMuonCandBxCollection);                    // MODIFIED

    std::vector<BTrack> PTracks[NUM_SECTORS];

    std::vector<TriggerPrimitive> tester;
    // std::vector<InternalTrack> FoundTracks;

    //////////////////////////////////////////////
    ///////// Make Trigger Primitives ////////////
    //////////////////////////////////////////////

    edm::Handle<CSCCorrelatedLCTDigiCollection> MDC;
    //ev.getByToken(inputTokenCSC, MDC);    // MODIFIED
    iEvent.getByToken(corrlctToken_, MDC);  // MODIFIED
    std::vector<TriggerPrimitive> out;

    auto chamber = MDC->begin();
    auto chend = MDC->end();
    for (; chamber != chend; ++chamber) {
        auto digi = (*chamber).second.first;
        auto dend = (*chamber).second.second;
        for (; digi != dend; ++digi) {
            out.push_back(TriggerPrimitive((*chamber).first, *digi));
        }
    }

    //////////////////////////////////////////////
    ///////// Get Trigger Primitives /////////////  Retrieve TriggerPrimitives
    /// from the event record: Currently does nothing because we don't take
    /// RPC's
    //////////////////////////////////////////////

    // auto tpsrc = _tpinputs.cbegin();
    // auto tpend = _tpinputs.cend();
    // for( ; tpsrc != tpend; ++tpsrc ) {
    // edm::Handle<TriggerPrimitiveCollection> tps;
    // ev.getByLabel(*tpsrc,tps);
    auto tp = out.cbegin();
    auto tpend = out.cend();

    for (; tp != tpend; ++tp) {
        if (tp->subsystem() == 1) {
            // TriggerPrimitiveRef tpref(out,tp - out.cbegin());

            tester.push_back(*tp);

            // std::cout<<"\ntrigger prim found
            // station:"<<tp->detId<CSCDetId>().station()<<std::endl;
        }
    }
    //}
    std::vector<ConvertedHit> CHits[NUM_SECTORS];
    MatchingOutput MO[NUM_SECTORS];

    for (int SectIndex = 0; SectIndex < NUM_SECTORS;
         SectIndex++) { // perform TF on all 12 sectors

        //////////////////////////////////////////////////////  Input is raw hit
        /// information from
        ///////////////// TP Conversion //////////////////////  Output is vector
        /// of Converted Hits
        //////////////////////////////////////////////////////

        std::vector<ConvertedHit> ConvHits = PrimConv(tester, SectIndex);
        CHits[SectIndex] = ConvHits;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////print values for input into Alex's
        /// emulator code/////////////////////////////////////////////////////
        // for(std::vector<ConvertedHit>::iterator h = ConvHits.begin();h !=
        // ConvHits.end();h++){

        // if((h->Id()) > 9){h->SetId(h->Id() - 9);h->SetStrip(h->Strip() +
        // 128);}
        // fprintf (write,"0    1    1     %d
        // %d\n",h->Sub(),h->Station());
        // fprintf (write,"1    %d    %d
        // %d\n",h->Quality(),h->Pattern(),h->Wire());
        // fprintf (write,"%d    0    %d\n",h->Id(),h->Strip());
        //}
        ////////////////////////////////print values for input into Alex's
        /// emulator code/////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////
        //////////////////////////////////////////////////////  Takes the vector
        /// of converted hits and groups into 3 groups of hits
        ////////////////////// BX Grouper ////////////////////  which are 3 BX's
        /// wide. Effectively looking 2 BX's into the future and
        //////////////////////////////////////////////////////  past from the
        /// central BX, this analyzes a total of 5 BX's.
        //////////////////////////////////////////////////////

        std::vector<std::vector<ConvertedHit>> GroupedHits = GroupBX(ConvHits);

        ////////////////////////////////////////////////////////  Creates a zone
        /// for each of the three groups created in the BX Grouper module.
        ////////// Creat Zones for pattern Recognition /////////  The output of
        /// this module not only contains the zones but also the
        ////////////////////////////////////////////////////////  reference back
        /// to the TriggerPrimitives that went into making them.

        std::vector<ZonesOutput> Zout = Zones(GroupedHits);

        ///////////////////////////////
        ///// Pattern Recognition /////  Applies pattern recognition logic on
        /// each of the 3 BX groups and assigns a quality to each keystrip in
        /// the
        /// zone.
        ///// & quality assinment /////  The delete duplicate patterns function
        /// looks at the 3 BX groups and deletes duplicate patterns found from
        /// the
        ///////////////////////////////  same hits. This is where the BX
        /// analysis ends; Only 1 list of found patterns is given to the next
        /// module.

        std::vector<PatternOutput> Pout = Patterns(Zout);

        PatternOutput Test = DeleteDuplicatePatterns(Pout);

        // PrintQuality(Test.detected);

        ///////////////////////////////
        //////Sector Sorting/////////// Sorts through the patterns found in each
        /// zone and selects the best three per zone to send to the next module.
        ///////Finding 3 Best Pattern//
        ///////////////////////////////

        SortingOutput Sout = SortSect(Test);

        //////////////////////////////////
        ///////// Match ph patterns ////// Loops over each sorted pattern and
        /// then loops over all possible triggerprimitives which could have made
        /// the pattern
        ////// to segment inputs ///////// and matches the associated full
        /// precision triggerprimitives to the detected pattern.
        //////////////////////////////////

        MatchingOutput Mout = PhiMatching(Sout);
        MO[SectIndex] = Mout;

        /////////////////////////////////
        //////// Calculate delta //////// Once we have matched the hits we
        /// calculate the delta phi and theta between all
        ////////    ph and th    //////// stations present.
        /////////////////////////////////

        std::vector<std::vector<DeltaOutput>> Dout = CalcDeltas(Mout); ////

        /////////////////////////////////
        /////// Sorts and gives /////////  Loops over all of the found
        /// tracks(looking across zones) and selects the best three per sector.
        ////// Best 3 tracks/sector /////  Here ghost busting is done to delete
        /// tracks which are comprised of the same associated stubs.
        /////////////////////////////////

        std::vector<BTrack> Bout = BestTracks(Dout);
        PTracks[SectIndex] = Bout;


        // _____________________________________________________________________
        // NTUPLE: Store convHits
        for (std::vector<ConvertedHit>::const_iterator it = ConvHits.begin(); it != ConvHits.end(); ++it) {
            ConvertedHit convHit = *it;

            v1_phi             ->push_back(convHit.Phi());
            v1_theta           ->push_back(convHit.Theta());
            v1_phit            ->push_back(convHit.Ph_hit());
            v1_phzvl           ->push_back(convHit.Phzvl());
            v1_station         ->push_back(convHit.Station());
            v1_sub             ->push_back(convHit.Sub());
            v1_id              ->push_back(convHit.Id());
            v1_quality         ->push_back(convHit.Quality());
            v1_pattern         ->push_back(convHit.Pattern());
            v1_wire            ->push_back(convHit.Wire());
            v1_strip           ->push_back(convHit.Strip());
            v1_zhit            ->push_back(convHit.Zhit());
            v1_bx              ->push_back(convHit.BX());
            v1_zoneCont        ->push_back(convHit.ZoneContribution());
            v1_bxgroupCont     ->push_back(std::vector<int>());  // to be filled
            v1_isector         ->push_back(SectIndex);

            // Make a link
            const ConvHitKey convHitKey = make_convHitKey(SectIndex, convHit.Id(), convHit.Strip(), convHit.Wire());
            const unsigned cur_size = convHitRefMap.size();
            convHitRefMap.insert(std::make_pair(convHitKey, cur_size));
        }

        // _____________________________________________________________________
        // NTUPLE: Update convHits bxgroupCont
        assert(GroupedHits.size() == 3);
        for (int i=0; i<3; i++) {  // loop over bxgroups
            for (std::vector<ConvertedHit>::const_iterator it = GroupedHits.at(i).begin(); it != GroupedHits.at(i).end(); ++it) {
                ConvertedHit convHit = *it;
                const ConvHitKey convHitKey = make_convHitKey(SectIndex, convHit.Id(), convHit.Strip(), convHit.Wire());
                const unsigned convHitRef = convHitRefMap.at(convHitKey);

                // Update
                v1_bxgroupCont->at(convHitRef).push_back(i);
            }
        }

        // _____________________________________________________________________
        // NTUPLE: Store zones
        assert(Zout.size() == 3);
        for (int i=0; i<3; i++) {  // loop over bxgroups
            for (int j=0; j<4; j++) {  // loop over zones
                const ZonesOutput& zone = Zout.at(i);

                v2_convHitRefs     ->push_back(std::vector<unsigned>());  // to be filled
                v2_zhitStations    ->push_back(std::vector<unsigned>());  // to be filled
                v2_zhitSuperstrips ->push_back(std::vector<unsigned>());  // to be filled
                v2_izone           ->push_back(j);
                v2_ibxgroup        ->push_back(i);
                v2_isector         ->push_back(SectIndex);

                // Update convHitRefs
                for (std::vector<ConvertedHit>::const_iterator it = zone.convertedhits.begin(); it != zone.convertedhits.end(); ++it) {
                    ConvertedHit convHit = *it;
                    const ConvHitKey convHitKey = make_convHitKey(SectIndex, convHit.Id(), convHit.Strip(), convHit.Wire());
                    const unsigned convHitRef = convHitRefMap.at(convHitKey);
                    //for (const auto& z: convHit.ZoneContribution()) {
                    //    if (j == z) {
                    //        v2_convHitRefs     ->back().push_back(convHitRef);
                    //    }
                    //}
                    v2_convHitRefs     ->back().push_back(convHitRef);
                }

                // Update zhitStations, zhitSuperstrips
                const PhiMemoryImage& image = zone.zone.at(j);
                for (int k=0; k<128; k++) {  // loop over superstrips
                    for (int s=0; s<4; s++) {  // loop over stations
                        if (image.GetBit(s, k)) {  // sparsify
                            v2_zhitStations    ->back().push_back(s);
                            v2_zhitSuperstrips ->back().push_back(k);
                        }
                    }
                }
            }
        }

        // _____________________________________________________________________
        // NTUPLE: Store roads
        assert(Pout.size() == 3);
        for (int i=0; i<3; i++) {  // loop over bxgroups
            for (int j=0; j<4; j++) {  // loop over zones
                for (int k=0; k<128; k++) {  // loop over superstrips
                    const PatternOutput& pattern = Pout.at(i);
                    if (pattern.detected.rank.at(j).at(k) > 0) {  // sparsify

                        v3_rank            ->push_back(pattern.detected.rank[j][k]);
                        v3_layer           ->push_back(pattern.detected.layer[j][k]);
                        v3_straightness    ->push_back(pattern.detected.straightness[j][k]);
                        v3_isDuplicate     ->push_back(true);                     // to be updated
                        v3_convHitRefs     ->push_back(std::vector<unsigned>());  // to be filled
                        v3_dedupConvHitRefs->push_back(std::vector<unsigned>());  // to be filled
                        v3_phiConvHitRefs  ->push_back(std::vector<unsigned>());  // to be filled
                        v3_thetaConvHitRefs->push_back(std::vector<unsigned>());  // to be filled
                        v3_dphiMask        ->push_back(-999);                     // to be updated
                        v3_dphi1           ->push_back(-999);                     // to be updated
                        v3_dphi2           ->push_back(-999);                     // to be updated
                        v3_dthetaMask      ->push_back(-999);                     // to be updated
                        v3_dtheta1         ->push_back(-999);                     // to be updated
                        v3_dtheta2         ->push_back(-999);                     // to be updated
                        v3_precisePhi      ->push_back(-999);                     // to be updated
                        v3_preciseTheta    ->push_back(-999);                     // to be updated
                        v3_preciseRank     ->push_back(-999);                     // to be updated
                        v3_ibesttrack      ->push_back(-999);                     // to be updated
                        v3_iwinner         ->push_back(-999);                     // to be updated
                        v3_isuperstrip     ->push_back(k);
                        v3_izone           ->push_back(j);
                        v3_ibxgroup        ->push_back(i);
                        v3_isector         ->push_back(SectIndex);

                        // Make a link
                        const RoadKey roadKey = make_roadKey(SectIndex, i, j, k);
                        const unsigned cur_size = roadRefMap.size();
                        roadRefMap.insert(std::make_pair(roadKey, cur_size));

                        // Update convHitRefs
                        for (std::vector<ConvertedHit>::const_iterator it = pattern.hits.begin(); it != pattern.hits.end(); ++it) {
                            ConvertedHit convHit = *it;
                            const ConvHitKey convHitKey = make_convHitKey(SectIndex, convHit.Id(), convHit.Strip(), convHit.Wire());
                            const unsigned convHitRef = convHitRefMap.at(convHitKey);
                            //for (const auto& z: convHit.ZoneContribution()) {
                            //    if (j == z) {
                            //        const int CentralBX = 6;
                            //        const int diff = convHit.BX() - CentralBX;
                            //        bool pass = (i==0 && ((diff > -3) && (diff < 1))) ||
                            //                    (i==1 && (abs(diff) < 2))             ||
                            //                    (i==2 && ((diff < 3) && (diff > -1)));
                            //        if (pass)
                            //            v3_convHitRefs     ->back().push_back(convHitRef);
                            //    }
                            //}
                            v3_convHitRefs     ->back().push_back(convHitRef);
                        }
                    }
                }
            }
        }

        // _____________________________________________________________________
        // NTUPLE: Update roads
        for (int j=0; j<4; j++) {  // loop over zones
            for (int k=0; k<128; k++) {  // loop over superstrips
                const PatternOutput& pattern = Test;

                if (pattern.detected.rank.at(j).at(k) > 0) {  // sparsify
                    int ibxgroup = -999;
                    for (int i=0; i<3; i++) {  // loop over bxgroups
                        const RoadKey roadKey = make_roadKey(SectIndex, i, j, k);
                        if (roadRefMap.find(roadKey) != roadRefMap.end()) {
                            const unsigned roadRef = roadRefMap.at(roadKey);
                            if (pattern.detected.rank.at(j).at(k) == v3_rank->at(roadRef)) {
                                ibxgroup = i;
                                break;
                            }
                        }
                    }
                    assert(ibxgroup != -999);
                    assert(pattern.detected.rank[j][k] == Pout.at(ibxgroup).detected.rank[j][k]);
                    assert(pattern.detected.layer[j][k] == Pout.at(ibxgroup).detected.layer[j][k]);
                    assert(pattern.detected.straightness[j][k] == Pout.at(ibxgroup).detected.straightness[j][k]);
                    const RoadKey roadKey = make_roadKey(SectIndex, ibxgroup, j, k);
                    const unsigned roadRef = roadRefMap.at(roadKey);

                    // Update dedupConvHitRefs
                    for (std::vector<ConvertedHit>::const_iterator it = pattern.hits.begin(); it != pattern.hits.end(); ++it) {
                        ConvertedHit convHit = *it;
                        const ConvHitKey convHitKey = make_convHitKey(SectIndex, convHit.Id(), convHit.Strip(), convHit.Wire());
                        const unsigned convHitRef = convHitRefMap.at(convHitKey);
                        //for (const auto& z: convHit.ZoneContribution()) {
                        //    if (j == z) {
                        //        const int CentralBX = 6;
                        //        const int diff = convHit.BX() - CentralBX;
                        //        bool pass = (ibxgroup==0 && ((diff > -3) && (diff < 1))) ||
                        //                    (ibxgroup==1 && (abs(diff) < 2))             ||
                        //                    (ibxgroup==2 && ((diff < 3) && (diff > -1)));
                        //        if (pass) {
                        //            v3_dedupConvHitRefs->at(roadRef).push_back(convHitRef);
                        //            v3_isDuplicate     ->at(roadRef) = false;
                        //        }
                        //    }
                        //}
                        v3_dedupConvHitRefs->at(roadRef).push_back(convHitRef);
                        v3_isDuplicate     ->at(roadRef) = false;
                    }
                }
            }
        }

        // _____________________________________________________________________
        // NTUPLE: Store roads (2)
        for (int j=0; j<4; j++) {  // loop over zones
            for (int l=0; l<3; l++) {  // loop over winners
                assert(Sout.Winners().at(j).at(l).Rank() == Mout.Winners().at(j).at(l).Rank());
                assert(Sout.Winners().at(j).at(l).Strip() == Mout.Winners().at(j).at(l).Strip());
                //assert(Sout.Winners().at(j).at(l).Rank() == (Dout.at(j).at(l).GetWinner().Rank()>>1));
                assert(Sout.Winners().at(j).at(l).Strip() == Dout.at(j).at(l).GetWinner().Strip());
                Winner winner = Sout.Winners().at(j).at(l);

                if (winner.Rank() > 0) {  // sparsify
                    int k = winner.Strip();

                    int ibxgroup = -999;
                    for (int i=0; i<3; i++) {  // loop over bxgroups
                        const RoadKey roadKey = make_roadKey(SectIndex, i, j, k);
                        if (roadRefMap.find(roadKey) != roadRefMap.end()) {
                            const unsigned roadRef = roadRefMap.at(roadKey);
                            if (winner.Rank() == v3_rank->at(roadRef)) {
                                ibxgroup = i;
                                break;
                            }
                        }
                    }
                    assert(ibxgroup != -999);
                    assert(winner.Rank() == Pout.at(ibxgroup).detected.rank[j][k]);
                    const RoadKey roadKey = make_roadKey(SectIndex, ibxgroup, j, k);
                    const unsigned roadRef = roadRefMap.at(roadKey);
                    v3_iwinner         ->at(roadRef) = l;

                    for (int s=0; s<4; s++) {  // loop over stations
                        assert(Mout.PhiMatch().at(j).at(l).at(s).Phi() == Dout.at(j).at(l).GetMatchOut().PhiMatch().at(j).at(l).at(s).Phi());
                        assert(Mout.PhiMatch().at(j).at(l).at(s).Id() == Dout.at(j).at(l).GetMatchOut().PhiMatch().at(j).at(l).at(s).Id());
                        ConvertedHit convHit = Mout.PhiMatch().at(j).at(l).at(s);

                        if (convHit.Phi() != -999) {  // sparsify
                            const ConvHitKey convHitKey = make_convHitKey(SectIndex, convHit.Id(), convHit.Strip(), convHit.Wire());
                            const unsigned convHitRef = convHitRefMap.at(convHitKey);
                            v3_phiConvHitRefs  ->at(roadRef).push_back(convHitRef);
                        }

                        for (int t=0; t<2; t++) {  // loop over wire ambiguities
                            assert(Mout.ThetaMatch().at(j).at(l).at(s).at(t).Theta() == Dout.at(j).at(l).GetMatchOut().ThetaMatch().at(j).at(l).at(s).at(t).Theta());
                            assert(Mout.ThetaMatch().at(j).at(l).at(s).at(t).Id() == Dout.at(j).at(l).GetMatchOut().ThetaMatch().at(j).at(l).at(s).at(t).Id());
                            ConvertedHit convHit = Mout.ThetaMatch().at(j).at(l).at(s).at(t);

                            if (convHit.Theta() != -999) {  // sparsify
                                const ConvHitKey convHitKey = make_convHitKey(SectIndex, convHit.Id(), convHit.Strip(), convHit.Wire());
                                const unsigned convHitRef = convHitRefMap.at(convHitKey);
                                v3_thetaConvHitRefs->at(roadRef).push_back(convHitRef);
                            }
                        }
                    }

                    DeltaOutput deltas = Dout.at(j).at(l);
                    assert(deltas.Deltas().size() == 2 && deltas.Deltas().at(0).size() == 2 && deltas.Deltas().at(1).size() == 2);
                    v3_dphi1           ->at(roadRef) = deltas.Deltas()[0][0];
                    v3_dphi2           ->at(roadRef) = deltas.Deltas()[0][1];
                    v3_dtheta1         ->at(roadRef) = deltas.Deltas()[1][0];
                    v3_dtheta2         ->at(roadRef) = deltas.Deltas()[1][1];
                    v3_precisePhi      ->at(roadRef) = deltas.Phi();
                    v3_preciseTheta    ->at(roadRef) = deltas.Theta();
                    v3_preciseRank     ->at(roadRef) = deltas.GetWinner().Rank();
                }
            }
        }

        // _____________________________________________________________________
        // NTUPLE: Store roads (3)
        assert(Bout.size() == 3);
        for (int l=0; l<3; l++) {  // loop over best winners (per sector)
            const BTrack& btrack = Bout.at(l);

            if (btrack.phi > 0) {  // sparsify
                Winner winner = btrack.winner;

                int k = winner.Strip();
                int ibxgroup = -999;
                int jzone = -999;
                for (int i=0; i<3; i++) {  // loop over bxgroups
                    for (int j=0; j<4; j++) {  // loop over zones
                        const RoadKey& roadKey = make_roadKey(SectIndex, i, j, k);
                        if (roadRefMap.find(roadKey) != roadRefMap.end()) {
                            const unsigned roadRef = roadRefMap.at(roadKey);
                            if (winner.Rank() == v3_preciseRank->at(roadRef)) {
                                ibxgroup = i;
                                jzone = j;
                                break;
                            }
                        }
                    }
                }
                assert(ibxgroup != -999 && jzone != -999);
                const RoadKey roadKey = make_roadKey(SectIndex, ibxgroup, jzone, k);
                const unsigned roadRef = roadRefMap.at(roadKey);
                assert(btrack.phi == v3_precisePhi->at(roadRef));
                assert(btrack.theta == v3_preciseTheta->at(roadRef));
                v3_ibesttrack      ->at(roadRef) = l;
            }
        }
        // _____________________________________________________________________
        // END NTUPLE
    }

    ///////////////////////////////////////
    /// Collect Muons from all sectors ////
    ///////////////////////////////////////

    std::vector<BTrack> PTemp[NUM_SECTORS];
    std::vector<BTrack> AllTracks;
    for (int i = 0; i < NUM_SECTORS; i++)
        PTemp[i] = PTracks[i];

    for (int j = 0; j < 36; j++) {

        if (PTemp[j / 3][j % 3].phi) // no track
            AllTracks.push_back(PTemp[j / 3][j % 3]);
    }

    ///////////////////////////////////
    /// Make Internal track if ////////
    /////// tracks are found //////////
    ///////////////////////////////////

    std::vector<l1t::RegionalMuonCand> tester1;
    std::vector<std::pair<int, l1t::RegionalMuonCand>> holder;

    for (unsigned int fbest = 0; fbest < AllTracks.size(); fbest++) {

        if (AllTracks[fbest].phi) {

            InternalTrack tempTrack;
            tempTrack.setType(2);
            tempTrack.phi = AllTracks[fbest].phi;
            tempTrack.theta = AllTracks[fbest].theta;
            tempTrack.rank = AllTracks[fbest].winner.Rank();
            tempTrack.deltas = AllTracks[fbest].deltas;
            std::vector<int> ps, ts;

            int sector = -1;
            bool ME13 = false;
            int me1address = 0, me2address = 0, CombAddress = 0, mode = 0;
            int ebx = 20, sebx = 20;

            for (std::vector<ConvertedHit>::iterator A =
                     AllTracks[fbest].AHits.begin();
                 A != AllTracks[fbest].AHits.end(); A++) {

                if (A->Phi() != -999) {

                    int station = A->TP().detId<CSCDetId>().station();
                    int id = A->TP().getCSCData().cscID;
                    int trknm =
                        A->TP().getCSCData().trknmb; // A->TP().getCSCData().bx

                    if (A->TP().getCSCData().bx < ebx) {
                        sebx = ebx;
                        ebx = A->TP().getCSCData().bx;
                    } else if (A->TP().getCSCData().bx < sebx) {
                        sebx = A->TP().getCSCData().bx;
                    }

                    tempTrack.addStub(A->TP());
                    ps.push_back(A->Phi());
                    ts.push_back(A->Theta());
                    sector = (A->TP().detId<CSCDetId>().endcap() - 1) * 6 +
                             A->TP().detId<CSCDetId>().triggerSector() - 1;
                    // std::cout<<"Q: "<<A->Quality()<<", keywire:
                    // "<<A->Wire()<<", strip: "<<A->Strip()<<std::endl;

                    switch (station) {
                    case 1:
                        mode |= 8;
                        break;
                    case 2:
                        mode |= 4;
                        break;
                    case 3:
                        mode |= 2;
                        break;
                    case 4:
                        mode |= 1;
                        break;
                    default:
                        mode |= 0;
                    }

                    if (A->TP().detId<CSCDetId>().station() == 1 &&
                        A->TP().detId<CSCDetId>().ring() == 3)
                        ME13 = true;

                    if (station == 1 && id > 3 && id < 7) {

                        int sub = 2;
                        if (A->TP().detId<CSCDetId>().chamber() % 6 > 2)
                            sub = 1;

                        me1address = id;
                        me1address -= 3;
                        me1address += 3 * (sub - 1);
                        me1address = id << 1; //
                        me1address |= trknm - 1;
                    }

                    if (station == 2 && id > 3) {

                        me2address = id;
                        me2address -= 3;
                        me2address = me2address << 1;
                        me2address |= trknm - 1;
                    }
                }
            }
            tempTrack.phis = ps;
            tempTrack.thetas = ts;

            //float xmlpt = CalculatePt(tempTrack, es);    // MODIFIED
            float xmlpt = CalculatePt(tempTrack, iSetup);  // MODIFIED
            tempTrack.pt = xmlpt * 1.4;
            // FoundTracks->push_back(tempTrack);

            CombAddress = (me2address << 4) | me1address;

            l1t::RegionalMuonCand outCand = MakeRegionalCand(
                xmlpt * 1.4, AllTracks[fbest].phi, AllTracks[fbest].theta,
                CombAddress, mode, 1, sector);
            // NOTE: assuming that all candidates come from the central BX:
            // int bx = 0;
            float theta_angle = (AllTracks[fbest].theta * 0.2851562 + 8.5) *
                                (3.14159265359 / 180);
            float eta = (-1) * log(tan(theta_angle / 2));
            std::pair<int, l1t::RegionalMuonCand> outPair(sebx, outCand);

            if (!ME13 && fabs(eta) > 1.1)
                holder.push_back(outPair);

            // _________________________________________________________________
            // NTUPLE: Store internal tracks
            Winner winner = AllTracks[fbest].winner;
            int SectIndex = sector;
            int k = winner.Strip();
            int ibxgroup = -999;
            int jzone = -999;
            for (int i=0; i<3; i++) {  // loop over bxgroups
                for (int j=0; j<4; j++) {  // loop over zones
                    const RoadKey& roadKey = make_roadKey(SectIndex, i, j, k);
                    if (roadRefMap.find(roadKey) != roadRefMap.end()) {
                        const unsigned roadRef = roadRefMap.at(roadKey);
                        if (winner.Rank() == v3_preciseRank->at(roadRef)) {
                            ibxgroup = i;
                            jzone = j;
                            break;
                        }
                    }
                }
            }
            assert(ibxgroup != -999 && jzone != -999);
            const RoadKey roadKey = make_roadKey(SectIndex, ibxgroup, jzone, k);
            const unsigned roadRef = roadRefMap.at(roadKey);

            v4_phi        ->push_back(AllTracks[fbest].phi);
            v4_theta      ->push_back(AllTracks[fbest].theta);
            v4_rank       ->push_back(AllTracks[fbest].winner.Rank());
            v4_dphi1      ->push_back(AllTracks[fbest].deltas[0][0]);
            v4_dphi2      ->push_back(AllTracks[fbest].deltas[0][1]);
            v4_dtheta1    ->push_back(AllTracks[fbest].deltas[1][0]);
            v4_dtheta2    ->push_back(AllTracks[fbest].deltas[1][1]);
            v4_phis       ->push_back(tempTrack.phis);
            v4_thetas     ->push_back(tempTrack.thetas);
            v4_pt         ->push_back(xmlpt * 1.4);
            v4_me1address ->push_back(me1address);
            v4_me2address ->push_back(me2address);
            v4_combaddress->push_back(CombAddress);
            v4_mode       ->push_back(mode);
            v4_ebx        ->push_back(ebx);
            v4_sebx       ->push_back(sebx);
            v4_isEndcap   ->push_back(!ME13 && fabs(eta) > 1.1);
            v4_globalPhi  ->push_back(getConvGlobalPhi(sector, AllTracks[fbest].phi));
            v4_globalTheta->push_back(getConvGlobalTheta(sector, AllTracks[fbest].theta));
            v4_globalEta  ->push_back(getConvGlobalEta(sector, AllTracks[fbest].theta));
            v4_ibesttrack ->push_back(fbest);
            v4_isector    ->push_back(sector);
            v4_roadRef    ->push_back(roadRef);
            // _________________________________________________________________
            // END NTUPLE
        }
    }

    OutputCands->setBXRange(-2, 2);

    for (int sect = 0; sect < 12; sect++) {

        for (unsigned int h = 0; h < holder.size(); h++) {

            int bx = holder[h].first - 6;
            int sector = holder[h].second.processor();
            if (holder[h].second.trackFinderType() == 3)
                sector += 6;

            if (sector == sect) {
                OutputCands->push_back(bx, holder[h].second);
            }
        }
    }

        // _____________________________________________________________________
        // End planting from L1TMuonEndCapTrackProducer.cc

    } else {
        edm::LogError("NtupleCSCRegionalCandidates") << "Cannot get the product: " << corrlctTag_;
    }


    edm::Handle<l1t::RegionalMuonCandBxCollection> tracks;
    //iEvent.getByLabel(trackTag_, tracks);
    if (!trackToken_.isUninitialized())
        iEvent.getByToken(trackToken_, tracks);

    if (tracks.isValid()) {
        //edm::LogInfo("NtupleCSCRegionalCandidates") << "Size: " << tracks->size();
        edm::LogInfo("NtupleCSCRegionalCandidates") << "Size: ??";

        // _____________________________________________________________________
        // NTUPLE: Store: regional muond candidates
        l1t::RegionalMuonCandBxCollection::const_iterator iter1 = tracks->begin();
        l1t::RegionalMuonCandBxCollection::const_iterator iter2 = OutputCands->begin();
        for (; iter1 != tracks->end(); ++iter1, ++iter2) {
            assert(iter1->hwPt() == iter2->hwPt());
            assert(iter1->hwPhi() == iter2->hwPhi());
            assert(iter1->hwEta() == iter2->hwEta());

            l1t::RegionalMuonCandBxCollection::const_iterator it = iter1;
            v5_hwPt           ->push_back(it->hwPt());
            v5_hwPhi          ->push_back(it->hwPhi());
            v5_hwEta          ->push_back(it->hwEta());
            v5_hwSign         ->push_back(it->hwSign());
            v5_hwSignValid    ->push_back(it->hwSignValid());
            v5_hwQual         ->push_back(it->hwQual());
            v5_link           ->push_back(it->link());
            v5_processor      ->push_back(it->processor());
            v5_trackFinderType->push_back(it->trackFinderType());
            v5_hwHF           ->push_back(it->hwHF());
            v5_dataword       ->push_back(it->dataword());
        }
        // _____________________________________________________________________
        // END NTUPLE
    } else {
        edm::LogError("NtupleCSCRegionalCandidates") << "Cannot get the product: " << trackTag_;
    }

    //__________________________________________________________________________
    // NTUPLE: Store output sizes
    *v1_size = v1_isector->size();
    *v2_size = v2_isector->size();
    *v3_size = v3_isector->size();
    *v4_size = v4_isector->size();
    *v5_size = v5_hwPt->size();

    //__________________________________________________________________________
    // convHits
    iEvent.put(v1_phi             , prefix_ + "convHits@" + "phi"              + suffix_);
    iEvent.put(v1_theta           , prefix_ + "convHits@" + "theta"            + suffix_);
    iEvent.put(v1_phit            , prefix_ + "convHits@" + "phit"             + suffix_);
    iEvent.put(v1_phzvl           , prefix_ + "convHits@" + "phzvl"            + suffix_);
    iEvent.put(v1_station         , prefix_ + "convHits@" + "station"          + suffix_);
    iEvent.put(v1_sub             , prefix_ + "convHits@" + "sub"              + suffix_);
    iEvent.put(v1_id              , prefix_ + "convHits@" + "id"               + suffix_);
    iEvent.put(v1_quality         , prefix_ + "convHits@" + "quality"          + suffix_);
    iEvent.put(v1_pattern         , prefix_ + "convHits@" + "pattern"          + suffix_);
    iEvent.put(v1_wire            , prefix_ + "convHits@" + "wire"             + suffix_);
    iEvent.put(v1_strip           , prefix_ + "convHits@" + "strip"            + suffix_);
    iEvent.put(v1_zhit            , prefix_ + "convHits@" + "zhit"             + suffix_);
    iEvent.put(v1_bx              , prefix_ + "convHits@" + "bx"               + suffix_);
    iEvent.put(v1_zoneCont        , prefix_ + "convHits@" + "zoneCont"         + suffix_);
    iEvent.put(v1_bxgroupCont     , prefix_ + "convHits@" + "bxgroupCont"      + suffix_);
    iEvent.put(v1_isector         , prefix_ + "convHits@" + "isector"          + suffix_);
    iEvent.put(v1_size            , prefix_ + "convHits@" + "size"             + suffix_);
    // zones
    iEvent.put(v2_convHitRefs     , prefix_ + "zones@"    + "convHitRefs"      + suffix_);
    iEvent.put(v2_zhitStations    , prefix_ + "zones@"    + "zhitStations"     + suffix_);
    iEvent.put(v2_zhitSuperstrips , prefix_ + "zones@"    + "zhitSuperstrips"  + suffix_);
    iEvent.put(v2_izone           , prefix_ + "zones@"    + "izone"            + suffix_);
    iEvent.put(v2_ibxgroup        , prefix_ + "zones@"    + "ibxgroup"         + suffix_);
    iEvent.put(v2_isector         , prefix_ + "zones@"    + "isector"          + suffix_);
    iEvent.put(v2_size            , prefix_ + "zones@"    + "size"             + suffix_);
    // roads
    iEvent.put(v3_rank            , prefix_ + "roads@"    + "rank"             + suffix_);
    iEvent.put(v3_layer           , prefix_ + "roads@"    + "layer"            + suffix_);
    iEvent.put(v3_straightness    , prefix_ + "roads@"    + "straightness"     + suffix_);
    iEvent.put(v3_isDuplicate     , prefix_ + "roads@"    + "isDuplicate"      + suffix_);
    iEvent.put(v3_convHitRefs     , prefix_ + "roads@"    + "convHitRefs"      + suffix_);
    iEvent.put(v3_dedupConvHitRefs, prefix_ + "roads@"    + "dedupConvHitRefs" + suffix_);
    iEvent.put(v3_phiConvHitRefs  , prefix_ + "roads@"    + "phiConvHitRefs"   + suffix_);
    iEvent.put(v3_thetaConvHitRefs, prefix_ + "roads@"    + "thetaConvHitRefs" + suffix_);
    iEvent.put(v3_dphiMask        , prefix_ + "roads@"    + "dphiMask"         + suffix_);
    iEvent.put(v3_dphi1           , prefix_ + "roads@"    + "dphi1"            + suffix_);
    iEvent.put(v3_dphi2           , prefix_ + "roads@"    + "dphi2"            + suffix_);
    iEvent.put(v3_dthetaMask      , prefix_ + "roads@"    + "dthetaMask"       + suffix_);
    iEvent.put(v3_dtheta1         , prefix_ + "roads@"    + "dtheta1"          + suffix_);
    iEvent.put(v3_dtheta2         , prefix_ + "roads@"    + "dtheta2"          + suffix_);
    iEvent.put(v3_precisePhi      , prefix_ + "roads@"    + "precisePhi"       + suffix_);
    iEvent.put(v3_preciseTheta    , prefix_ + "roads@"    + "preciseTheta"     + suffix_);
    iEvent.put(v3_preciseRank     , prefix_ + "roads@"    + "preciseRank"      + suffix_);
    iEvent.put(v3_ibesttrack      , prefix_ + "roads@"    + "ibesttrack"       + suffix_);
    iEvent.put(v3_iwinner         , prefix_ + "roads@"    + "iwinner"          + suffix_);
    iEvent.put(v3_isuperstrip     , prefix_ + "roads@"    + "isuperstrip"      + suffix_);
    iEvent.put(v3_izone           , prefix_ + "roads@"    + "izone"            + suffix_);
    iEvent.put(v3_ibxgroup        , prefix_ + "roads@"    + "ibxgroup"         + suffix_);
    iEvent.put(v3_isector         , prefix_ + "roads@"    + "isector"          + suffix_);
    iEvent.put(v3_size            , prefix_ + "roads@"    + "size"             + suffix_);
    // internal tracks
    iEvent.put(v4_phi             , prefix_ + "itracks@"  + "phi"              + suffix_);
    iEvent.put(v4_theta           , prefix_ + "itracks@"  + "theta"            + suffix_);
    iEvent.put(v4_rank            , prefix_ + "itracks@"  + "rank"             + suffix_);
    iEvent.put(v4_dphi1           , prefix_ + "itracks@"  + "dphi1"            + suffix_);
    iEvent.put(v4_dphi2           , prefix_ + "itracks@"  + "dphi2"            + suffix_);
    iEvent.put(v4_dtheta1         , prefix_ + "itracks@"  + "dtheta1"          + suffix_);
    iEvent.put(v4_dtheta2         , prefix_ + "itracks@"  + "dtheta2"          + suffix_);
    iEvent.put(v4_phis            , prefix_ + "itracks@"  + "phis"             + suffix_);
    iEvent.put(v4_thetas          , prefix_ + "itracks@"  + "thetas"           + suffix_);
    iEvent.put(v4_pt              , prefix_ + "itracks@"  + "pt"               + suffix_);
    iEvent.put(v4_me1address      , prefix_ + "itracks@"  + "me1address"       + suffix_);
    iEvent.put(v4_me2address      , prefix_ + "itracks@"  + "me2address"       + suffix_);
    iEvent.put(v4_combaddress     , prefix_ + "itracks@"  + "combaddress"      + suffix_);
    iEvent.put(v4_mode            , prefix_ + "itracks@"  + "mode"             + suffix_);
    iEvent.put(v4_ebx             , prefix_ + "itracks@"  + "ebx"              + suffix_);
    iEvent.put(v4_sebx            , prefix_ + "itracks@"  + "sebx"             + suffix_);
    iEvent.put(v4_isEndcap        , prefix_ + "itracks@"  + "isEndcap"         + suffix_);
    iEvent.put(v4_globalPhi       , prefix_ + "itracks@"  + "globalPhi"        + suffix_);
    iEvent.put(v4_globalTheta     , prefix_ + "itracks@"  + "globalTheta"      + suffix_);
    iEvent.put(v4_globalEta       , prefix_ + "itracks@"  + "globalEta"        + suffix_);
    iEvent.put(v4_ibesttrack      , prefix_ + "itracks@"  + "ibesttrack"       + suffix_);
    iEvent.put(v4_isector         , prefix_ + "itracks@"  + "isector"          + suffix_);
    iEvent.put(v4_roadRef         , prefix_ + "itracks@"  + "roadRef"          + suffix_);
    iEvent.put(v4_size            , prefix_ + "itracks@"  + "size"             + suffix_);
    // regional muon candidates
    iEvent.put(v5_hwPt            , prefix_ + "emuons@"   + "hwPt"             + suffix_);
    iEvent.put(v5_hwPhi           , prefix_ + "emuons@"   + "hwPhi"            + suffix_);
    iEvent.put(v5_hwEta           , prefix_ + "emuons@"   + "hwEta"            + suffix_);
    iEvent.put(v5_hwSign          , prefix_ + "emuons@"   + "hwSign"           + suffix_);
    iEvent.put(v5_hwSignValid     , prefix_ + "emuons@"   + "hwSignValid"      + suffix_);
    iEvent.put(v5_hwQual          , prefix_ + "emuons@"   + "hwQual"           + suffix_);
    iEvent.put(v5_link            , prefix_ + "emuons@"   + "link"             + suffix_);
    iEvent.put(v5_processor       , prefix_ + "emuons@"   + "processor"        + suffix_);
    iEvent.put(v5_trackFinderType , prefix_ + "emuons@"   + "trackFinderType"  + suffix_);
    iEvent.put(v5_hwHF            , prefix_ + "emuons@"   + "hwHF"             + suffix_);
    iEvent.put(v5_dataword        , prefix_ + "emuons@"   + "dataword"         + suffix_);
    //iEvent.put(v5_trackAddress    , prefix_ + "emuons@"   + "trackAddress"     + suffix_);
    iEvent.put(v5_size            , prefix_ + "emuons@"   + "size"             + suffix_);
}