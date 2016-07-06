#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCChamber.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/CSCTriggerNumbering.h"
#include "L1Trigger/CSCCommonTrigger/interface/CSCConstants.h"

#include "L1TMuonSimulations/MuonTools/interface/ModuleIdHelper.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>


class AnalyzerCSCGeometry : public edm::EDAnalyzer {
  public:
    explicit AnalyzerCSCGeometry(const edm::ParameterSet&);
    ~AnalyzerCSCGeometry();

  private:
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual void endRun(const edm::Run&, const edm::EventSetup&);

    virtual void beginJob();
    virtual void endJob();

    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    /// Event setup
    const CSCGeometry * theCSCGeometry_;

    /// Maps
    std::map<uint32_t, const CSCLayer*> moduleIdMap_;

    /// Configurations
    std::string csvfile_;
    int verbose_;
};

AnalyzerCSCGeometry::AnalyzerCSCGeometry(const edm::ParameterSet& iConfig)
: csvfile_(iConfig.getParameter<std::string>("csv") ),
  verbose_(iConfig.getParameter<int>("verbosity") ) {}

AnalyzerCSCGeometry::~AnalyzerCSCGeometry() {}

void AnalyzerCSCGeometry::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    /// Geometry setup
    edm::ESHandle<CSCGeometry> cscGeometryHandle;
    iSetup.get<MuonGeometryRecord>().get(cscGeometryHandle);
    if (!cscGeometryHandle.isValid()) {
        edm::LogWarning("NtupleSimHits") << "Unable to get MuonGeometryRecord!";
    } else {
        theCSCGeometry_ = cscGeometryHandle.product();
    }

    // _________________________________________________________________________
    unsigned n = 0;

    if (verbose_) {
        std::cout << "Looping over CSC chambers" << std::endl;
    }

    // Loop over detUnits
    for (CSCGeometry::DetUnitContainer::const_iterator it = theCSCGeometry_->detUnits().begin();
        it != theCSCGeometry_->detUnits().end(); ++it){

        const CSCLayer * cscLayer = dynamic_cast<const CSCLayer*>(*it); // CSCLayer is inherited from GeomDetUnit
        const CSCChamber * cscChamber = cscLayer->chamber();            // CSCChamber is inherited from GeomDet
        const CSCLayerGeometry * cscLayerGeom = cscLayer->geometry();   // CSCLayerGeometry is inherited from TrapezoidalPlaneBounds
        const CSCStripTopology * cscStripTopo = cscLayerGeom->topology();
        const CSCWireTopology * cscWireTopo = cscLayerGeom->wireTopology();

        if (cscLayer) {
            DetId detId = cscLayer->geographicalId();
            const CSCDetId cscDet(detId);

            int iendcap    = cscDet.endcap();         // 1: positive, 2: negative
            int istation   = cscDet.station();        // 1-4: stations 1-4
            int iring      = cscDet.ring();           // 1-4: (ME1/1,2,3,A) or 1-2: (ME2/1,2 or ME3/1,2 or ME4/1,2)
            int ichamber   = cscDet.chamber();        // 1-36
            int ilayer     = cscDet.layer();          // 1-6
            int isector    = cscDet.triggerSector();  // 1-6
            int isubsector = (cscDet.station() != 1) ? 0 : ((cscDet.chamber()%6 > 2) ? 1 : 2);  // 1-2 for ME1, 0 for ME2,3,4
            int cscID      = cscDet.triggerCscId();   // 1-9

            assert(CSCTriggerNumbering::triggerSectorFromLabels(cscDet) == cscDet.triggerSector());
            assert(CSCTriggerNumbering::triggerCscIdFromLabels(cscDet) == cscDet.triggerCscId());
            assert(CSCTriggerNumbering::triggerSubSectorFromLabels(cscDet) == isubsector);

            if (ilayer == CSCConstants::KEY_ALCT_LAYER) {  // should it be CSCConstants::KEY_CLCT_LAYER?
                // Get moduleId
                uint32_t moduleId = ModuleIdHelper::getModuleId(cscDet);
                bool evenBit      = ModuleIdHelper::isEven(cscDet);
                bool frBit        = ModuleIdHelper::isFront(cscDet);
                bool ccwBit       = ModuleIdHelper::isCounterClockwise(cscDet);

                // Geometry ...
                int nstrips = cscLayerGeom->numberOfStrips();
                int nwires = cscLayerGeom->numberOfWires();
                int nwiregroups = cscLayerGeom->numberOfWireGroups();

                float stripPhiPitch = cscLayerGeom->stripPhiPitch();  // angular pitch
                //float stripPitch = cscLayerGeom->stripPitch();  // local pitch
                float stripOffset = cscLayerGeom->stripOffset();
                int stagger = cscLayerGeom->stagger();
                float xOfStrip0 = cscLayerGeom->xOfStrip(1);
                float xOfStripN = cscLayerGeom->xOfStrip(nstrips);
                float stripAngle0 = cscLayerGeom->stripAngle(1);
                float stripAngleN = cscLayerGeom->stripAngle(nstrips);
                float stripLength = cscStripTopo->stripLength();
                float originToIntersect = cscStripTopo->originToIntersection();

                float wirePitch = cscLayerGeom->wirePitch();
                float wireAngle = cscLayerGeom->wireAngle();
                float wireSpacing = cscWireTopo->wireSpacing();
                float wideWidthOfPlane = cscWireTopo->wideWidthOfPlane();
                float narrowWidthOfPlane = cscWireTopo->narrowWidthOfPlane();
                float lengthOfPlane = cscWireTopo->lengthOfPlane();

                float boundWidth = cscLayer->surface().bounds().width();
                float boundWidthAtHalfLength = cscLayer->surface().bounds().widthAtHalfLength();
                float boundLength = cscLayer->surface().bounds().length();
                float boundThickness = cscLayer->surface().bounds().thickness();

                int chamberType = cscChamber->specs()->chamberType();
                //float stripPhiPitch2 = cscChamber->specs()->stripPhiPitch();
                //float ctiOffset2 = cscChamber->specs()->ctiOffset();
                float stripRes = cscChamber->specs()->stripResolution();
                float wireRes = cscChamber->specs()->wireResolution();
                int nstrips2 = cscChamber->specs()->nStrips();
                assert(nstrips2 == nstrips);

                const GlobalPoint& centerOfStrip0 = cscLayer->centerOfStrip(1);
                const GlobalPoint& centerOfStripN = cscLayer->centerOfStrip(nstrips);
                const GlobalPoint& centerOfWireGroup = cscLayer->centerOfWireGroup(1);
                bool isCounterClockwise = reco::deltaPhi(centerOfStrip0.barePhi(), centerOfStripN.barePhi()) < 0.;
                bool isFront = false;
                if (cscDet.station() == 1 && (cscDet.ring() == 1 || cscDet.ring() == 4)) {
                    if (std::abs(centerOfStrip0.z()) < 602.332)  isFront = true;
                } else if (cscDet.station() == 1 && (cscDet.ring() == 2)) {
                    if (std::abs(centerOfStrip0.z()) < 698.693)  isFront = true;
                } else if (cscDet.station() == 2) {
                    if (std::abs(centerOfStrip0.z()) < 828.392)  isFront = true;
                } else if (cscDet.station() == 3) {
                    if (std::abs(centerOfStrip0.z()) < 935.437)  isFront = true;
                } else if (cscDet.station() == 4) {
                    if (std::abs(centerOfStrip0.z()) < 1025.17)  isFront = true;
                }
                bool isEven = (cscDet.chamber() % 2 == 0);

                if (verbose_) {
                    std::cout << ".. " << n << " endcap: " << iendcap << " station: " << istation << " ring: " << iring << " chamber: " << ichamber << " layer: " << ilayer << std::endl;
                    std::cout << ".. " << n << " sector: " << isector << " subsector: " << isubsector << " cscID: " << cscID << " -- moduleId: " << moduleId << " frBit: " << frBit << " ccwBit: " << ccwBit << std::endl;
                    std::cout << ".. " << n << " nstrips: " << nstrips << " nwires: " << nwires << " nwiregroups: " << nwiregroups << " chamberType: " << chamberType << " stripRes: " << stripRes << " wireRes: " << wireRes << " boundWidthAtHalfLength: " << boundWidthAtHalfLength << " boundWidth: " << boundWidth << " boundLength: " << boundLength << " boundThickness: " << boundThickness << std::endl;
                    std::cout << ".. " << n << " stripPhiPitch: " << stripPhiPitch << " stripOffset: " << stripOffset << " stagger: " << stagger << " xOfStrip0: " << xOfStrip0 << " xOfStripN: " << xOfStripN << " stripAngle0: " << stripAngle0 << " stripAngleN: " << stripAngleN << " stripLength: " << stripLength << " originToIntersect: " << originToIntersect << std::endl;
                    std::cout << ".. " << n << " wirePitch: " << wirePitch << " wireAngle: " << wireAngle << " wireSpacing: " << wireSpacing << " wideWidthOfPlane: " << wideWidthOfPlane << " narrowWidthOfPlane: " << narrowWidthOfPlane << " lengthOfPlane: " << lengthOfPlane << std::endl;
                    std::cout << ".. " << n << " centerOfStrip: " << centerOfStrip0.x() << "," << centerOfStrip0.y() << "," << centerOfStrip0.z() << " centerOfWireGroup: " << centerOfWireGroup.x() << "," << centerOfWireGroup.y() << "," << centerOfWireGroup.z() << std::endl;

                    int nwires1 = 0;
                    for (int wg=0; wg!=nwiregroups; ++wg) {
                        int nwirespergroup = cscLayerGeom->numberOfWiresPerGroup(wg+1);
                        float middlewire = cscLayerGeom->middleWireOfGroup(wg+1);
                        float yRes = cscLayerGeom->yResolution(wg+1);
                        nwires1 += nwirespergroup;
                        std::cout << ".... " << wg << " nwirespergroup: " << nwirespergroup << " nwires: " << nwires1 << " middlewire: " << middlewire << " yResolution: " << yRes << std::endl;
                    }

                }

                auto ins = moduleIdMap_.insert(std::make_pair(moduleId, cscLayer));
                if (!ins.second) {
                    std::cerr << "ERROR: fail to insert into map!" << std::endl;
                    throw std::invalid_argument("fail to insert into map");  // check if moduleId is unique
                }

                if (moduleId > 611) {
                    std::cerr << "ERROR: moduleId > 611!" << std::endl;
                    throw std::invalid_argument("moduleId > 611");  // check if there are unused moduleId
                }

                assert(evenBit == isEven);
                assert(frBit == isFront);
                assert(ccwBit == isCounterClockwise);

                n++;

            }  // end if KEY_CLCT_LAYER
        }  // end if cscLayer
    }  // end loop over detUnits

}

