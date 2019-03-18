#ifndef POINT_HH
#define POINT_HH

#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <exception>
#include <algorithm>

#include "util.hh"

template<class T>
class Point
{
    private:
    std::vector<T> coordinates; 
    unsigned int id; // point numeric id
    public:
    Point(void) = default;
    Point(std::initializer_list<T> init) : coordinates(init) {};
    Point(std::vector<T> init) { for(auto x : init) { coordinates.push_back(x); } }
    Point(unsigned int init_id,std::initializer_list<T> init) : id(init_id),coordinates(init) {};
    Point(unsigned int init_id,std::vector<T> init) { this->id = init_id; for(auto x : init) { coordinates.push_back(x); } }
    ~Point(void) {this->coordinates.clear(); }

    // Getters  
    const unsigned int get_id(void) { return this->id; }
    const T get_x(void) { return this->coordinates[0]; }
    const T get_y(void) { return this->coordinates[1]; }
    const T get_z(void) { return this->coordinates[2]; }
    const T get_coordinate(unsigned int index) { return this->coordinates[index]; }
    const std::vector<T> get_coordinates(void) { return this->coordinates; }
    // Setters 
    void set_id(const unsigned int id) { this->id = id; }
    void set_x(const T x) { this->coordinates[0] = x; }
    void set_y(const T y) { this->coordinates[1] = y; }
    void set_z(const T z) { this->coordinates[2] = z; }
    void set_coordinate(const T coordinate, unsigned int index) { this->coordinates[index] = coordinate; }
    void set_coordinates(std::initializer_list<T> init) 
    { 
        this->coordinates.clear(); 

        for(auto x : init) 
        { this->coordinates.push_back(x); } 
    }

    // Operators 
    const bool operator == (const Point P) { return this->coordinates == P.coordinates; }
    const bool operator != (const Point P) { return this->coordinates != P.coordinates; }
    Point & operator = (Point P) { this->coordinates = P.coordinates; }
    // Utlity Functions
    // prints point with id
    const void printPointAll(void)
    {
        std::cout << this->id << "\t"; // print point id
        std::cout << "("; // print bracket 
        for(auto it = this->coordinates.begin(); it != this->coordinates.end(); ++it) 
        {
            if(it != this->coordinates.end() - 1) { std::cout << *it << ","; } 
            else { std::cout << *it; } // last item doesn't need a comma after it
        } 

        std::cout << ")\n"; // print bracket after last element
    }

    const void printPointID(void) { std::cout << this->id << "\n"; }
    // prints point without id
    const void printPoint(void)
    {
        std::cout << "("; // print bracket 
        for(auto it = this->coordinates.begin(); it != this->coordinates.end(); ++it) 
        {
            if(it != this->coordinates.end() - 1) { std::cout << *it << ","; } 
            else { std::cout << *it; } // last item doesn't need a comma after it
        } 

        std::cout << ")\n"; // print bracket after last element
    }

    // Math Functions 
    const T getDistance(Point P, std::string metric = "Manchattan")
    {
        switch(util::hash_metric(metric))
        {
            case util::eManchattan: return this->getManchattanDistance(P); break;
            case util::eEuclidean: return this->getEuclideanDistance(P); break;
            case util::eChebyshev: return this->getChebyshevDistance(P); break;
            case util::eMinkowski: return this->getMinkowskiDistance(P); break;
            case util::eCanberra: return this->getCanberraDistance(P); break;
            case util::eCosine: return this->getCosineDistance(P); break;
            default: return this->getManchattanDistance(P); break;
        }
    }

    private:
    const T getManchattanDistance(Point P) 
    {
        if(this->coordinates.size() != P.coordinates.size())
        { throw std::invalid_argument("Points of not equal Dimensions"); }
        // Dimensions are equal
        else
        { 
            T distance;  
            for(unsigned int i=0; i < this->coordinates.size(); ++i)
            { distance += std::abs(this->coordinates[i] - P.coordinates[i]); }
            return distance;
        }
    }

    const T getEuclideanDistance(Point P)
    {
        if(this->coordinates.size() != P.coordinates.size())
        { throw std::invalid_argument("Points of not equal Dimensions"); }
        // Dimensions are equal
        else
        {
            T distance;    
            for(unsigned int i=0; i < this->coordinates.size(); ++i)
            { distance += std::pow(this->coordinates[i] - P.coordinates[i],2); } // sqaring doesn't change variable's type

            T distance_root = std::sqrt(distance); // implicit converion can lose precison
            return distance_root; 
        }
    }

    const T getChebyshevDistance(Point P)
    {
        if(this->coordinates.size() != P.coordinates.size())
        { throw std::invalid_argument("Points of not equal Dimensions"); }
        // Dimensions are equal
        else
        {
            std::vector<T> distance;    
            for(unsigned int i=0; i < this->coordinates.size(); ++i)
            { distance.push_back(std::abs(this->coordinates[i] - P.coordinates[i])); } 

            return *std::max_element(distance.begin(),distance.end()); // returned max distance along any dimension
        }
    }

    const T getMinkowskiDistance(Point P)
    { 
        if(this->coordinates.size() != P.coordinates.size())
        { throw std::invalid_argument("Points of not equal Dimensions"); }
        // Dimensions are equal
        else
        {
            const int p = 3; // normalization factor
            T distance;
            // Uses metric simlar to Euclidean but with 3 degree power and root  
            for(unsigned int i=0; i < this->coordinates.size(); ++i)
            { distance += std::abs(std::pow(this->coordinates[i] - P.coordinates[i],p)); } 

            T distance_root = std::pow(distance,(double)1/p); // can lose precision
            return distance_root;
        }
    }

    const T getCanberraDistance(Point P)
    { 
        if(this->coordinates.size() != P.coordinates.size())
        { throw std::invalid_argument("Points of not equal Dimensions"); }
        // Dimensions are equal
        else
        {
            T distance;    
            // Distance is absolute value of sum divided by sum of absolute values  
            for(unsigned int i=0; i < this->coordinates.size(); ++i)
            { distance += std::abs(this->coordinates[i] - P.coordinates[i])/(std::abs(this->coordinates[i]) + std::abs(P.coordinates[i])); } 

            return distance;
        }
    }
    
    const T getCosineDistance(Point P)
    { 
        if(this->coordinates.size() != P.coordinates.size())
        { throw std::invalid_argument("Points of not equal Dimensions"); }
        // Dimensions are equal
        else
        {
            T distance;    
            for(unsigned int i=0; i < this->coordinates.size(); ++i)
            { 
                distance += (this->coordinates[i] * P.coordinates[i])/(std::abs(this->coordinates[i]) * std::abs(P.coordinates[i]));
            } 

            T distance_root = 1 - distance; 
            return distance_root;
        }
    }

};

#endif