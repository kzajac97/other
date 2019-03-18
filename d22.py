import collections
from threading import Thread

def removeFromList(x,lst):
    result = lst
    result.remove(x)
    return result

def getStringCounter(string):
    dWordCount = dict(collections.Counter(string))
    count2 = False
    cout3 = False
    for x in dWordCount.values():
        if x is 2:
            count2 = True
        if x is 3:
            cout3 = True

    return (count2,cout3)

def compareStrings(string1,string2):
    diffCounter = 0
    for i in range(len(string1)):
        if string1[i] is not string2[i]:
            diffCounter += 1
        if diffCounter > 1:
            return False
        
    return True if diffCounter is 1 else False

def compareOnList(string,lst):
    for x in lst:
        if compareStrings(string,x) is True:
            print(string)
    
    return False

if __name__ == "__main__":
    file = open("datad2.txt","r")
    sLetters = file.read()

    if type(sLetters) is type(str()):
        lVals = [str(x) for x in sLetters.split('\n')]    

    threads = [] # empty thread list
    for x in lVals:
        thread = Thread(target = compareOnList, args=(x,lVals))
        thread.start()
        threads.append(thread)    
    
    # join all threads
    for x in threads:
        x.join()