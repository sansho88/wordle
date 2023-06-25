//
// Created by tgriffit on 6/25/23.
//

#include "../includes/Game.hpp"

Game::Game() {

}

Game::Game(size_t lives, bool funMode, std::vector<std::string> dictionary)
        : lives(lives),  funMode(funMode), dictionary(dictionary) {}

Game::~Game() {

}

std::string Game::paintLetter(char c, const std::string &color)
{
    std::string result = color + c + RESET;
    return result;
}

bool Game::isAtCorrectPlace(char c, size_t cpos, const std::string &target){
    return target.at(cpos) == c;
}

bool Game::isThereAnOtherLetterToFind(char target, std::map<size_t,Letter> &letters) {
    std::map<size_t,Letter>::iterator it = letters.begin();
    for (; it != letters.end(); ++it)
    {
        Letter l = it->second;
        if (target == l.getC() && l.getState() != Letter::e_state::CORRECT)
            return true;
    }

    return false;
}

std::map<size_t , Letter> Game::getLettersAtCorrectPlace(const std::string &pword, const std::string &target){
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

size_t Game::getNb1LetterTarget(char c, const std::string &target) {
    size_t result = 0;
    for (char letter: target) {
        result += (letter == c);
    }
    return result;
}

size_t Game::getNbSameLettersFound(char c, std::map<size_t , Letter> &checked) {
    size_t result = 0;
    std::map<size_t , Letter>::iterator it = checked.begin();
    for (; it != checked.end(); ++it) {
        if (it->second.getC() == c && it->second.getState() != Letter::e_state::NONE)
            ++result;
    }
    return result;
}

void Game::checkLetters(const std::string &pword, const std::string &target) {
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

bool Game::isWordKnown(std::vector<std::string> &refList, const std::string &word)
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
const std::string &Game::getRandomWord(std::vector<std::string> &list, bool funMode)
{
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    int todayTime = now->tm_yday + now->tm_mon * now->tm_year + (now->tm_min * now->tm_sec * funMode * rand());
    return (list.at(todayTime % list.size() ));
}


void Game::rewriteLine(const std::string &line, const std::string &color, bool isError) {
    std::cout << CLR_LAST_LINE << color + line + RESET << std::endl;
    if (isError)
    {
        sleep(1);
        std::cout << CLR_LAST_LINE;
    }
}

void Game::play() {
    std::string line;
    wordOfTheDay = getRandomWord(dictionary, funMode);

    while (lives > 0 && std::getline(std::cin, line))
    {
        if (line.empty())
            break;
        if (line.length() != LIVES - 1)
        {
            rewriteLine(line + " (Wrong amount of letters)", RED, true);
            continue;
        }
        for (size_t i = 0; i < line.length(); ++i)
            line.at(i) = (char)std::tolower(line.at(i));
        if (isWordKnown(dictionary, line))
        {
            if (line != wordOfTheDay)
            {
                std::cout << CLR_LAST_LINE;
                checkLetters(line, wordOfTheDay);
            }
            else
            {
                rewriteLine(line + "\t CONGRATULATIONS !!!\n", GREEN, false);
                exit(0);
            }
            --lives;
            std::cout << " [" RED "♥ " RESET <<  lives << "]" << std::endl;
        }
        else
        {
            rewriteLine(line, RED, true);
        }
    }
    std::cout << "You loose :'(\nIt was: " << wordOfTheDay << std::endl;
}