void AnalyzerCSCGeometry::endRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {}

void AnalyzerCSCGeometry::beginJob() {}

void AnalyzerCSCGeometry::endJob() {
    std::cout << "Map size: " << moduleIdMap_.size() << std::endl;


    // Open text file
    std::ofstream outfile(csvfile_);

    // Write text file
    outfile << "moduleId/I, x0_cm/D, y0_cm/D, z0_cm/D, x1_cm/D, y1_cm/D, z1_cm/D, x2_cm/D, y2_cm/D, z2_cm/D, x3_cm/D, y3_cm/D, z3_cm/D, nstrips/I, nwires/I, nwiregroups/I, evenBit/I, frBit/I, ccwBit/I" << std::endl;

    for (const auto& kv : moduleIdMap_) {
        const uint32_t moduleId = kv.first;
        const CSCLayer * cscLayer = kv.second;
        const CSCLayerGeometry * cscLayerGeom = cscLayer->geometry();
        const CSCDetId cscDet(cscLayer->id());

        int nstrips = cscLayerGeom->numberOfStrips();
        int nwires = cscLayerGeom->numberOfWires();
        int nwiregroups = cscLayerGeom->numberOfWireGroups();

        bool evenBit      = ModuleIdHelper::isEven(cscDet);
        bool frBit        = ModuleIdHelper::isFront(cscDet);
        bool ccwBit       = ModuleIdHelper::isCounterClockwise(cscDet);

        const LocalPoint lp0 = cscLayerGeom->stripWireGroupIntersection(1, 1);
        const LocalPoint lp1 = cscLayerGeom->stripWireGroupIntersection(nstrips, 1);
        const LocalPoint lp2 = cscLayerGeom->stripWireGroupIntersection(nstrips, nwiregroups);
        const LocalPoint lp3 = cscLayerGeom->stripWireGroupIntersection(1, nwiregroups);

        const GlobalPoint& gp0 = cscLayer->surface().toGlobal(lp0);
        const GlobalPoint& gp1 = cscLayer->surface().toGlobal(lp1);
        const GlobalPoint& gp2 = cscLayer->surface().toGlobal(lp2);
        const GlobalPoint& gp3 = cscLayer->surface().toGlobal(lp3);

        // Positions are in unit of centimeter
        outfile.unsetf(std::ios_base::floatfield);
        outfile << moduleId << ", "
                << std::fixed << std::setprecision(8)
                << gp0.x() << ", " << gp0.y() << ", " << gp0.z() << ", "
                << gp1.x() << ", " << gp1.y() << ", " << gp1.z() << ", "
                << gp2.x() << ", " << gp2.y() << ", " << gp2.z() << ", "
                << gp3.x() << ", " << gp3.y() << ", " << gp3.z() << ", "
                << nstrips << ", " << nwires << ", " << nwiregroups << ", "
                << evenBit << ", " << frBit << ", " << ccwBit
                << std::endl;
    }

    // Close text file
    outfile.close();

    std::cout << ">>> " << csvfile_ << " is written." << std::endl;
}

// ANALYZE
void AnalyzerCSCGeometry::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    // Do nothing
}

// DEFINE THIS AS A PLUG-IN
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(AnalyzerCSCGeometry);
