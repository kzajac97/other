#ifndef AREA_HH
#define AREA_HH

#pragma once

#include <tuple>
#include <cmath>
#include <array>
#include <vector>

#include "point.hh"

template <class T>
class Area
{
	// border points representing square 
	// exnlosing all points in point set 
	private:
    Point<T> bottomLeft;
    Point<T> bottomRight;
    Point<T> topLeft;
    Point<T> topRight;

	// 2D vector of T (numeric type)
	// contaitning id of closest point
	public:
	std::vector<std::vector<unsigned int> > distances;

	public:
    // constructors
	Area(void) {};
    // Initialize with 4 points 
    Area(Point<T> bottomLeft, Point<T> bottomRight, Point<T> topLeft,Point<T>  topRight) 
    : bottomLeft(bottomLeft), bottomRight(bottomRight), topLeft(topLeft), topRight(topRight) {};
    // Initialize with a tuple 
    Area(std::tuple<Point<T>,Point<T>,Point<T>,Point<T>> initTuple) 
    : bottomLeft(std::get<0>(initTuple)), bottomRight(std::get<1>(initTuple)), 
      topLeft(std::get<2>(initTuple)), topRight(std::get<3>(initTuple)) {};

    ~Area(void) {};

	// Getters 
	const Point<T> get_bottomLeft(void) { return this->bottomLeft; }
	const Point<T> get_bottomRight(void) { return this->bottomRight; }
	const Point<T> get_topLeft(void) { return this->topLeft; }
	const Point<T> get_topRight(void) { return this->topRight; }

	const unsigned int get_x_size(void) { return std::abs(this->topRight.get_x()); }
	const unsigned int get_y_size(void) { return std::abs(this->topRight.get_y()); }
	
	// Setters

	// setters resize initially empty 2D vector in area object 
	// so it can be filled with distance values later 
	void set_x_size(unsigned int x) { this->distances.resize(x); }
	void set_y_sizes(unsigned int y) { for(auto &it : this->distances) { it.resize(y); } }
	//simple setters 
	void set_distance(unsigned int distance, unsigned int x, unsigned int y) { this->distances[x][y] = distance; }
	void set_bottomLeft(const Point<T> P) { this->bottomLeft = P; }
	void set_bottomRight(const Point<T> P) { this->bottomRight = P; }
	void set_topLeft(const Point<T> P) { this->topLeft = P; }
	void set_topRight(const Point<T> P) { this->topRight = P; }
	// sets all border points from tuple, order has to be the same 
	void set_borders(const std::tuple<Point<T>,Point<T>,Point<T>,Point<T>> borderTuple)
	{
		this->bottomLeft = std::get<0>(borderTuple);
		this->bottomRight = std::get<1>(borderTuple);
		this->topLeft = std::get<2>(borderTuple);
		this->topRight = std::get<3>(borderTuple);
	}

	//print functions
	void printBorders(void)
	{
		this->bottomLeft.printPoint();
		this->bottomRight.printPoint();
		this->topLeft.printPoint();
		this->topRight.printPoint();
	}

	void printDistances(void)
	{
		for(auto it : this->distances)
		{
			// print distances in y vector
			for(auto jt : it) { std::cout << jt << " "; }
			std::cout << "\n";
		} 
	}
	// calculates if given area is inifite
	// are is infinite when point in closest 
	// to any of enclosing square borders 
	bool isInfiniteArea(Point<T> P)
	{
		auto x = P.get_x();
		auto y = P.get_y();
		// if point's x or y coordinate is equal to any of 
		// the border points coordinate's it's part of a inifite area
		if(x == topLeft.get_x() && x == topRight.get_x() 
		   && y == topLeft.get_y() && y == bottomLeft.get_y() )
		{ return true; }

		else { return false; }
	}

	unsigned int getPointArea(Point<T> P)
	{
		auto id = P.get_id();
		unsigned int area = 0;
		for(unsigned int x=0; x < this->distances.size(); ++x)
		{
			for(unsigned int y=0; y < this->distances[x].size(); ++y)
			{
				Point<T> p({(int)x,(int)y});
				if(this->distances[x][y] == id) { area++; }
				if(isInfiniteArea(p)) { return 0; }
			}
		}

		return area;
	}

	unsigned int getPointsWithCorrectDistance(const unsigned int maxDistance)
	{
		unsigned int sum;
		for(auto x : this->distances)
		{
			for(auto y : x)
			{
				if(y < maxDistance) { sum++; }
			}
		}

		return sum;
	}
};

#endif