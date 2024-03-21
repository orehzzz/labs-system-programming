#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <list>
#include <cctype>

struct Node
{
    std::string data;
    struct Node *next;
};

bool isVowel(char c)
{
    c = tolower(c);
    return c == 'a' || c == 'o' || c == 'u' || c == 'i' || c == 'y' || c == 'e';
};

std::string word_detector(std::string input)
{
    std::string word_with_vowels = "";
    for (int i = 0; i < input.length(); i++)
    {
        char *last_char = &input.back();

        // last char
        if (last_char == &input[i])
        {
            if (isVowel(input[i]))
            {
                word_with_vowels += input[i];
            };
            if (!isVowel(input[i]) && isalpha(input[i]))
            {
                word_with_vowels = "";
            }
            if (word_with_vowels != "")
            {
                std::cout << " answer(" << word_with_vowels << ")";
            };
            return word_with_vowels;
        }

        if (isalpha(input[i]))
        {
            if (isVowel(input[i]))
            {
                word_with_vowels += input[i];
                continue;
            }
            else // consonant
            {
                word_with_vowels = "";
                for (int j = i; j < input.length(); j++)
                {
                    if ((!isalpha(input[j])) && (last_char != &input[j]))
                    {
                        input.erase(0, j + 1);
                        word_detector(input);
                        break;
                    };
                };
                if (word_with_vowels != "")
                {
                    std::cout << " answer(" << word_with_vowels << ")";
                };
                return word_with_vowels;
            }
        }
        else // symbol
        {
            if (last_char != &input[i])
            {
                input.erase(0, i + 1);
                word_detector(input);
                if (word_with_vowels != "")
                {
                    std::cout << " answer(" << word_with_vowels << ")";
                };
                return word_with_vowels;
            }
        };
    };
};

int main()
{

    std::ifstream file("Test2.txt");
    std::istream_iterator<std::string> fileIterator(file);
    std::istream_iterator<std::string> endIterator;
    while (fileIterator != endIterator)
    {
        std::string word = *fileIterator;
        std::cout << '\n'
                  << *fileIterator << '\n';
        word_detector(word);
        ++fileIterator;
    };
    return 0;
}
