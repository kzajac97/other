#!bin/python

def elementInList(element,lst):
    return True if element in set(lst) else False

file = open("data.txt","r")
sFrequencies = file.read()

if type(sFrequencies) is type(str()):
    lVals = [int(x) for x in sFrequencies.split('\n')]

lResults = []
frequency = 0 # if is x at the end no freq found
currentSum = 0
elementFound = False

while(elementFound is False):
    for i in range(len(lVals)):
        currentSum += lVals[i]
        lResults.append(currentSum - lVals[i])
        if elementInList(currentSum,lResults) is True:
            print(frequency)
            elementFound = True
            break