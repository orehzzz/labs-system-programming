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
            std::cout << "This word is not in the language!" << std::endl;
            return false;
        }
    }
    if (isEndState(currentState))
    {
        std::cout << "This word is in the language!" << std::endl;
    }
    return true;
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

bool is_forbidden_transition(int transition, std::list<int> forbidden_transitions)
{
    for (int forbidden_transition : forbidden_transitions)
    {
        if (transition == forbidden_transition)
        {
            return true;
        }
    }
    return false;
}

void autocomlete_word(std::string inputWord)
{
    std::string word;
    int currentWordState = currentState;
    bool should_continue = true;
    std::vector<int> current_states;
    std::map<int, std::set<int>> tried_transitions;
    std::map<int, std::list<int>> forbidden_transitions;
    int last_transition = -1;

    current_states.push_back(currentWordState);

    while (should_continue)
    {
        bool transition_found;

        for (int i = 0; i < transitions.size(); i++) // check all transitions
        {
            transition_found = false;

            if ((transitions[i][0] - '0' == currentWordState) && (!is_forbidden_transition(i, forbidden_transitions[word.length()])))
            {
                word += transitions[i][1];
                tried_transitions[currentWordState].insert(i); // position of function, count from 0
                currentWordState = transitions[i][2] - '0';
                should_continue = true;
                transition_found = true;
                current_states.push_back(i);
                last_transition = i;

                if (isEndState(currentWordState))
                {
                    if (is_unique(inputWord + word))
                    {
                        forbidden_transitions.clear();
                        suggestions.push_back(inputWord + word);

                        current_states.clear();

                        transition_found = true; // to not go back
                        should_continue = false;

                        break;
                    }
                }
            }
        }

        if (!transition_found)
        {
            if (word.length() == 0) // in initial state, all transitions tried
            {
                break;
            }
            word.pop_back();
            forbidden_transitions[word.length()].push_back(last_transition);
            current_states.pop_back();
            currentWordState = current_states.back();
        }

        for (auto it = forbidden_transitions.begin(); it != forbidden_transitions.end();)
        {
            if (it->first < word.length())
            {
                it = forbidden_transitions.erase(it);
            }
            else
            {
                ++it;
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

    std::cout << "(press \"Enter\" for more or type anything to exit)\n Suggestions: " << std::endl;
    std::string user_input = "";
    std::string last_suggestion = "";
    std::getline(std::cin, user_input);
    while (user_input == "")
    {
        autocomlete_word(inputWord);
        
        if (suggestions.back() != last_suggestion)
        {
            std::cout << suggestions.back() << std::endl;
            last_suggestion = suggestions.back();
            std::getline(std::cin, user_input);
        }
        else
        {
            std::cout << "That's all" << std::endl;
            break;
        }
    }

    std::cout << "End of program" << std::endl;

    return 0;
}