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

bool validate_input_word(std::string inputWord) // add check if in end state then add as suggestion
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
    std::list<int> forbidden_transitions;
    int last_transition = -1;

    current_states.push_back(currentWordState);

    while (should_continue)
    {
        if (word.length() > 10)
        {
            break;
        }

        bool transition_found = false;
        // std::cout << "Current word_state: " << currentWordState << std::endl;
        // std::cout << "word: " << word<< std::endl;
        for (int i = 0; i < transitions.size(); i++) //check all transitions
        {
            std::cout << "Forbidden transition: ";
            for (int forbidden_transition : forbidden_transitions)
            {
                std::cout << forbidden_transition;
            }
            std::cout << std::endl;
            if ((transitions[i][0] - '0' == currentWordState) && (!is_forbidden_transition(i, forbidden_transitions)))
            {
                word += transitions[i][1];
                // std::cout << "Transition: " << transitions[i][0] << " " << transitions[i][1] << " " << transitions[i][2] << std::endl;
                tried_transitions[currentWordState].insert(i); // position of function, count from 0
                currentWordState = transitions[i][2] - '0';
                should_continue = true;
                transition_found = true;
                current_states.push_back(i); //
                last_transition = i;

                if (isEndState(currentWordState))
                {
                    if (is_unique(inputWord + word))
                    {
                        forbidden_transitions.clear();
                        suggestions.push_back(inputWord + word);

                        // std::cout << "Suggestion: " << inputWord + word << std::endl;

                        current_states.clear();


                        transition_found = true;//to not go back
                        should_continue = false;
                        
                        // std::cout << "Tried Transitions: " << std::endl;
                        // for (const auto &entry : tried_transitions)
                        // {
                        //     std::cout << "State " << entry.first << ": ";
                        //     for (int transition : entry.second)
                        //     {
                        //         std::cout << transition << " ";
                        //     }
                        //     std::cout << std::endl;
                        // }
                        break;
                    }
                }
            }

        }
        //after adding(or not) a letter, if no transition found, go back 1 step (+ban transition)
        if (!transition_found)
        {
            if (word.length() == 0) //in initisl state, all transitions tried
            {
                break;
            }
            forbidden_transitions.push_back(last_transition);
            word.pop_back();
            current_states.pop_back();
            currentWordState = current_states.back();
        }
        // std::cout << "State after iteration: " << currentWordState << std::endl;
        // std::cout << "Word after iteration: " << inputWord + word << std::endl;

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

    // for (int i = 0; i < SUGGESTIONS_COUNT; i++)
    // {
    //     std::cout << "Autocomplete" << i + 1 << ':'<< std::endl;
    //     autocomlete_word(inputWord);
    // }

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
        }else{
            std::cout << "That's all" << std::endl;
            break;
        }
    }

    std::cout << "End of program" << std::endl;

    // for (std::string suggestion : suggestions)
    // {
    //     std::cout << suggestion << std::endl;
    // }

    return 0;
}