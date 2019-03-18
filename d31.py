import re

def getShift(string):
    shiftv = re.search('@(.*),', string) # all characters between @ and ,
    shifth = re.search(',(.*):',string) # all characters betweem , and :
    return (int(shiftv.group(1)),int(shifth.group(1))) # return tuple with string converten to ints

def getSize(string):
    sizev = re.search(':(.*)x', string)
    sizeh = re.search('x(.*)',string) # from x to string end
    return (int(sizev.group(1)),int(sizeh.group(1)))

def countElements(lst):
    counter = 0              
    for i,j in [(i,j) for i in range(len(lst)) for j in range(len(lst[i]))]:
        if lst[i][j] > 1:  
            counter += 1 # if more than two items found in this location

    return counter

if __name__ == "__main__":
    file = open("datad3.txt","r")
    sLetters = file.read()

    if type(sLetters) is type(str()):
        lVals = [str(x) for x in sLetters.split('\n')]    

    llFabric = [] # list of lists

    for _ in range(0,1000):
       llFabric.append([0 for _ in range(0,1000)])

    for x in lVals:
        shift = getShift(x)
        size = getSize(x)
        for i,j in [(i,j) for i in range(size[0]) for j in range(size[1])]:
            llFabric[i+shift[0]][j+shift[1]] += 1 

    print(countElements(llFabric))