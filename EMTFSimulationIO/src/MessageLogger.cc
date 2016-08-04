#include "L1TMuonSimulations/EMTFSimulationIO/interface/MessageLogger.h"

namespace phasetwoemtf {

// Global accessor
LogTools& logTools() {  return LogTools::getInstance(); }

// Stolen from $ROOTSYS/tmva/src/Tools.cxx
const std::string& LogTools::Color(const std::string& c) const {
    // human readable color strings
    static const std::string gClr_none         = "" ;
    static const std::string gClr_white        = "\033[1;37m";  // white
    static const std::string gClr_black        = "\033[30m";    // black
    static const std::string gClr_blue         = "\033[34m";    // blue
    static const std::string gClr_red          = "\033[1;31m" ; // red
    static const std::string gClr_yellow       = "\033[1;33m";  // yellow
    static const std::string gClr_darkred      = "\033[31m";    // dark red
    static const std::string gClr_darkgreen    = "\033[32m";    // dark green
    static const std::string gClr_darkyellow   = "\033[33m";    // dark yellow

    static const std::string gClr_bold         = "\033[1m"    ; // bold
    static const std::string gClr_black_b      = "\033[30m"   ; // bold black
    static const std::string gClr_lblue_b      = "\033[1;34m" ; // bold light blue
    static const std::string gClr_magenta_b    = "\033[1;35m" ; // bold magenta
    static const std::string gClr_cyan_b       = "\033[0;36m" ; // bold cyan
    static const std::string gClr_gray_b       = "\033[0;37m" ; // bold gray
    static const std::string gClr_lgreen_b     = "\033[1;32m";  // bold light green

    static const std::string gClr_blue_bg      = "\033[44m";    // blue background
    static const std::string gClr_red_bg       = "\033[1;41m";  // white on red background
    static const std::string gClr_whiteonblue  = "\033[1;44m";  // white on blue background
    static const std::string gClr_whiteongreen = "\033[1;42m";  // white on green background
    static const std::string gClr_grey_bg      = "\033[47m";    // grey background

    static const std::string gClr_reset  = "\033[0m";     // reset

    if (!useColor_)            return gClr_none;

    if (c == "white" )         return gClr_white;
    if (c == "blue"  )         return gClr_blue;
    if (c == "black"  )        return gClr_black;
    if (c == "magenta")        return gClr_magenta_b;
    if (c == "lightblue")      return gClr_cyan_b;
    if (c == "gray"  )         return gClr_gray_b;
    if (c == "yellow")         return gClr_yellow;
    if (c == "red"   )         return gClr_red;
    if (c == "dred"  )         return gClr_darkred;
    if (c == "dgreen")         return gClr_darkgreen;
    if (c == "lgreenb")        return gClr_lgreen_b;
    if (c == "dyellow")        return gClr_darkyellow;

    if (c == "bold")           return gClr_bold;
    if (c == "bblack")         return gClr_black_b;

    if (c == "blue_bgd")       return gClr_blue_bg;
    if (c == "red_bgd" )       return gClr_red_bg;

    if (c == "white_on_blue" ) return gClr_whiteonblue;
    if (c == "white_on_green") return gClr_whiteongreen;

    if (c == "reset")          return gClr_reset;
    return gClr_none;
}

const std::string& LogTools::EndColor() const {
    static std::string gClr_none         = "" ;
    static std::string gClr_reset        = "\033[0m";     // reset
    if (!useColor_)            return gClr_none;
    return gClr_reset;
}

void LogTools::UseColor(bool use) {
    useColor_ = use;
}

std::string LogTools::Error() const {
    return "[" + Color("red") + "ERROR" + Color("reset") + "  ] ";
}

std::string LogTools::Warning() const {
    return "[" + Color("yellow") + "WARNING" + Color("reset") + "] ";
}

std::string LogTools::Info() const {
    return "[" + Color("lightblue") + "INFO" + Color("reset") + "   ] ";
}

std::string LogTools::Debug() const {
    return "[" + Color("gray") + "DEBUG" + Color("reset") + "  ] ";
}

}  // namespace phasetwoemtf
