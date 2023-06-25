//
// Created by tgriffit on 6/25/23.
//

#ifndef WORDLE_GAME_HPP
#define WORDLE_GAME_HPP

#include "main.hpp"
#include "Letter.hpp"

class Game {
    size_t lives;
    std::string wordOfTheDay;
    std::vector<std::string> dictionary;

    bool isWordKnown(std::vector<std::string> &refList, const std::string &word);
    void checkLetters(const std::string &pword, const std::string &target);
    size_t getNbSameLettersFound(char c, std::map<size_t , Letter> &checked);
    size_t getNb1LetterTarget(char c, const std::string &target);
    std::map<size_t , Letter> getLettersAtCorrectPlace(const std::string &pword, const std::string &target);
    bool isThereAnOtherLetterToFind(char target, std::map<size_t,Letter> &letters);
    bool isAtCorrectPlace(char c, size_t cpos, const std::string &target);
    std::string paintLetter(char c, const std::string &color);
    void rewriteLine(const std::string &line, const std::string &color, bool isError);

public:
    Game();
    Game(size_t lives, std::vector<std::string> dictionary);
    ~Game();

    void play();
    const std::string &getRandomWord(std::vector<std::string> &list, bool funMode);

};


#endif //WORDLE_GAME_HPP
