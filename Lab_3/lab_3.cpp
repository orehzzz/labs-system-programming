#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

int red = 31;    // reserved word
int yellow = 33; // operator
int blue = 34;   // number
int purple = 35; // identifier
int cyan = 36;   // string
int white = 37;  // delimiter
int gray = 90;   // comment
int green = 92;  // error

std::vector<std::string> reserved_words = {
    "False", "await", "else", "import", "pass", "None", "break", "except", "raise",
    "True", "class", "finally", "return", "continue", "for", "lambda", "try",
    "as", "def", "from", "nonlocal", "while", "assert", "del", "global", "with",
    "async", "elif", "if", "yield", "f"};

std::vector<std::string> operators = {
    "+", "-", "*", "/", "//", "%", "**",
    "==", "!=", ">", "<", ">=", "<=",
    "and", "or", "not", "is", "is not",
    "in", "not in", "&", "|", "^", "~", "<<", ">>", "@"};

std::vector<std::string> delimiters = {
    "(", ")", "[", "]", "{", "}", ",", ":", ".",
    "+=", "-=", "*=", "/=", "//=", "%=", "**=", "&=", "|=", "^=", ">>=", "<<=", "="};

std::vector<std::string> identifiers;

int main()
{
    std::ifstream file("Test1.py");

    char c;
    while (file.get(c))
    {
        // print spaces, tabs and new lines
        if (c == '\n' or c == '\t' or c == ' ')
        {
            std::cout << c;
            continue;
        }
        // coments
        else if (c == '#')
        {
            std::cout << "\033[" << gray << "m" << c;
            while (file.get(c) && c != '\n')
            {
                std::cout << c;
            }
            std::cout << "\033[0m";
            std::cout << '\n';
        }

        // strings
        else if (c == '"' or c == '\'')
        {
            char start_char = c;
            std::cout << "\033[" << cyan << "m" << c;
            bool skip_char = false;

            while (file.get(c))
            {
                if (skip_char)
                {
                    skip_char = false;
                    continue;
                }
                if (c == '\\')
                {
                    skip_char = true;
                }
                if (c == start_char)
                {
                    break;
                }
                std::cout << c;
            }

            std::cout << c << "\033[0m";
            continue;
        }
        // reserved words, identifiers and some operators
        else if (isalpha(c) or c == '_')
        {
            std::string word = "";
            word += c;
            while (file.get(c) && (isalnum(c) or c == '_'))
            {
                word += c;
            }

            if (std::find(reserved_words.begin(), reserved_words.end(), word) != reserved_words.end())
            {
                std::cout << "\033[" << red << "m" << word << "\033[0m";
            }
            else if (std::find(operators.begin(), operators.end(), word) != operators.end())
            {
                std::cout << "\033[" << yellow << "m" << word << "\033[0m";
            }
            else
            {
                if (std::find(identifiers.begin(), identifiers.end(), word) == identifiers.end())
                {
                    identifiers.push_back(word);
                }
                std::cout << "\033[" << purple << "m" << word << "\033[0m";
            }
            file.unget();
            continue;
        }
        // numbers
        else if (isdigit(c))
        {
            std::string number = "";
            number += c;
            while (file.get(c) && (isalnum(c) or c == '.' or c == '+' or c == '-'))
            {
                number += c;
            }
            std::cout << "\033[" << blue << "m" << number << "\033[0m";
            file.unget();
            continue;
        }
        // delimiters and operators, they could be several caracters long
        else if (ispunct(c))
        {
            // check if it is a number with a dot infront(peek if next element is a digit). if it is, color this dot as a number
            if (c == '.' && isdigit(file.peek()))
            {
                std::cout << "\033[" << blue << "m" << c << "\033[0m";
                continue;
            }

            std::string line_of_characters = "";
            line_of_characters += c;
            while (file.get(c) && ispunct(c) && c != '"' && c != '\'')
            {
                line_of_characters += c;
            }
            bool continue_dividing = true;
            while (continue_dividing)
            {
                if (std::find(operators.begin(), operators.end(), line_of_characters) != operators.end())
                {
                    std::cout << "\033[" << yellow << "m" << line_of_characters << "\033[0m";
                    continue_dividing = false;
                }
                else if (std::find(delimiters.begin(), delimiters.end(), line_of_characters) != delimiters.end())
                {
                    std::cout << "\033[" << white << "m" << line_of_characters << "\033[0m";
                    continue_dividing = false;
                }
                else if (line_of_characters.size() > 1)
                {
                    line_of_characters.pop_back();
                    file.unget();
                }
                else
                {
                    std::cout << "\033[" << green << "m" << line_of_characters << "\033[0m";
                    continue_dividing = false;
                }
            }

            file.unget();
            continue;
        }
        // anything else
        else
        {
            std::cout << "\033[" << green << "m" << c << "\033[0m";
        }
    }

    file.close();
    return 0;
}