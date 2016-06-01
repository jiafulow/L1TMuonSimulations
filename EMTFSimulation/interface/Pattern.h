#ifndef EMTFSimulation_Pattern_h_
#define EMTFSimulation_Pattern_h_

#include <stdint.h>
#include <array>
#include <iosfwd>

typedef uint32_t superstrip_type;
typedef std::array<superstrip_type,8> pattern_type;

// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const pattern_type& patt);

#endif

