import sys
import utils

if len(sys.argv) == 2:
    try:
        inputTextToList, numberOfLines, C1 = utils.ReadCSV(sys.argv[1])
    except FileNotFoundError:
        print("\033[91m {}\033[00m" .format("Only .csv is supported"))
        exit()

    #get user input
    try:
        minSupport = input("Enter Minimum Support Percerage: ")
        minSupport = numberOfLines * (float(minSupport) / 100)
    except ValueError:
        print("\033[91m {}\033[00m" .format("Enter a number 0-100"))
        exit()
    try:
        minConfidence = input("Enter Minimum Confidence Percerage: ")
        minConfidence = float(minConfidence) / 100
    except ValueError:
        print("\033[91m {}\033[00m" .format("Enter a number 0-100"))
        exit()

    #create L1 to prime the primary loop
    L1 = utils.PruneInfrequentItemsets(C1, minSupport, inputTextToList)

    LK = L1
    frequentItemSets = []
    K = 2
    
    while LK:
        frequentItemSets.extend(LK)
        #generate possile combinations (candidate set)
        CK = utils.GenerateCandidateSet(LK, K)
        LK.clear()
        #prune the infrequent items save to LK
        LK = utils.PruneInfrequentItemsets(CK, minSupport, inputTextToList)
        K += 1
    
    #final output
    utils.FormattedPrint(frequentItemSets, utils.AssociationRules(frequentItemSets, inputTextToList, minConfidence))
else:
    print("\033[91m {}\033[00m" .format("Error incorrect number of arguments, two arguments are required"))