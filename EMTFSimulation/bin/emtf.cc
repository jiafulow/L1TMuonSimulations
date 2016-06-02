#include "L1TMuonSimulations/EMTFSimulation/interface/StubSelector.h"
#include "L1TMuonSimulations/EMTFSimulation/interface/PatternGenerator.h"

#include "boost/program_options.hpp"
#include <cstdlib>
#include <fstream>

// _____________________________________________________________________________
int main(int argc, char **argv) {

    // Log the input arguments
    std::cout << "Command:" << std::endl;
    std::cout << argv[0] << " ";
    for(int i = 1; i < argc-1; ++i) {
        std::cout << argv[i] << " ";
    }
    std::cout << argv[argc-1] << std::endl << std::endl;

    // _________________________________________________________________________
    // Register the program options

    // Create a struct that holds all input arguments
    ProgramOption option;

    // Declare a group of options that will be allowed only on command line
    namespace po = boost::program_options;
    po::options_description generic("Generic options");
    generic.add_options()
        ("version"              , "Print version")
        ("help,h"               , "Produce help message")
        ("selectStubs,S"        , "Select one unique stub per layer")
        ("generateBank,B"       , "Generate associative memory pattern bank")
        ;

    // Declare a group of options that will be allowed both on command line
    // and in config file
    po::options_description config("Configuration");
    config.add_options()
        ("input,i"      , po::value<std::string>(&option.input)->required(), "Specify input files")
        ("output,o"     , po::value<std::string>(&option.output)->required(), "Specify output file")
        ("bank,b"       , po::value<std::string>(&option.bankfile), "Specify pattern bank file")

        ("verbosity,v"  , po::value<int>(&option.verbose)->default_value(1), "Verbosity level (-1 = very quiet; 0 = quiet, 1 = verbose, 2+ = debug)")
        ("maxEvents,n"  , po::value<long long>(&option.maxEvents)->default_value(-1), "Specfiy max number of events")

        // MC truth
        ("minPt"        , po::value<float>(&option.minPt)->default_value(     3.0), "Specify min pt")
        ("maxPt"        , po::value<float>(&option.maxPt)->default_value(999999.0), "Specify max pt")
        ("minInvPt"     , po::value<float>(&option.minInvPt)->default_value(-999999.0), "Specify min signed 1/pt")
        ("maxInvPt"     , po::value<float>(&option.maxInvPt)->default_value( 999999.0), "Specify max signed 1/pt")
        ("minEta"       , po::value<float>(&option.minEta)->default_value(-2.5), "Specify min eta (signed)")
        ("maxEta"       , po::value<float>(&option.maxEta)->default_value( 2.5), "Specify max eta (signed)")
        ("minPhi"       , po::value<float>(&option.minPhi)->default_value(-M_PI), "Specify min phi (from -pi to pi)")
        ("maxPhi"       , po::value<float>(&option.maxPhi)->default_value( M_PI), "Specify max phi (from -pi to pi)")
        ("minVz"        , po::value<float>(&option.minVz)->default_value(-300.), "Specify min vertex z (cm)")
        ("maxVz"        , po::value<float>(&option.maxVz)->default_value( 300.), "Specify max vertex z (cm)")

        // Trigger tower selection
        ("tower,t"      , po::value<unsigned>(&option.tower)->default_value(27), "Specify the trigger tower")

        // Trigger sector selection
        ("sector"       , po::value<unsigned>(&option.sector)->default_value(0), "Specify the trigger sector")

        // Superstrip definition
        ("superstrip,s" , po::value<std::string>(&option.superstrip)->default_value("ss4"), "Specify the superstrip definition (default: ss4)")

        // Track fitting algorithm
        ("fitter,f"     , po::value<std::string>(&option.fitter)->default_value("PCA4"), "Select track fitter -- PCA4: PCA fitter 4 params; PCA5: PCA fitter 5 params (default: PCA4)")

        // Only for bank generation
        ("minPopularity" , po::value<int>(&option.minPopularity)->default_value(1), "Specify min popularity of a pattern to be stored or read")
        ("maxCoverage"  , po::value<float>(&option.maxCoverage)->default_value(1.0), "Specify max coverage of patterns to be stored or read")
        ;

    // Hidden options, will be allowed both on command line and in config file,
    // but will not be shown to the user.
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("cfg"          , po::value<std::vector<std::string> >(), "Specify config file")
        ;

    // Further group the options
    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description cfgfile_options;
    cfgfile_options.add(config).add(hidden);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    // Allow a positional option, which does not specify any name at all
    po::positional_options_description p;
    p.add("cfg", -1);


    // _________________________________________________________________________
    // Parse program options

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        if (vm.count("help") || argc==1) {
            std::cout << visible << std::endl;
            return EXIT_SUCCESS;
        }

        if (vm.count("cfg")) {
            const std::vector<std::string>& cfgfiles = vm["cfg"].as<std::vector<std::string> >();
            for (unsigned i=0; i<cfgfiles.size(); ++i) {
                std::ifstream ifs(cfgfiles.at(i).c_str());
                po::store(po::parse_config_file(ifs, cfgfile_options), vm);
            }
        }

        po::notify(vm);
    } catch (const boost::program_options::error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Exactly one of these options must be selected
    int vmcount = vm.count("selectStubs")       +
                  vm.count("generateBank")      ;
    if (vmcount != 1) {
        std::cerr << "ERROR: Must select exactly one option: '-B'" << std::endl;
        //std::cout << visible << std::endl;
        return EXIT_FAILURE;
    }

    // Update options
    if (option.maxEvents < 0)
        option.maxEvents = std::numeric_limits<long long>::max();

    // Add options
    option.datadir = std::getenv("CMSSW_BASE");
    option.datadir += "/src/L1TMuonSimulations/EMTFSimulation/data/";

    if (option.verbose>1)
        std::cout << option << std::endl << std::endl;


    // _________________________________________________________________________
    // Call the producers

    if (vm.count("selectStubs")) {
        std::cout << "Start stub selection..." << std::endl;

        StubSelector selector(option);
        selector.init();
        selector.run();
        std::cout << "DONE" << std::endl;

    } else if (vm.count("generateBank")) {
        std::cout << "Start pattern bank generation..." << std::endl;

        PatternGenerator generator(option);
        generator.init();
        generator.run();
        std::cout << "DONE" << std::endl;
    }

    return EXIT_SUCCESS;
}
