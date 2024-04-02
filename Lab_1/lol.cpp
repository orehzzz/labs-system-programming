#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <list>

std::list<std::string> result;

bool isVowel(char c)
{
    c = tolower(c);
    return c == 'a' || c == 'o' || c == 'u' || c == 'i' || c == 'y' || c == 'e';
};

bool add_if_unique(std::string word)
{
    for (std::string n : result)
    {
        if (n == word)
        {
            return -1;
        }
    };
    result.push_front(word);
    return 0;
};

int main()
{
    std::ifstream file("Test2.txt");
    std::istreambuf_iterator<char> iter(file);
    std::istreambuf_iterator<char> endIter;
    std::string word_of_vowels;

    while (iter != endIter)
    {
        // word.length() == 30: *iter letter -> iter ++ to non letter. non-letter -> add. CLEAR ANYWAY
        if (word_of_vowels.length() == 30)
        {
            if (isalpha(*iter))
            {
                while (isalpha(*iter) && (iter != endIter))
                {
                    ++iter;
                };
            }
            else
            {
                add_if_unique(word_of_vowels);
                ++iter;
            };
            word_of_vowels = "";
            continue;
        }

        // vowel: add to word, ++iter
        if (isVowel(*iter))
        {
            word_of_vowels += *iter;
            ++iter;
            continue;
        }
        // consonant: clear word, move to non-letter
        else if (isalpha(*iter))
        {
            word_of_vowels = "";
            while (isalpha(*iter) && (iter != endIter))
            {
                ++iter;
            };
            continue;
        }
        // non-letter: add word if word not empty(and make word empty afterwards), ++iter
        else
        {
            if (word_of_vowels != "")
            {
                add_if_unique(word_of_vowels);
            };
            word_of_vowels = "";
            ++iter;
        };
    };
    if (word_of_vowels != "")
    {
        add_if_unique(word_of_vowels);
    };

    for (std::string word : result)
    {
        std::cout << word << " ";
    };

    return 0;
};