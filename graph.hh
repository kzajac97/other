#ifndef GRAPH_HH
#define GRAPH_HH

#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <exception>
#include <queue>
#include <algorithm>

// simple structure holding nodes of graph
// it uses adjacency_list rpresentation with 
// vector as container
template <class T>
struct node 
{
    T index;
    std::vector<T> adjacency_list;
    std::vector<T> prerequisits;
    node(void) = default;
    node(T init_index) : index(init_index) {};
    ~node(void) { this->adjacency_list.clear(); }    
};

// class implementing simple directed graph
// with labeled nodes 
template <class T>
class graph
{
    std::vector<node<T> > vertices;

    public:
    graph(void) {};
    graph(std::vector<node<T> > init) : vertices(init) {};
    ~graph(void) { this->vertices.clear(); }
    
    // Adds vertex only when given label is not 
    // already on the graph
    void AddVertex(T vertex_label) 
    {
        // checks if label is already used
        for(auto it : this->vertices)
        {
            if(vertex_label == it.index)
            { return; }
        }
        // if element isn't on the graph
        vertices.push_back(node(vertex_label)); 
    }

    // adds directed edge, base index has the adjacent index
    // label added to it's adjacency vector 
    void AddEdge(T start_vertex_label, T end_vertex_label)
    {
        for(auto & it : this->vertices)
        {
            if(start_vertex_label == it.index)
            { it.adjacency_list.push_back(end_vertex_label); }
        }
    }

    void AddPrerequisite(T start_vertex_label, T end_vertex_label)
    {
        for(auto & it : this->vertices)
        {
            if(start_vertex_label == it.index)
            { it.prerequisits.push_back(end_vertex_label); }
        }
    }

    const void PrintVertices(void)
    {
        for(const auto it : this->vertices)
        { std::cout << it.index << " "; }
        
        std::cout << "\n";
    }

    const void PrintEdges(void)
    {
        for(const auto it : this->vertices)
        {
            // print all adjacent index labels
            for(const auto jt : it.adjacency_list) 
            { 
                if(it.adjacency_list.empty()) { std::cout << "\"" << it.index << "\","; }
                std::cout << "\"" <<it.index << "\"" << "->\"" << jt << "\", ";     
            }
            std::cout << "\n";
        }
    }

    const bool findInVector(std::vector<node<T> > vec, node<T> n)
    {
        for(const auto it : vec)
        {
            if(it.index == n.index)
            { return true; }
        }      

        return false;
    } 

    void depthFirstSearch(void)
    {
        std::stack<node<T> > stack;
        std::vector<node<T> > visited;
        node<T> vertex;

        stack.push(vertices[0]);

        while(!stack.empty())
        {
            vertex = stack.top();
            stack.pop();

            if(!findInVector(visited, vertex))
            { visited.push_back(vertex); }

            for(const auto it : vertex.adjVertices)
            { 
                for(const auto jt : vertices)
                {
                    if(it == jt.index && !findInVector(visited,jt))
                    { stack.push(jt); }
                }
            }
        }

        for(const auto it : visited)
        { std::cout << it.index << "\n"; }
    }

    // returns lowest element of templte argument
    // elements could be numeric or char but should be built in type
    // <returns> elements value
    T getLowest(std::vector<T> elements)
    { return *std::min_element(elements.begin(),elements.end()); }

    std::vector<T> lexigraphicalTopologicalSort(T firstVertex)
    {
        std::vector<T> visited;
        std::vector<T> avialable;
        visited.push_back(firstVertex);

        makeAvialable(getAdjacencyList(visited[0]),avialable);

        while(!avialable.empty())
        {
            auto lowest = getLowest(avialable);

            if(prerequisitesComplete(visited,getPrerequistesList(lowest)))
            {
                visited.push_back(lowest);
                makeAvialable(getAdjacencyList(lowest),avialable);          
            }

            else
            {
                removeAvialable(lowest,avialable);
            }
            
        }

        return visited;
    }

    // finds adjacnecy_list for given vertex label
    // <returns> adjacency_list vector or empty vector 
    std::vector<T> getAdjacencyList(T vertexLabel)
    {
        std::vector<T> empty;

        for(auto it : this->vertices)
        {
            if(it.index == vertexLabel)
            { return it.adjacency_list; }
        }

        return empty;
    }

    // <returns> prerequisites list of given veretexLabel
    std::vector<T> getPrerequistesList(T vertexLabel)
    {
        std::vector<T> empty;

        for(auto it : this->vertices)
        {
            if(it.index == vertexLabel)
            { return it.prerequisits; }
        }

        return empty;
    }

    bool prerequisitesComplete(std::vector<T> visited, std::vector<T> prerequisites)
    {
        for(const auto it : prerequisites)
        {
            for(const auto jt : visited)
            {
                if(jt == it) { prerequisites.erase(prerequisites.begin() + it); }
            }
        }

        return prerequisites.empty() ? true : false;
    }

    void makeAvialable(std::vector<T> adjacent,std::vector<T> & avialable)
    {
        for(const auto & it : adjacent)
        { avialable.push_back(it); }
    }

    void removeAvialable(T element, std::vector<T> & avialable)
    {
        for(auto it : avialable)
        {
            if(it == element) { avialable.erase(avialable.begin() + it); }
        }
    }
};

#endif