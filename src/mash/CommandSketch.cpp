#include "CommandSketch.h"
#include "Index.h"

using namespace::std;

CommandSketch::CommandSketch()
: Command()
{
    name = "sketch";
    description = "Create a reference index";
    argumentString = "fast(a|q)[.gz] ...";
    
    useOption("help");
    useOption("kmer");
    useOption("windowed");
    useOption("window");
    useOption("mins");
    useOption("minsWindowed");
    useOption("verbose");
    useOption("silent");
    addOption("prefix", Option(Option::File, "o", "Output prefix (first input file used if unspecified). The suffix '.mash' will be appended.", ""));
}

int CommandSketch::run() const
{
    if ( arguments.size() == 0 || options.at("help").active )
    {
        print();
        return 0;
    }
    
    int kmer = options.at("kmer").getArgumentAsNumber(1, 32);
    bool windowed = options.at("windowed").active;
    int minHashesPerWindow = windowed ? options.at("minsWindowed").getArgumentAsNumber() : options.at("mins").getArgumentAsNumber();
    int windowSize = options.at("window").getArgumentAsNumber();
    int verbosity = options.at("silent").active ? 0 : options.at("verbose").active ? 2 : 1;
    
    Index index;
    
    index.initFromSequence(arguments, kmer, minHashesPerWindow, windowed, windowSize, false, verbosity);
    
    string prefix = options.at("prefix").argument.length() > 0 ? options.at("prefix").argument : arguments[0];
    
    index.writeToCapnp((prefix + ".mash").c_str());
    
    return 0;
}