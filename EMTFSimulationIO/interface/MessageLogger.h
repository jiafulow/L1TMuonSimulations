#ifndef EMTFSimulationIO_MessageLogger_h_
#define EMTFSimulationIO_MessageLogger_h_

#include <iostream>
#include <sstream>
#include <string>
#include <memory>

#include "TString.h"

// _____________________________________________________________________________
// Global logging tools
//
// References:
// - Singleton in C++:
//     http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
// - TMVA Tools
//     https://root.cern.ch/doc/v606/Tools_8h.html

class LogTools {
public:
    static LogTools& getInstance() {
        static LogTools instance;
        return instance;
    }

    const TString& Color(const TString& c) const;
    const TString& EndColor() const;
    void UseColor(bool use=true);

    TString Error() const;
    TString Warning() const;
    TString Info() const;
    TString Debug() const;

private:
    bool useColor_;

private:
    LogTools() {
        useColor_ = true;
    }

public:
    LogTools(LogTools &)            = delete;
    void operator=(LogTools const&) = delete;
};

LogTools& logTools();  // global accessor


// _____________________________________________________________________________
// Log functions
//
// Threshold:
//   0 - Error
//   1 - Warning
//   2 - Info
//   3 - Debug
//
// References:
// - CMSSW MessageLogger
//    https://cmssdt.cern.ch/lxr/source/FWCore/MessageLogger/interface/MessageLogger.h

class LogError {
public:
    explicit LogError( std::string const & id, int verbose) {
        valid_ = (verbose >= 0);
        padding_ = logTools().Error().Data();
    }
    ~LogError() {
        if (valid_) {
            std::cout << padding_ << os_.str();
        }
    }

    template<class T>
    LogError &
    operator<< (T const& t);

    LogError &
    operator<< ( std::ostream&(*f)(std::ostream&) );

    LogError &
    operator<< ( std::ios_base&(*f)(std::ios_base&) );
protected:
    bool valid_;
    std::string padding_;
    std::stringstream os_;
};

class LogWarning : public LogError {
public:
    explicit LogWarning( std::string const & id, int verbose) : LogError(id, verbose) {
        valid_ = (verbose >= 1);
        padding_ = logTools().Warning().Data();
    }
    ~LogWarning() {}
};

class LogInfo : public LogError{
public:
    explicit LogInfo( std::string const & id, int verbose) : LogError(id, verbose) {
        valid_ = (verbose >= 2);
        padding_ = logTools().Info().Data();
    }
    ~LogInfo() {}
};

class LogDebug : public LogError {
public:
    explicit LogDebug( std::string const & id, int verbose) : LogError(id, verbose) {
        valid_ = (verbose >= 3);
        padding_ = logTools().Debug().Data();
    }
    ~LogDebug() {}
};

#define EMTFSimulationIO_MessageLogger_icc_
#include "L1TMuonSimulations/EMTFSimulationIO/interface/MessageLogger.icc"
#undef EMTFSimulationIO_MessageLogger_icc_

#endif
