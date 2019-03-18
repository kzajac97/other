#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include <typeinfo>
#include <list>
#include <memory>
#include <numeric>
#include <limits>
// Use -I and -L to compile giving headers and binaries paths
#include <boost/algorithm/string/predicate.hpp>

namespace bst = boost::algorithm;

/* String to compare which input line holds shift start, asleep or awake time */
const std::string shfitStartComparator = "begins shift";
const std::string startTimeComparator = "falls asleep";
const std::string endTimeComparator = "wakes up";

class Guard
{
    /* Private vector holding asleep and awake minutes */
    std::vector<int> minutes;
    public:
    int Id; 
    /* Constructor initializing class fields */
    Guard(int Id) : minutes(60,0), Id(Id) {}; // 60 awake minutes
    Guard() = default;
    ~Guard() {};

    /* custom comparators using ID to compare object to int */
    bool operator == (const int id) { return this->Id == id; }
    bool operator != (const int id) { return this->Id != id; }

    /* Retruns vector with info about how many times guard slept each minute */
    void updateMinutes(unsigned int startTime, unsigned int endTime);
    const void printMinutes(void) { for(auto x : this->minutes) { std::cout << x << " "; } }
    const int getSumOfMinutes(void) { return std::accumulate(this->minutes.begin(),this->minutes.end(),0); }
    const unsigned int getHighestMinute(void) { return std::distance(this->minutes.begin(),std::max_element(this->minutes.begin(),this->minutes.end())); }
};

void Guard::updateMinutes(unsigned int startTime, unsigned int endTime)
{
    for(unsigned int i=0; i < this->minutes.size(); ++i)
    {
        if(i >= startTime && i < endTime)
        { this->minutes[i]++; } // increment minutes between falling asleep and waking up times 
    }
}

/*  Read contents of file with filename passed as string
    Returns vector of strings where each element is new line of input file */
std::vector<std::string> readFile(std::string fileName)
{
    std::string str;
    std::vector<std::string> outString;
	std::ifstream inputFile(fileName.c_str()); //open file copying string 
 
	if(!inputFile) // File not found
	{ std::perror("Error! Can't open file!\n"); }
    
	// read the next line from file 
	while (std::getline(inputFile, str))
	{
		if(str.size() > 0) { outString.push_back(str); }
	}

	inputFile.close(); // close file
	return outString; // return vector of strings
}

int getGuardID(std::string inputString)
{
    const std::regex re("#(.*)b"); // gets string between # and b characters
    std::smatch match;
    std::string outputString;
    /* while serach returns true */
    while(std::regex_search(inputString,match,re,std::regex_constants::match_any))
    {
        outputString = match[1]; // string without # character
        inputString = match.suffix().str();
    }
    
    return std::stoi(outputString); // convert to number and return
} 

int getMinutes(std::string inputString)
{
    const std::regex re(":(.*)]"); // only last time characters are important always between : and ]
    std::smatch match;
    std::string outputString;
    /* while serach returns true */
    while(std::regex_search(inputString,match,re,std::regex_constants::match_any))
    {
        outputString = match[1];
        inputString = match.suffix().str();
    }
    
    return std::stoi(outputString); // convert to number and return
}

int main(void)
{
    auto valuesVec = readFile("datad4.txt"); // vector of strings
    // sorting vector results in alphabetical 
    // order which also is chronological
    std::sort(valuesVec.begin(),valuesVec.end());
    std::list<Guard> guardsVec; // list of guard objects
    /* Times and ID variables */
    int guardID;
    int largest = 0;
    int startTime = -1; 
    int endTime = -1;
    int output;
    Guard* guard_ptr;

    /* Loop creating guards objects */
    for(const auto & it : valuesVec)
    { 
        if(bst::contains(it,shfitStartComparator))
        {
            auto Id = getGuardID(it); // should be int
            /* seraches for object with ID equal to ID from string */
            auto found = std::find_if(guardsVec.begin(),guardsVec.end(),[Id](const Guard & guard){
                return guard.Id == Id; // custom operator
            });
            /* Check if this ID exists if not create new object and push to list */
            if(*found != Id) { guardsVec.push_back(Guard(Id)); }
        }
    }

    /*  */
    for(const auto & it : valuesVec)
    {
        if(bst::contains(it,shfitStartComparator)) { guardID = getGuardID(it); }
        else if(bst::contains(it,startTimeComparator)) { startTime = getMinutes(it); }
        else if(bst::contains(it,endTimeComparator)) { endTime = getMinutes(it); }

        /* seraches for object with ID equal to ID from string */
        auto found = std::find_if(guardsVec.begin(),guardsVec.end(),[guardID](const Guard & guard)
        {
            return guard.Id == guardID; // custom operator 
        });
        /* Check if this ID exists if not create new object and push to list */
        if(*found == guardID) { guard_ptr = &*found; }
        /* If start and end times read correctly update minutes */
        if(startTime > 0 && endTime > 0)
        {
            auto sTime = unsigned(startTime); //explicit convertions
            auto eTime = unsigned(endTime);
            guard_ptr->updateMinutes(sTime,eTime); // iterating through vector requires unsigned ints as indexes
            startTime = -1; //set to negative value to avoid updating the same time twice
            endTime = -1;
        }
    }

    for(auto x : guardsVec)
    {
        auto sum = x.getSumOfMinutes();
        if(sum > largest)
        {
            largest = sum;
            output = x.getHighestMinute() * x.Id;
        }
    }

    std::cout << output << "\n";
}