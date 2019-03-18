#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "point.hh"
#include "area.hh"

using type_t = int; // alias for int created for easy data type switching 

// Global object representing space
Area<type_t> gridArea;

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

// Creates 2D points from vector of ints 
// Input const vector of numeric type variables with even number of elements
// Returns vector of 2D points represented by Point object
std::vector<Point<type_t> > createPointsFromVector(const std::vector<type_t> & inputData)
{
    std::vector<Point<type_t> > output;
    unsigned int id_iterator = 1; // additional variable for creating points ids

    // iterates moving by two elements 
    for(unsigned int i=0; i < inputData.size(); i += 2)
    {
        // Point created from two int's
        Point<type_t> point({inputData[i],inputData[i+1]});
        // set point id and increment iterator
        point.set_id(id_iterator); 
        ++id_iterator; 

        output.push_back(point);
    }

    return output;
}

// Finds four border points of given point set
// output represents smallest square enclosing all points in given set 
// Input vector of 2D points 
// Output Tuple of four border points 
// border points are smallest x coordinate smallest y coordinate
// highest x coordinate and highest y coordinate
std::tuple<Point<type_t>,Point<type_t>,Point<type_t>,Point<type_t> > getBorderPoints( std::vector<Point<type_t> > points)
{

    Point<type_t> highestX({points[0].get_x(),0}); //set initial values
    Point<type_t> lowestX({points[0].get_x(),0}); 
    Point<type_t> highestY({0,points[0].get_y()});
    Point<type_t> lowestY({0,points[0].get_y()});
    
    // iterate through points vector and save 
    // lowest and hightest X and Y values 
    for(auto p : points)
    {
        if(p.get_x() > highestX.get_x()) // save both point coordinates  
        { highestX.set_x(p.get_x()); highestX.set_y(p.get_y()); }
        else if(p.get_x() < lowestX.get_x()) 
        { lowestX.set_x(p.get_x()); lowestX.set_y(p.get_y()); }
        if(p.get_y() > highestY.get_y()) 
        { highestY.set_y(p.get_y()); highestY.set_x(p.get_x()); }
        else if(p.get_y() < lowestY.get_y()) 
        { lowestY.set_y(p.get_y()); lowestY.set_x(p.get_x());}
    }

    return {lowestX,highestX,lowestY,highestY}; // return tuple order is important
}

// gets closest point from global points vector to any grid location
// Input points vector representing set of points to search in 
// xIndex and yIndex are 2D coordinates of point for which function searches 
// for smallest point in given point set
unsigned int getClosestPoint(const std::vector<Point<type_t> > & points, type_t xIndex, type_t yIndex)
{
    unsigned int closestPointId = 0;
    type_t smallestDistance = 500;
    Point<type_t> P({xIndex,yIndex}); // create base point for function

    for(auto it : points)
    {
        // distance between point on a grid and point from vector
        auto currentDistance = it.getDistance(P);
        // compare to currently smallest found distance and 
        // save point's  id if new distance is smaller  
        if(currentDistance < smallestDistance) 
        { 
            smallestDistance = currentDistance; 
            closestPointId = it.get_id(); 
        }
        // if two points are equally close set id to 0
        else if(currentDistance == smallestDistance) { closestPointId = 0; }
    }

    return closestPointId;
}

// calculates overall distance to given grid point
// similar to getClosestPoint
// Returns sum of distances to all points in point set
unsigned int getPointDistance(const std::vector<Point<type_t> > & points, type_t xIndex, type_t yIndex)
{
    type_t distance = 0;
    Point<type_t> P({xIndex,yIndex});
    // sum up distances to all grid points
    for(auto it : points) { distance += it.getDistance(P); }

    return distance;
} 

// function to fill one column of vector of vectors
// utility for multithreading
void set_split_distances(const std::vector<Point<type_t> > & points,const unsigned int size,const unsigned int index)
{
    for(unsigned int y=0; y < size; ++y)
    {
        auto point = getClosestPoint(points,index,y);
        gridArea.set_distance(point,index,y);
    }
}

// splits 2D vector of vectors into column vectors to allow multithreading
// size is column vector length and index is the column number (x coordinate)
void get_split_distances(const std::vector<Point<type_t> > & points, const unsigned int size, const unsigned int index)
{
    // loop through single column
    for(unsigned int y=0; y < size; ++y)
    {
        auto point = getPointDistance(points,index,y); // calculate sum of distances to all points
        gridArea.set_distance(point,index,y); // set distance in area object
    }
}

// set distances in global gridArea object
// Input points vector representing points set to calculate distances to
void set_distances(const std::vector<Point<type_t> > & points)
{
    std::vector<std::thread> threads;
    // get sizes of x and y vector
    auto x_size = gridArea.get_x_size(); 
    auto y_size = gridArea.get_y_size();
    // modify vector sizes to acess elements using indexes
    gridArea.set_x_size(x_size);
    gridArea.set_y_sizes(y_size);
    // loop through vector of vectors and use threads to 
    // fill vectors inside it 
    for(unsigned int x=0; x < x_size; ++x)
    { 
        std::thread t(set_split_distances,points,y_size,x);
        threads.push_back(std::move(t)); // can't copy thread objects
    }

    for(unsigned int x=0; x < x_size; ++x)
    { threads[x].join(); }

    threads.clear();
}

// set distances in global gridArea object
void get_distances(const std::vector<Point<type_t> > & points)
{
    std::vector<std::thread> threads;
    // get sizes of x and y vector
    auto x_size = gridArea.get_x_size(); 
    auto y_size = gridArea.get_y_size();
    // modify vector sizes to acess elements using indexes
    gridArea.set_x_size(x_size);
    gridArea.set_y_sizes(y_size);
    // loop through vector of vectors and use threads to 
    // fill vectors inside it 
    for(unsigned int x=0; x < x_size; ++x)
    { 
        std::thread t(get_split_distances,points,y_size,x);
        threads.push_back(std::move(t)); // can't copy thread objects
    }

    for(unsigned int x=0; x < x_size; ++x)
    { threads[x].join(); }

    threads.clear();
}

int main(void)
{
    const auto inputData = readFile("datad6.txt"); // read input data to string
    const auto points = createPointsFromVector(inputData); // vector of points
    const auto borders = getBorderPoints(points); // tuple of type_t
    // main for part 2
    gridArea.set_borders(borders);
    get_distances(points);
    // 10000 is given constant 
    auto sum = gridArea.getPointsWithCorrectDistance(10000);
    std::cout << sum << "\n";
}