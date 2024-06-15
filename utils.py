import csv
from itertools import chain, combinations

def ReadCSV(inputFile):
    C1 = []
    inputTextToList = []
    numberOfLines = 0

    with open(inputFile, "r") as f:
        csvFile = csv.reader(f)
        for line in csvFile:
            inputTextToList.append(line)
            for word in line:
                tempWordList = []
                tempWordList.append(word)
                if tempWordList not in C1:
                    C1.append(tempWordList)
            numberOfLines += 1 
    return inputTextToList, numberOfLines, C1

def PruneInfrequentItemsets(CK, minimumSupport, inputTextToList):
    LK = []
    for itemSet in CK:
        if Support(itemSet, inputTextToList) >= minimumSupport:
            LK.append(itemSet)
    return LK

def Support(itemSet, inputTextToList):
    numberOfOccurences = 0
    for line in inputTextToList:
        if all(e in line for e in itemSet):
            numberOfOccurences += 1
    return numberOfOccurences


def GenerateCandidateSet(itemset, K):
    CK = []
    requiredNumberOfMatchingElements = (K - 2)
    for x in range(len(itemset)):
        for y in range(x + 1, len(itemset)):
            #if the first K - 2 elements are the same then make combination
            if MatchingFirstKElements(itemset[x],itemset[y], requiredNumberOfMatchingElements):
                #make a new list from joining x and y
                newList = itemset[x] + itemset[y]
                #remove duplicates
                newList = list(dict.fromkeys(newList))
                #append to CK
                CK.append(newList)
    return CK

def MatchingFirstKElements(list1, list2, K):
    for i in range(K):
        if list1[i] != list2[i]:
            return False
    return True

def FormattedPrint(frequentItemSets, associationRules):
    print("=====Frequent Sets=====")
    for set in frequentItemSets:
        print(set)
    print("=======================")
    print("=======Confidence======")
    for rule in associationRules:
        #print(rule)
        print(f"{rule[0]} --> {rule[1]} : {rule[2]:.2%}")
    print("=======================")

def AssociationRules(frequentItemSets, inputTextToList, minConfidence):
    associationRules = []
    for item in frequentItemSets:
        powerset = list(chain.from_iterable(combinations(item, r) for r in range(1,len(item))))
        for set in powerset:
            confidence = (Support(item, inputTextToList) / Support(list(set), inputTextToList))
            if confidence >= minConfidence:
                associationRules.append([list(set),[x for x in item if x not in list(set)], confidence])
    return associationRules