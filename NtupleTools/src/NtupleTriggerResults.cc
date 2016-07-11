#include "L1TMuonSimulations/NtupleTools/interface/NtupleTriggerResults.h"

#include <string>
#include <regex>

namespace {
std::string replace_all(std::string str, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = str.find(search, pos)) != std::string::npos) {
        str.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return str;
}
}

// _____________________________________________________________________________
void NtupleTriggerResults::syncHLTPaths(const edm::TriggerNames& triggerNames) {
    if (hltPSetID_ != triggerNames.parameterSetID()) {
        hltPSetID_ = triggerNames.parameterSetID();
        assert(hltPaths_.size() == hltPathNames_.size());
        assert(hltPaths_.size() == hltPathIndices_.size());

        //for (unsigned i=0; i<triggerNames.size(); ++i) {
        //    std::cout << "HLT trigger name: " << triggerNames.triggerName(i) << std::endl;
        //}

        for (unsigned j=0; j<hltPaths_.size(); ++j) {
            std::string pattern = hltPaths_.at(j);
            pattern = replace_all(pattern, "v*", "v\\d+");  // change wildcard to regular expression
            std::regex re(pattern);

            bool found = false;
            for (unsigned i=0; i<triggerNames.size(); ++i) {
                const std::string& triggerName = triggerNames.triggerName(i);

                if (std::regex_match(triggerName, re)) {
                    found = true;
                    hltPathNames_.at(j) = triggerName;
                    hltPathIndices_.at(j) = i;
                    break;  // only take the first match
                }
            }

            if (found) {
                edm::LogInfo("NtupleTriggerResults") << "Found HLT path: " << hltPathNames_.at(j);
            } else {
                edm::LogWarning("NtupleTriggerResults") << "Cannot find HLT path: " << hltPaths_.at(j);
                hltPathIndices_.at(j) = triggerNames.size();
            }
        }
    }
}

void NtupleTriggerResults::syncL1TPaths(const edm::EventSetup& iSetup) {
    const L1TUtmTriggerMenuRcd& menuRcd = iSetup.get<L1TUtmTriggerMenuRcd>();
    unsigned long long cacheId = menuRcd.cacheIdentifier();
    if (l1tCacheID_ != cacheId) {
        l1tCacheID_ = cacheId;

        edm::ESHandle<L1TUtmTriggerMenu> l1tMenuHandle;
        menuRcd.get(l1tMenuHandle);

        if (l1tMenuHandle.isValid()) {
            const std::map<std::string, L1TUtmAlgorithm>& l1tAlgorithmMap = l1tMenuHandle->getAlgorithmMap();

            //for (auto const & keyval: l1tAlgorithmMap) {
            //    const std::string & name  = keyval.second.getName();
            //    unsigned int        index = keyval.second.getIndex();
            //    std::cout << "L1T algo: " << keyval.first << " " << name << " " << index << std::endl;
            //}

            for (unsigned j=0; j<l1tPaths_.size(); ++j) {
                std::string path = l1tPaths_.at(j);

                bool found = false;
                if (l1tAlgorithmMap.find(path) != l1tAlgorithmMap.end()) {
                    found = true;
                    const L1TUtmAlgorithm& algo = l1tAlgorithmMap.at(path);
                    l1tPathNames_.at(j) = algo.getName();
                    l1tPathIndices_.at(j) = algo.getIndex();
                }

                if (found) {
                    edm::LogInfo("NtupleTriggerResults") << "Found L1T path: " << l1tPathNames_.at(j);
                } else {
                    edm::LogWarning("NtupleTriggerResults") << "Cannot find L1T path: " << l1tPaths_.at(j);
                    hltPathIndices_.at(j) = GlobalAlgBlk::maxPhysicsTriggers;
                }
            }
        }
    }
}

