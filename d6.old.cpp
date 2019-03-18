#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <sstream>
#include <iterator>
#include <tuple>
#include <map>
#include <future>

#include "point.hh"
#include "area.hh"

using type_t = int; // alias for int created for easy data type switching 

std::vector<type_t> readFile(std::string inputString)
{
    std::ifstream inputFile(inputString);
    std::vector<type_t> outputVec;
    std::istream_iterator<type_t> input(inputFile); // copy file contents with auto iterator
    std::copy(input, std::istream_iterator<type_t>(),std::back_inserter(outputVec));

    return outputVec;
}

// Creates 2D points from vector of ints 
std::vector<Point<type_t> > createPointsFromVector(std::vector<type_t> inputData)
{
    std::vector<Point<type_t> > output;

    /* iterates moving by two elements */
    for(unsigned int i=0; i < inputData.size(); i += 2)
    {
        Point<type_t> point({inputData[i],inputData[i+1]}); // Point created from two int's
        output.push_back(point);
    }

    return output;
}

std::tuple<type_t,type_t,type_t,type_t> getBorderPoints(std::vector<Point<type_t> > points)
{

    type_t highestX = points[0].get_x(); //set initial values
    type_t lowestX = points[0].get_x(); 
    type_t highestY = points[0].get_y();
    type_t lowestY = points[0].get_y();
    
    // iterate through points vector and save 
    // lowest and hightest X and Y vales 
    for(auto p : points)
    {
        if(p.get_x() > highestX) { highestX = p.get_x(); }
        else if(p.get_x() < lowestX) { lowestX = p.get_x(); }
        if(p.get_y() > highestY) { highestY = p.get_y(); }
        else if(p.get_y() < lowestY) { lowestY = p.get_y(); }
    }

    return {lowestX,highestX,lowestY,highestY}; // return tuple order is important
}

std::tuple<Point<type_t>,Point<type_t>,Point<type_t>,Point<type_t> > getNearestPoints(Point<type_t> P, std::vector<Point<type_t> > points)
{
    Point<type_t> bottomLeft({0,0}); // set initial values
    Point<type_t> bottomRight({1000,0});
    Point<type_t> topLeft({0,1000});
    Point<type_t> topRight({1000,1000});

    for(auto point : points)
    {
        if(point != P)
        {
            // point is in fisrt quarter with respect to P point as origin 
            if(point.get_x() >= P.get_x() && point.get_y() >= P.get_y())
            { if(P.getDistance(point) < P.getDistance(topRight)) { topRight = point; } }
            // Second quarter 
            else if(point.get_x() < P.get_x() && point.get_y() >= P.get_y())
            { if(P.getDistance(point) < P.getDistance(topRight)) { topLeft = point; } }
            // Third quarter 
            if(point.get_x() < P.get_x() && point.get_y() < P.get_y())
            { if(P.getDistance(point) < P.getDistance(topRight)) { bottomLeft = point; } }
            // Fourth quarter 
            else if(point.get_x() >= P.get_x() && point.get_y() < P.get_y())
            { if(P.getDistance(point) < P.getDistance(topRight)) { bottomRight = point; } }
        }
    }

    return {bottomLeft,bottomRight,topLeft,topRight}; // return as a tuple
}

// Extract rectangular area from border points 
Area<type_t> createArea(std::tuple<Point<type_t>,Point<type_t>,Point<type_t>,Point<type_t> > borderPoints)
{
    // Create vector from tuple 
    std::vector<Point<type_t> > points = 
    { std::get<0>(borderPoints), 
      std::get<1>(borderPoints), 
      std::get<2>(borderPoints), 
      std::get<3>(borderPoints) };

    auto borderCoordinates = getBorderPoints(points); // tuple of 4 points
    
    std::cout << "Local Borders: \n" << std::get<0>(borderCoordinates) << " " << std::get<1>(borderCoordinates)
    << " " << std::get<2>(borderCoordinates) << " " << std::get<3>(borderCoordinates) << " " ; 
    //  Coordinates on tuple are  
    //    0->left 1->right 2->bottom 3->top 
    Point<type_t> bottomLeft({std::get<0>(borderCoordinates),std::get<2>(borderCoordinates)}); //get coordinates returned from tuple
    Point<type_t> bottomRight({std::get<1>(borderCoordinates),std::get<2>(borderCoordinates)});
    Point<type_t> topLeft({std::get<0>(borderCoordinates),std::get<3>(borderCoordinates)});
    Point<type_t> topRight({std::get<1>(borderCoordinates),std::get<3>(borderCoordinates)});

    return Area<type_t>(bottomLeft,bottomRight,topLeft,topRight); // Order of points can't be changed
}

std::vector<Point<type_t> > getPointsInArea(Area<type_t> A)
{
    std::vector<Point<type_t> > pointsInArea;
    // Create all points in 2D grid enclosed by given area 
    for(type_t iterX = A.bottomLeft.get_x(); iterX <= A.bottomRight.get_x(); ++iterX)
    {
        for(type_t iterY = A.bottomLeft.get_y(); iterY <= A.topLeft.get_y(); ++iterY)
        {
            Point<type_t> point({iterX,iterY});
            pointsInArea.push_back(point);
        }
    }
    
    return pointsInArea;
}

bool isClosest(Point<type_t> searchedPoint, Point<type_t> basePoint,  std::vector<Point<type_t> > points)
{
    auto distanceToBase = basePoint.getDistance(searchedPoint);
    for(auto point : points)
    {
        if(searchedPoint.getDistance(point) < distanceToBase) 
        { return false; }
    }

    return true;
}

type_t getArea(Point<type_t> P, Area<type_t> A, std::vector<Point<type_t> > points)
{
    auto areaPoints = getPointsInArea(A); // 1D vector of points representing 2D grid
    type_t area = 0;
    for(auto x : areaPoints)
    { if(isClosest(x,P,points)) { area++; } }

    std::cout << "area: " << area << "\n"; 
    return area;
}

// Function executing all functions requiring computation 
type_t getAreaAsyncCaller(Point<type_t> P, std::vector<Point<type_t> > points)
{
    auto area = createArea(getNearestPoints(P,points));
    auto outputArea = getArea(P,area,points);

    return outputArea;
}

int main(void)
{
    auto inputData = readFile("datad6.csv");
    auto points = createPointsFromVector(inputData); // vector of points
    auto borders = getBorderPoints(points); // tuple of type_t

    std::cout << "Global Borders: " << std::get<0>(borders) << " " << std::get<1>(borders) << " " << std::get<2>(borders) << " " << std::get<3>(borders) << "\n" ; 
    std::vector<type_t> areas;

    for(auto x : points)
    {
        // if point has non-infinite area 
        std::cout << "Point x and y: " << x.get_x() << " " << x.get_y() << "\n";
        if(x.get_x() != std::get<0>(borders) && x.get_x() != std::get<1>(borders) 
        && x.get_y() != std::get<2>(borders) && x.get_y() != std::get<3>(borders))
        // Compute area of given point in its area created from calling area functions 
        {
            std::cout << "creating grid\n";
            auto future = std::async(getAreaAsyncCaller,x,points); // create thread for each point
            auto area = future.get(); // get value returned by thread
            areas.push_back(area); 
        }
    }

    std::cout << *std::max_element(areas.begin(),areas.end()) + 1 << "\n";
}