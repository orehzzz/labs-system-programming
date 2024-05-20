#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

int red = 31;    // reserved word
int orange = 32; // operator
int blue = 34;   // number
int yellow = 33; // identifier
int cyan = 36;   // string
int white = 37;  // delimiters
int gray = 90;   // comment
int green = 92;  // error

std::vector<std::string> reserved_words = {
    "False", "await", "else", "import", "pass", "None", "break", "except", "in", "raise",
    "True", "class", "finally", "is", "return", "and", "continue", "for", "lambda", "try",
    "as", "def", "from", "nonlocal", "while", "assert", "del", "global", "not", "with",
    "async", "elif", "if", "or", "yield"};

std::vector<std::string> operators = {
    "+", "-", "*", "/", "//", "%", "**",
    "==", "!=", ">", "<", ">=", "<=",
    "and", "or", "not", "is", "is not",
    "in", "not in", "&", "|", "^", "~", "<<", ">>"};

std::vector<std::string> delimiters = {
    "(", ")", "[", "]", "{", "}", ",", ":", ".",
    "+=", "-=", "*=", "/=", "//=", "%=", "**=", "&=", "|=", "^=", ">>=", "<<=", "="};

std::vector<std::string> identifiers;

int main()
{
    std::ifstream file("test.py");

    char c;
    while (file.get(c))
    {
        // coments
        if (c == '#')
        {
            std::cout << "\033[" << gray << "m" << c;
            while (file.get(c) && c != '\n')
            {
                std::cout << c;
            }
            std::cout << "\033[0m";
        }

        // strings
        if (c == '"' or c == '\'')
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
        }

        std::cout << c;
    }

    file.close();
    return 0;
}