#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>

int SUGGESTIONS_COUNT = 10;

int alpabetLength;
int statesLength;
int startState;
std::list<int> endStates;
std::vector<std::vector<char>> transitions;
int currentState;
std::list<std::string> suggestions;

std::list<std::vector<int>> transitions_history;

bool read_instructions(std::string instructions)
{
    std::ifstream file(instructions);
    std::istreambuf_iterator<char> iter(file);
    std::istreambuf_iterator<char> endIter;

    int lineCounter = 1;

    while (lineCounter < 5 and iter != endIter)
    {
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

bool validate_input_word(std::string inputWord)
{
    currentState = startState;
    for (char letter : inputWord)
    {
        bool should_continue = false;
        for (std::vector<char> transition : transitions)
        {
            if (transition[0] - '0' == currentState and transition[1] == letter)
            {
                currentState = transition[2] - '0';
                should_continue = true;
                break;
            }
        }
        if (!should_continue)
        {
            std::cout << "The word is not in the language!" << std::endl;
            return false;
        }
    }
    return true;
}

bool isEndState(int state)
{
    for (int endState : endStates)
    {
        if (state == endState)
        {
            return true;
        }
    }
    return false;
}

bool is_unique(std::string newWord)
{
    for (std::string word : suggestions)
    {
        if (newWord == word)
        {
            return 0;
        }
    };
    return 1;
};

void autocomlete_word(std::string inputWord)
{
    std::string word = inputWord;
    int currentWordState = currentState;
    bool should_continue = true;
    std::vector<int> current_transitions;
    std::map<int, std::set<int>> tried_transitions;
    while (should_continue)
    {
        std::cout << "Current word_state: " << currentWordState << std::endl;
        if (word.length() > 10)
        {
            break;
        }

        bool transition_found = false;
        for (int i = 0; i < transitions.size(); i++)
        {
            if (transitions[i][0] - '0' == currentWordState) // tried_transitions[currentWordState].find(i) == tried_transitions[currentWordState].end()
            {
                word += transitions[i][1];
                tried_transitions[currentWordState].insert(i); // position of function, count from 0
                currentWordState = transitions[i][2] - '0';
                should_continue = true;
                transition_found = true;
                current_transitions.push_back(i);
            }
            if (isEndState(currentWordState) && is_unique(word))
            {
                suggestions.push_back(word);
                transition_found = true;
                should_continue = false;
                std::cout << "Tried Transitions: " << std::endl;
                for (const auto &entry : tried_transitions)
                {
                    std::cout << "State " << entry.first << ": ";
                    for (int transition : entry.second)
                    {
                        std::cout << transition << " ";
                    }
                    std::cout << std::endl;
                }
                break;
            }
        }
        // when in dead end or was (need multiple steps back)(add counter) and it is already in suggestions return via transitions_history 1 step and dont use last(current) transition
        if (!transition_found || !is_unique(word))
        {
            if (!current_transitions.empty())
            {
                current_transitions.pop_back(); // remove last transition
                if (!current_transitions.empty())
                {
                    // return to the state of the last transition
                    int last_transition = current_transitions.back();
                    currentWordState = transitions[last_transition][0] - '0';
                    // remove the last character from the word
                    word.pop_back();
                }
                else
                {
                    // no more transitions to backtrack, break the loop
                    break;
                }
            }
            else
            {
                // no more transitions to backtrack, break the loop
                break;
            }
        }
    }
}

int main()
{
    read_instructions("instructions.txt");

    std::string inputWord;
    std::cout << "Enter a part of a word: ";
    std::cin >> inputWord;

    if (!validate_input_word(inputWord))
    {
        return 0;
    }

    for (int i = 0; i < SUGGESTIONS_COUNT; i++)
    {
        autocomlete_word(inputWord);
    }

    std::cout << "Suggestions: " << std::endl;
    for (std::string suggestion : suggestions)
    {
        std::cout << suggestion << std::endl;
    }

    return 0;
}