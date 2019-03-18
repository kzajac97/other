#include <string>
#include <exception>
// Utility to be able to switch strings 
namespace util
{
    // Enumarate possibile metric 
    enum metric_code 
    {
        eManchattan,
        eEuclidean,
        eChebyshev,
        eMinkowski,
        eCanberra,
        eCosine
    };

    // Return enum value if input string is equal 
    metric_code hash_metric(const std::string & inString)
    {
        if(inString == "Manchattan") { return eManchattan; }
        if(inString == "Euclidean") { return eEuclidean; }
        if(inString == "Chebyshev") { return eChebyshev; }
        if(inString == "Minkowski") { return eMinkowski; }
        if(inString == "Canberra") { return eCanberra; }
        if(inString == "Cosine") { return eCosine; }
        else { throw std::invalid_argument("Unrecognized option"); }
    }
}