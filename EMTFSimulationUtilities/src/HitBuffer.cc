#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/HitBuffer.h"
#include <cassert>
#include <sstream>
#include <iostream>

namespace {
// Join 'layer' and 'superstrip' into one number
inline unsigned simpleHash(unsigned nss, unsigned layer, unsigned ss) {  // as simple as it gets
    return nss * layer + ss;
}
}  // namespace

namespace phasetwoemtf {

// _____________________________________________________________________________
void HitBuffer::init(unsigned nlayers, unsigned nss) {
    nlayers_ = nlayers;
    nss_ = nss;

    superstripHits_.clear();

    superstripBools_.clear();
    superstripBools_.resize(nlayers_ * nss_);

    assert(nlayers_ != 0);
    assert(nss_ != 0);
    assert(superstripBools_.size() != 0);
}

// _____________________________________________________________________________
void HitBuffer::reset() {
    superstripHits_.clear();

    std::fill(superstripBools_.begin(), superstripBools_.end(), 0);
}

// _____________________________________________________________________________
void HitBuffer::insert(unsigned layer, superstrip_t ssId, unsigned stubRef) {
    const unsigned hash = simpleHash(nss_, layer, ssId);
    superstripHits_[hash].push_back(stubRef);
    superstripBools_.at(hash) = true;
}

// _____________________________________________________________________________
void HitBuffer::freeze(unsigned maxStubs) {
    for (std::map<superstrip_t, std::vector<unsigned> >::iterator it=superstripHits_.begin();
         it!=superstripHits_.end(); ++it) {

        if (it->second.size() > maxStubs) {
            //it->second.resize(maxStubs);  // keep first N stubs

            int n = it->second.size() - maxStubs;
            it->second.erase(it->second.begin(), it->second.begin() + n);
            assert(it->second.size() == maxStubs);
        }
    }

    frozen_ = true;
}

// _____________________________________________________________________________
bool HitBuffer::hasHits(unsigned layer, superstrip_t ssId) const {
    const unsigned hash = simpleHash(nss_, layer, ssId);
    return superstripBools_.at(hash);
}

// _____________________________________________________________________________
std::vector<unsigned> HitBuffer::getHits(unsigned layer, superstrip_t ssId) const {
    const unsigned hash = simpleHash(nss_, layer, ssId);
    return superstripHits_.at(hash);
}

// _____________________________________________________________________________
std::string HitBuffer::str() const {
    std::ostringstream o;
    o << "nlayers: " << nlayers_ << " nss: " << nss_;
    return o.str();
}

}  // namespace phasetwoemtf

