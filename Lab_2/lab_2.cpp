#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

int alpabetLength;
int statesLength;
int startState;
std::list<int> endStates;
std::vector<std::vector<char>> transitions;

bool read_instructions()
{
    std::ifstream file("instructions.txt");
    std::istreambuf_iterator<char> iter(file);
    std::istreambuf_iterator<char> endIter;

    int lineCounter = 1;

    while (lineCounter < 5 and iter != endIter)
    {
        // std::cout << *iter;
        if (*iter == '\n')
        {
            ++lineCounter;
        }
        else if (lineCounter == 1)
        {
            alpabetLength = *iter - '0';
            std::cout << "Alphabet length: " << alpabetLength << std::endl;
        }
        else if (lineCounter == 2)
        {
            statesLength = *iter - '0';
            std::cout << "States length: " << statesLength << std::endl;
        }
        else if (lineCounter == 3)
        {
            startState = *iter - '0';
            std::cout << "Start state: " << startState << std::endl;
        }
        else if (lineCounter == 4 and *iter != ' ')
        {
            endStates.push_back(*iter - '0');
        }
        ++iter;
    }

    std::cout << "End states: ";
    for (int state : endStates)
    {
        std::cout << state << " ";
    }
    std::cout << std::endl;

    std::vector<char> oneTransition;
    while (iter != endIter)
    {
        if (*iter == '\n')
        {
            transitions.push_back(oneTransition);
            oneTransition.clear();
        }
        else if (*iter != ' ')
        {
            oneTransition.push_back(*iter);
        }
        ++iter;
    }

    std::cout << "Transitions: " << std::endl;
    for (std::vector<char> transition : transitions)
    {
        std::cout << transition[0] << " " << transition[1] << " " << transition[2] << " " << std::endl;
    }

    return true;
}

int main()
{
    read_instructions();

    std::cout << "Enter a part of a word: ";
    std::string inputWord;
    std::cin >> inputWord;

    return 0;
}