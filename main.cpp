#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>

#define LIVES 6
#define GREEN "\e[92m"
#define YELLOW "\e[93m"
#define GREY "\e[90m"
#define RESET "\e[0m"
#define CLR_LAST_LINE "\e[1A\e[K"

typedef enum {
    NONE,
    INWORD,
    CORRECT
} e_state;

bool isWordKnown(std::vector<std::string> &refList, const std::string &word)
{
    for (const auto &item: refList) {
        if (item == word)
            return true;
    }
    return false;
}

const std::string &getRandomWord(std::vector<std::string> &list, bool funMode)
{
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    int todayTime = now->tm_yday + now->tm_mon + now->tm_year + (now->tm_min * now->tm_sec * funMode * rand());
    return (list.at(todayTime % list.size() ));
}

std::string paintLetter(char c, const std::string &color)
{
    std::string result = color + c + RESET;
    return result;
}

void getNbCommonLetters(const std::string &pword, const std::string &target) {
    static std::map<char, int> wrongLetters;

    for (int i = 0; i < pword.length(); ++i) {
        if (pword.at(i) == target.at(i))
            std::cout << paintLetter(pword.at(i), GREEN);
        else if (target.find(pword.at(i)) != std::string::npos)
            std::cout << paintLetter(pword[i], YELLOW) ;
        else
        {
            std::cout << paintLetter(pword[i], GREY);
            wrongLetters[pword[i]];
        }
    }
    std::cout << GREY " (All Wrong Letters tried: ";
    std::map<char, int>::iterator wlIt = wrongLetters.begin();

    for (;wlIt != wrongLetters.end(); ++wlIt) {
        std::cout << wlIt->first << " ";
    }
    std::cout << ")" RESET;
}

int main() {
    std::vector<std::string> dic;
    std::ifstream is(R"(C:\Users\sansh\CLionProjects\wordle\words.txt)");
    std::string line;
    std::string wordOfTheDay;
    int playerLives = LIVES;
    if (is.is_open())
    {
        while (std::getline(is, line))
            dic.push_back(line);
        std::cerr << "words in the dic: " << dic.size() << std::endl;
        is.close();
    }
    else
    {
        std::cerr << "Error: No dictionary found" << std::endl;
        return 2;
    }
    wordOfTheDay = getRandomWord(dic, true);
    line.clear();
    while (playerLives >= 0 && std::getline(std::cin, line))
    {
        if (line.empty())
            break;
        if (line.length() != 5)
        {
            std::cerr << "Wrong amount of letters" << std::endl;
            continue;
        }
        for (size_t i = 0; i < line.length(); ++i)
            line.at(i) = (char)std::tolower(line.at(i));
        /*todo:
         * 1- détecter le nombre de lettres communes au mot et à la target
         * 2- détecter si la position de certaines lettres du mot correspond à celle des lettres de la target
         * 3- compter le nombre d'essais du joueur
         * 4- colorer les lettres en fonction de leur justesse*/
        if (isWordKnown(dic, line))
        {
            if (line != wordOfTheDay)
            {
                std::cout << CLR_LAST_LINE;
                getNbCommonLetters(line, wordOfTheDay);
            }
            else
            {
                std::cout << GREEN << "CONGRATULATIONS !!!\n" << RESET;
                return 0;
            }
            --playerLives;
            std::cout << "\nTr" << (playerLives > 1? "ies": "y") << " remaining: " <<  playerLives << std::endl;
        }
        else
            std::cout << line << " doesn't exists in our dictionary :(" << std::endl;
        std::cout << std::endl;
    }
    std::cout << "You loose :'(\nIt was: " << wordOfTheDay;
    return 0;
}
