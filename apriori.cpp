// Seth Winkhart
// 10/13/2022
// implementation of Apriori algorithm, takes two commandline inputs,
// the input filename, and a minimum support level

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>

// global variable containing file info and structure
double numLines = 0.0;
std::vector<std::string> DB;

///////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
///////////////////////////////////////////////////////////////////////////////////////////////

void print(const std::map<std::vector<int>, int> &);
void testPrint(const std::map<std::vector<int>, int> &);
void readFile(std::string, std::map<std::vector<int>, int> &);
std::map<std::vector<int>, int> frequentPrune(std::map<std::vector<int>, int>, double);
std::map<std::vector<int>, int> aprioriAlg(const std::map<std::vector<int>, int> &, double);
std::map<std::vector<int>, int> generateNextSet(const std::map<std::vector<int>, int> &, const std::map<std::vector<int>, int> &);

///////////////////////////////////////////////////////////////////////////////////////////////
// DRIVER
///////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    // error checking
    if (argc < 3)
    {
        std::cout << "Error: too few arguments, three are required" << std::endl;
        return 0;
    }
    else if (argc > 3)
    {
        std::cout << "Error: too many arguments, three are required" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "=======================" << std::endl;
        std::cout << "Running apriori on: " << argv[1] << std::endl
                  << "Minimum support: " << argv[2] << "%" << std::endl;
    }

    // turning supplied minsupport into double
    int length = 0;
    double minSupport = 0;
    for (int i = 0; argv[2][i] != '\0'; ++i)
    {
        ++length;
    }
    for (int i = 0; argv[2][i] != '\0'; ++i)
    {
        minSupport += ((argv[2][i] - '0') * pow(10, length - i - 1));
    }

    // read file, generate c1, and master list of database
    std::map<std::vector<int>, int> c1;
    readFile(argv[1], c1);

    std::map<std::vector<int>, int> result = aprioriAlg(c1, minSupport);

    print(result);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////

// fancy print
void print(const std::map<std::vector<int>, int> &entries)
{
    std::cout << "=====frequent sets=====" << std::endl;
    int i = 0;
    for (const auto &pr : entries)
    {
        for (const auto &v : pr.first)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "=======================" << std::endl;
}

// debug print
void testPrint(const std::map<std::vector<int>, int> &entries)
{
    for (auto pr : entries)
    {
        std::cout << "Number: ";
        for (auto v : pr.first)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
        std::cout << "Occurences: " << pr.second << std::endl
                  << std::endl;
    }
}

// read file and create c1
void readFile(std::string fileName, std::map<std::vector<int>, int> &entries)
{
    std::ifstream in(fileName);
    std::string line;

    while (std::getline(in, line))
    {
        // updates global variable
        DB.push_back(line);
        for (auto word : line)
        {
            if (word != ' ')
            {
                int num = word - '0';

                // prime the map
                if (entries.empty())
                {
                    std::vector<int> temp(1, num);
                    entries.insert(std::make_pair(temp, 1));
                    continue;
                }

                // track curtent position in the map
                int count = 1;
                for (auto &pr : entries)
                {
                    // if you found the number increment and break since no repeats, always [0] since inital item set is each individual item, not pairs
                    if (pr.first[0] == num)
                    {
                        ++pr.second;
                        count = 1;
                        break;
                    }

                    // if you reach the end of the map, add a new one and break so not to go over newly added one
                    if (count == entries.size())
                    {
                        std::vector<int> temp(1, num);
                        entries.insert(std::make_pair(temp, 1));
                        count = 1;
                        break;
                    }
                    ++count;
                }
            }
        }
        // updates global variable
        ++numLines;
    }
    in.close();
}

// prune data set based on minimum support
std::map<std::vector<int>, int> frequentPrune(std::map<std::vector<int>, int> ck, double minSupport)
{
    // loop through map
    for (auto it = ck.begin(); it != ck.end();)
    {
        // make frequency relative to the DB and make it a double
        double frequency = ((it->second / numLines) * 100.0);

        // if its less then erase that pair and increment
        if (frequency < minSupport)
        {
            // this is there pre vs post incrementation matters
            ck.erase(it++);
        }
        // just increment
        else
        {
            ++it;
        }
    }
    return ck;
}

// generate new supersets and find their frequency in the DB vector
std::map<std::vector<int>, int> generateNextSet(const std::map<std::vector<int>, int> &lk, const std::map<std::vector<int>, int> &l1)
{
    std::map<std::vector<int>, int> ck;

    // generate next supersets
    for (const auto &itemSet : lk)
    {
        for (auto item : l1)
        {
            if (std::find(itemSet.first.begin(), itemSet.first.end(), item.first[0]) == itemSet.first.end())
            {
                std::vector<int> priviousItemClone(itemSet.first);

                for (auto it = priviousItemClone.begin(); it != priviousItemClone.end(); ++it)
                {
                    if (it == priviousItemClone.end() - 1 || item.first[0] < *it)
                    {
                        if (it == priviousItemClone.end() - 1 && !(item.first[0] < *it))
                        {
                            priviousItemClone.push_back(item.first[0]);
                        }
                        else
                        {
                            priviousItemClone.insert(it, item.first[0]);
                        }

                        if (ck.find(priviousItemClone) == ck.end())
                        {
                            ck.insert(std::make_pair(priviousItemClone, 0));
                        }
                        break;
                    }
                }
            }
        }
    }

    // find frequency of new supersets in DB
    for (auto &pr : ck)
    {
        for (const auto &line : DB)
        {
            for (int i = 0; i < pr.first.size(); ++i)
            {
                if (line.find(pr.first[i] + '0') == std::string::npos)
                {
                    break;
                }
                if (i + 1 == pr.first.size())
                {
                    ++pr.second;
                }
            }
        }
    }

    return ck;
}

// actual algorithm
std::map<std::vector<int>, int> aprioriAlg(const std::map<std::vector<int>, int> &c1, double minSupport)
{
    std::map<std::vector<int>, int> result;
    std::map<std::vector<int>, int> ck;
    std::map<std::vector<int>, int> lk;
    std::map<std::vector<int>, int> l1;

    // prune c1 to make l1, prime the algorithm
    l1 = frequentPrune(c1, minSupport);

    lk = l1;
    result = l1;

    // generate next itemset and their frequency based off l1 and master DB vector

    while (ck.size() != 0 || lk.size() != 0)
    {
        // generate next itemset and their frequency based off the previous itemset, the first canidate itemset and compare to master DB vector to find frequency
        ck = generateNextSet(lk, l1);

        // prune
        lk = frequentPrune(ck, minSupport);

        // adding frequent items to the result
        for (const auto &pr : lk)
        {
            // this is not efficient, map tree is constantly rebalencing itself
            result.insert(pr);
        }
    }

    return result;
}