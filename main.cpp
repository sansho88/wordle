#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <unistd.h>

#include "Letter.hpp"

#define LIVES 6
#define GREEN "\e[92m"
#define YELLOW "\e[93m"
#define RED "\e[91m"
#define GREY "\e[90m"
#define RESET "\e[0m"
#define CLR_LAST_LINE "\e[1A\e[K"



bool isWordKnown(std::vector<std::string> &refList, const std::string &word)
{
    for (const auto &item: refList) {
        if (item == word)
            return true;
    }
    return false;
}

/**
 *
 * @param list: the file converted into a container (for limits the i/o operations)
 * @param funMode: false=1word/day; true=1word/launch
 * @return a random picked word from the ref file
 */
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

bool isAtCorrectPlace(char c, size_t cpos, const std::string &target){
    return target.at(cpos) == c;
}

bool isThereAnOtherLetterToFind(char target, std::map<size_t,Letter> &letters) {
    std::map<size_t,Letter>::iterator it = letters.begin();
    for (; it != letters.end(); ++it)
    {
        Letter l = it->second;
        if (target == l.getC() && l.getState() != Letter::e_state::CORRECT)
            return true;
    }

    return false;
}

std::map<size_t , Letter> getLettersAtCorrectPlace(const std::string &pword, const std::string &target){
    std::map<size_t , Letter> result;
    for (size_t i = 0; i < pword.length(); ++i) {
        result.insert(
                std::pair<size_t, Letter>(
                        i, Letter(pword[i], isAtCorrectPlace(pword[i], i, target) * Letter::e_state::CORRECT)
                )
        );
    }
    return result;
}

size_t getNb1LetterTarget(char c, const std::string &target) {
    size_t result = 0;
    for (char letter: target) {
        result += (letter == c);
    }
    return result;
}

size_t getNbSameLettersFound(char c, std::map<size_t , Letter> &checked) {
    size_t result = 0;
    std::map<size_t , Letter>::iterator it = checked.begin();
    for (; it != checked.end(); ++it) {
        if (it->second.getC() == c && it->second.getState() != Letter::e_state::NONE)
            ++result;
    }
    return result;
}

void checkLetters(const std::string &pword, const std::string &target) {
    static std::map<char, int> wrongLetters;
    std::map<size_t , Letter> checked = getLettersAtCorrectPlace(pword, target);

    std::map<size_t , Letter>::iterator checkedIt = checked.begin();

    for (size_t pos = 0; checkedIt != checked.end() ; ++checkedIt, ++pos) {
        char c = checkedIt->second.getC();
        const Letter::e_state state = checkedIt->second.getState();
        size_t nbOfC = getNb1LetterTarget(c, target);
        size_t cLeftToFind = nbOfC - getNbSameLettersFound(c, checked);

        if (state == Letter::e_state::CORRECT)
            std::cout << paintLetter(c, GREEN);
        else if (target.find(checked.at(pos).getC()) != std::string::npos &&
            (isThereAnOtherLetterToFind(c, checked)) && cLeftToFind != 0)
        {
            checkedIt->second.setState(Letter::e_state::INWORD);
            std::cout << paintLetter(c, YELLOW);
        }
        else
        {
            std::cout << paintLetter(c, GREY);
            wrongLetters[c];
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
    std::ifstream is("words.txt");
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
    while (playerLives > 0 && std::getline(std::cin, line))
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
        if (isWordKnown(dic, line))
        {
            if (line != wordOfTheDay)
            {
                std::cout << CLR_LAST_LINE;
                checkLetters(line, wordOfTheDay);
            }
            else
            {
                std::cout << GREEN << "CONGRATULATIONS !!!\n" << RESET;
                return 0;
            }
            --playerLives;
            std::cout << " [" RED "♥ " RESET <<  playerLives << "]" << std::endl;
        }
        else
        {
            std::cout << CLR_LAST_LINE << RED + line + RESET << std::endl;
        }
    }
    std::cout << "You loose :'(\nIt was: " << wordOfTheDay << std::endl;
    return 0;
}
