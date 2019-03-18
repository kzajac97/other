#include <iostream>
#include <fstream>
#include <iterator>
#include <utility>
#include <vector>
#include <tuple>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/adjacency_iterator.hpp>

// aliases for using directed graph with vector containers
// with char as vertex element and iterators
using type_t = char;
using graph_t = boost::adjacency_list<boost::vecS,boost::vecS,boost::directedS>;
using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
using edge_iterator_t = boost::graph_traits<graph_t>::edge_iterator;
using vertex_iterator_t = boost::graph_traits<graph_t>::vertex_iterator;
using index_map_t = std::map<vertex_t,size_t>;

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

// creates directed graph using boost::graph library
// uses adjacency_list with vectors as containers
std::tuple<graph_t,index_map_t> createGraph(const std::vector<type_t> & inputData)
{
    graph_t graph;
    index_map_t indices; 
    std::vector<vertex_t> vertices;
    unsigned int index_iterator = 0;

    // create vertices
    for(unsigned int i=0; i < inputData.size(); i += 2)
    { vertices.push_back(boost::add_vertex(graph)); }

    for(const auto it : vertices)
    { 
        indices[it] = inputData[2*index_iterator + 1];
        index_iterator++; 
    }

    // loop iterates by 2 elements creating edges in graph
    for(unsigned int i=0; i < inputData.size(); i += 2)
    { boost::add_edge(inputData[i],inputData[i+1],graph); }

    return (graph,indices);
}

int main(void)
{
    const auto inputData = readFile("datad7.txt"); // read input data to string
    const auto graph = createGraph(inputData); // boost::graph
    graph_t graph;
    index_map_t indexes;

    vertex_t v0 = boost::add_vertex(graph);
    vertex_t v1 = boost::add_vertex(graph);
    vertex_t v2 = boost::add_vertex(graph);
    vertex_t v3 = boost::add_vertex(graph);

    indexes[v0] = 0;
    indexes[v1] = 1;
    indexes[v2] = 2;
    indexes[v3] = 3;

    boost::add_edge(v0,v2,graph);
    boost::add_edge(v1,v3,graph);
    boost::add_edge(v1,v0,graph);

    for(auto vi = boost::vertices(graph); vi.first != vi.second; ++vi.first)
    {
        std::cout << *vi.first << "\n";
    }
}