// _____________________________________________________________________________
NtupleTriggerResults::NtupleTriggerResults(const edm::ParameterSet& iConfig) :
  hltTag_        (iConfig.getParameter<edm::InputTag>("hltTag")),
  l1tTag_        (iConfig.getParameter<edm::InputTag>("l1tTag")),
  prefix_        (iConfig.getParameter<std::string>("prefix")),
  suffix_        (iConfig.getParameter<std::string>("suffix")),
  hltPSetID_     (),
  hltPaths_      (iConfig.getParameter<std::vector<std::string> >("hltPaths")),
  hltPathNames_  (hltPaths_.size(), ""),
  hltPathIndices_(hltPaths_.size(), 0),
  l1tCacheID_    (0ULL),
  l1tPaths_      (iConfig.getParameter<std::vector<std::string> >("l1tPaths")),
  l1tPathNames_  (l1tPaths_.size(), ""),
  l1tPathIndices_(l1tPaths_.size(), 0) {

    hltToken_ = consumes<edm::TriggerResults>(hltTag_);
    l1tToken_ = consumes<GlobalAlgBlkBxCollection>(l1tTag_);

    produces<std::vector<bool> >              (prefix_ + "hltBits"         + suffix_);
    produces<std::vector<bool> >              (prefix_ + "l1tBits"         + suffix_);
}

NtupleTriggerResults::~NtupleTriggerResults() {}

void NtupleTriggerResults::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<bool> >              v_hltBits         (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v_l1tBits         (new std::vector<bool>());

    //__________________________________________________________________________
    edm::Handle<edm::TriggerResults> hltResults;
    //iEvent.getByLabel(hltTag_, hltResults);
    if (!hltToken_.isUninitialized())
        iEvent.getByToken(hltToken_, hltResults);

    if (hltResults.isValid()) {
        edm::LogInfo("NtupleTriggerResults") << "Size: " << hltResults->size();

        const edm::TriggerNames & triggerNames = iEvent.triggerNames(*hltResults);

        syncHLTPaths(triggerNames);

        unsigned n = 0;
        for (unsigned j=0; j<hltPaths_.size(); ++j) {
            bool accept = false;
            const std::string& hltPathName = hltPathNames_.at(j);
            const unsigned hltPathIndex = hltPathIndices_.at(j);
            if (hltPathIndex != triggerNames.size()) {
                assert(hltPathName == triggerNames.triggerName(hltPathIndex));
                accept = hltResults->accept(hltPathIndex);
            }

            // Fill the vectors
            v_hltBits->push_back(accept);

            n++;
        }

    } else {
        edm::LogError("NtupleTriggerResults") << "Cannot get the product: " << hltTag_;
    }

    //__________________________________________________________________________
    edm::Handle<GlobalAlgBlkBxCollection> l1tAlgoBlocks;
    //iEvent.getByLabel(l1tTag_, l1tAlgoBlocks);
    if (!l1tToken_.isUninitialized())
        iEvent.getByToken(l1tToken_, l1tAlgoBlocks);

    if (l1tAlgoBlocks.isValid()) {
        edm::LogInfo("NtupleTriggerResults") << "Size: " << l1tAlgoBlocks->size();

        if (!l1tAlgoBlocks->isEmpty(0)) {
            const GlobalAlgBlk& l1tAlgoBlock = l1tAlgoBlocks->at(0, 0);  // for BX = 0

            syncL1TPaths(iSetup);

            unsigned n = 0;
            for (unsigned j=0; j<l1tPaths_.size(); ++j) {
                bool accept = false;
                //const std::string& l1tPathName = l1tPathNames_.at(j);
                const unsigned l1tPathIndex = l1tPathIndices_.at(j);
                if (l1tPathIndex != GlobalAlgBlk::maxPhysicsTriggers) {
                    accept = l1tAlgoBlock.getAlgoDecisionFinal(l1tPathIndex);
                }

                // Fill the vectors
                v_l1tBits->push_back(accept);

                n++;
            }
        }

    } else {
        edm::LogError("NtupleTriggerResults") << "Cannot get the product: " << l1tTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_hltBits         , prefix_ + "hltBits"         + suffix_);
    iEvent.put(v_l1tBits         , prefix_ + "l1tBits"         + suffix_);
}
