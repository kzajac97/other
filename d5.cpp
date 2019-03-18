#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cctype>
#include <future>

#include <boost/algorithm/string.hpp>

bool isStringCleared(std::vector<char> str)
{
    for(auto it = str.begin(); it != str.end(); ++it)
    {
        /* if element is upper case and next element is lower */
        if(std::isupper(*it) && std::islower(*(it+1)))
        { if(*it == std::toupper(*(it+1))) { return false; } }
        /* if element is lower case and next element is upper */
        else if(std::islower(*it) && std::isupper(*(it+1)))
        { if(*it == std::tolower(*(it+1))) { return false; } }
    }

    return true;
}


/*  Removes all occurances of character in string upper and lower case
    Requires upper_case input character  */
std::string erase_all_cases(std::string str, char c)
{
    std::string upper(1,c); // convert to one element string
    std::string lower(1,std::tolower(c));
    boost::erase_all(str,upper); // function requires string
    boost::erase_all(str,lower);

    return str;
}

/*
*/
std::string getProducedString(std::vector<char> stringVec)
{
    /* loop until no further shrinking can be achieved */
    while(!isStringCleared(stringVec))
    {
        for(auto it = stringVec.begin(); it != stringVec.end(); )
        {
            /* if element is upper case and next element is lower */
            if(std::isupper(*it) && std::islower(*(it+1)))
            {
                if(*it == std::toupper(*(it+1)))
                {
                    it = stringVec.erase(it); // remove matching element
                    it = stringVec.erase(it); // remove next element   
                }
                
                else { ++it; } // increment only if nothing was erased
            }
            /* if element is lower case and next element is upper */
            else if(std::islower(*it) && std::isupper(*(it+1)))
            {    
                if(*it == std::tolower(*(it+1)))
                {
                    it = stringVec.erase(it); // remove matching element
                    it = stringVec.erase(it); // remove next element   
                }

                else { ++it; } // increment only if nothing was erased
            }
            else { ++it; } // increment when two lower case or upper case letters found
        }
    }
    
    std::string output(stringVec.begin(),stringVec.end()); //create string from initializer list
    return output;
}

/*
*/
unsigned int getProducedLength(std::vector<char> stringVec)
{
    /* loop until no further shrinking can be achieved */
    while(!isStringCleared(stringVec))
    {
        for(auto it = stringVec.begin(); it != stringVec.end(); )
        {
            /* if element is upper case and next element is lower */
            if(std::isupper(*it) && std::islower(*(it+1)))
            {
                if(*it == std::toupper(*(it+1)))
                {
                    it = stringVec.erase(it); // remove matching element
                    it = stringVec.erase(it); // remove next element   
                }
                
                else { ++it; } // increment only if nothing was erased
            }
            /* if element is lower case and next element is upper */
            else if(std::islower(*it) && std::isupper(*(it+1)))
            {    
                if(*it == std::tolower(*(it+1)))
                {
                    it = stringVec.erase(it); // remove matching element
                    it = stringVec.erase(it); // remove next element   
                }

                else { ++it; } // increment only if nothing was erased
            }
            else { ++it; } // increment when two lower case or upper case letters found
        }
    }
    return stringVec.size(); // returns length of shrinked sequence 
}

int main(void)
{
    std::string input = "";
    std::string alphabet = "ABCDEFGHIJKLMNOPRSTUWXYZ";
    std::vector<char> stringVec;
    std::vector<unsigned int> outputVec;

    std::copy(input.begin(),input.end(),std::back_inserter(stringVec));
    auto shrinkedInput = getProducedString(stringVec); // Get shrinked input from part one to reduce complexity 
    stringVec.clear(); 
    
    /* iterate through alphabet */
    for(const auto it : alphabet)
    {
        auto str = erase_all_cases(shrinkedInput,it); 
        std::copy(str.begin(),str.end(),std::back_inserter(stringVec)); // copy into vector
        auto future = std::async(getProducedLength,stringVec); // create thread for each letter
        auto size = future.get(); // get value returned by thread 
        outputVec.push_back(size); // push sizes to vector
        stringVec.clear(); // clear input vector
    }

    /* display smallest value on vector dereferencing iterator returned by min_element */
    std::cout << *std::min_element(outputVec.begin(),outputVec.end()) << "\n";
}