#ifndef SPACE_HH
#define SPACE_HH

#pragma once

#include <vector>

#include "point.hh"

template <class T>
class Space
{
    private:
    // border points
    const int lowerBorderX;
    const int lowerBorderY;
    const int upperBorderX;
    const int upperBorderY;
    // 2D vector of int's representing 
    // distance to nearest point for all area
    std::vector<std::vector<T> > grid;
    public:
    // constructors 
    Space(void) = default;
    // initialize with initializer_list
    Space(std::initializer_list<T> init);
    // initialize with tuple of borders
    Space(std::tuple<T,T,T,T> init_tuple) 
    : bottomLeft(std::get<0>(initTuple)), bottomRight(std::get<1>(initTuple)), 
      topLeft(std::get<2>(initTuple)), topRight(std::get<3>(initTuple)) {};
    // initiazlie with vector of borders
    Space(std::vector<T> init);
    ~Space(void) {};

    // getters and setters
    std::vector<std::vector<T> > get_grid(void) { return this->grid; }
    

};

#endif