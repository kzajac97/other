import collections

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

if __name__ == "__main__":
    
    file = open("datad2.txt","r")
    sLetters = file.read()

    if type(sLetters) is type(str()):
        lVals = [str(x) for x in sLetters.split('\n')]    

    counter2 = 0
    counter3 = 0
    
    for x in lVals:
        counter = getStringCounter(x)
        if counter[0] is True:
            counter2 += 1
        if counter[1] is True:
            counter3 += 1
    
    checksum = counter2 * counter3
    print(checksum)