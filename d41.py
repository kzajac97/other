import re

class Guard:
    # constructor
    def __init__(self,Id):
        self.Id = Id
        self.minutes = [0]*60 # list of 60 zeros

    def updateMinutes(self,start,end):
        for i in range(len(self.minutes)):
            if i >= start and i < end:
                self.minutes[i] += 1 

    def getSumOfSleptMinutes(self):
        return sum(self.minutes)

    def findMostOftenSleptMinute(self):
        return self.minutes.index(max(self.minutes))

def getGuardId(string):
    guardID = re.search('#(.*)b',string)
    return int(guardID.group(1)) if guardID is not None else None

def getMinutes(string):
    minutes = re.search(":(.*)]",string)
    return int(minutes.group(1))  

def getGuardById(lst,Id):
    for x in lst:
        if Id is x.Id:
            return x

    return None

def IdFound(string):
    Id = re.search('#(.*)b',string)
    return True if Id is not None else False

if __name__ == "__main__":
    inFile = open("datad4.txt","r")
    sTimes = inFile.read()
    if type(sTimes) is type(str()):
        lVals = [str(x) for x in sTimes.split('\n')]
    
    lVals.sort()
    
    outFile = open("data4sorted.txt","w")
    for x in lVals:
        outFile.write(x+'\n')
    
    # sort list of string by dates
    # sorts list alphabetically 
    # but dates are stored as strings 
    # so it's chronological as well
    lVals.sort()  
    lGuards = []
    i = 0
    Id = 0
    guard = 0
    while i < len(lVals):
        if IdFound(lVals[i]) is True:
            Id = getGuardId(lVals[i])
            if getGuardById(lGuards,Id) is None:
                lGuards.append(Guard(Id))
                i += 1 # move by one element
                guard = getGuardById(lGuards,Id) # acess last element
            else:
                guard = getGuardById(lGuards,Id) # find guard on list by ID
                i += 1 # move by one element
        else:
            guard.updateMinutes(getMinutes(lVals[i]),getMinutes(lVals[i+1])) #update minutes
            i += 2 # move iterator by 2 elements

    G = Guard(-1)

    for x in lGuards:
        # if x.getSumOfSleptMinutes() > G.getSumOfSleptMinutes():
        #     G = x
        if x.Id == 521:
            print(x.minutes)

    print(G.Id * G.findMostOftenSleptMinute())