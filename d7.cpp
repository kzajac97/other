#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>

#include "graph.hh"

using type_t = char; // alias for labels type in a graph

// reads text file with name passed as const string
// outputs vector of some numeric type
std::vector<type_t> readFile(const std::string & inputString)
{
    std::ifstream inputFile(inputString);
    std::vector<type_t> outputVec;
    std::istream_iterator<type_t> input(inputFile); // copy file contents with auto iterator
    std::copy(input, std::istream_iterator<type_t>(),std::back_inserter(outputVec));

    return outputVec;
}

// creates graph form inputData vector
graph<type_t> createGraph(const std::vector<type_t> & inputData)
{
    graph<type_t> g;
    
    // add all vertexes looping every element
    for(unsigned int i=0; i < inputData.size(); ++i)
    { g.AddVertex(inputData[i]); } 

    // add directed egdes looping by two elements
    for(unsigned int j=0; j < inputData.size(); j += 2)
    { g.AddEdge(inputData[j],inputData[j+1]); }

    return g;
}

// creates directed graph with reversed edge dependency
graph<type_t> createReversedGraph(const std::vector<type_t> & inputData)
{
    graph<type_t> g;
    
    for(unsigned int i=0; i < inputData.size(); ++i)
    { g.AddVertex(inputData[i]); }

    for(unsigned int j=0; j < inputData.size(); j += 2)
    { g.AddEdge(inputData[j+1],inputData[j]); }

    return g;
}

graph<type_t> createGraphWithPrerequisites(const std::vector<type_t> & inputData)
{
    graph<type_t> g;
    
    // add all vertexes looping every element
    for(unsigned int i=0; i < inputData.size(); ++i)
    { g.AddVertex(inputData[i]); } 

    // add directed egdes looping by two elements
    for(unsigned int j=0; j < inputData.size(); j += 2)
    { 
        g.AddEdge(inputData[j],inputData[j+1]); 
        // preequisite is like reversed edge in directed graph
        g.AddPrerequisite(inputData[j+1],inputData[j]); 
    }

    return g;
}

int main(void)
{
    const auto inputData = readFile("datad7.txt"); // read input data to string
    auto graph = createGraph(inputData);

    auto result = graph.lexigraphicalTopologicalSort('C'); 
    for(const auto it : result) { std::cout << it; }
    std::cout << "\n";
}