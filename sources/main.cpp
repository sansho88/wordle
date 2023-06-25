#include "../includes/main.hpp"
#include "../includes/Letter.hpp"

void	printWelcome(void)
{
	std::string	welcomeStr[12] = {
		"#####################################################################################",
		"#                                                                                   #",
		"#         **       **    *******    *******    *******    **        ********        #",
		"#        /**      /**   **/////**  /**////**  /**////**  /**       /**/////         #",
		"#        /**   *  /**  **     //** /**   /**  /**    /** /**       /**              #",
		"#        /**  *** /** /**      /** /*******   /**    /** /**       /*******         #",
		"#        /** **/**/** /**      /** /**///**   /**    /** /**       /**////          #",
		"#        /**** //**** //**     **  /**  //**  /**    **  /**       /**              #",
		"#        /**/   ///**  //*******   /**   //** /*******   /******** /********        #",
		"#        //       //    ///////    //     //  ///////    ////////  ////////         #",
		"#                                                                                   #",
		"#####################################################################################"
	};
	for (int i = 0; i < 12; i++)
		std::cout << welcomeStr[i] << std::endl;
}

void	printRules(unsigned int dictSize)
{
	std::cout << std::endl << std::endl;
	std::cout << "RULES OF THE GAME:" << std::endl << std::endl;
	std::cout << "1#   6 guesses to find the WORDLE" << std::endl;
	std::cout << "2#   you can only enter five letter WORDLES" << std::endl;
	std::cout << "3#   the letters can only be lowercase" << std::endl;
	std::cout << "4#   a GREEN letter means it is valid and at the right place in the WORDLE" << std::endl;
	std::cout << "5#   a YELLOW letter means it is valid but not at the right place in the WORDLE" << std::endl;
	std::cout << "6#   a GREY letter means it is not valid for the WORDLE" << std::endl;
	std::cout << std::endl << "7#   find the WORDLE" << std::endl;
	std::cout << std::endl << "Today the pool of WORDLE size is: " + std::to_string(dictSize) << std::endl;
	std::cout << std::endl << std::endl;
}

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

bool	closeError(std::ifstream *inputStream, std::string msg)
{
	inputStream->close();
	std::cerr << msg << std::endl;
    return false;
}

bool    parseDict(std::ifstream *is, std::vector<std::string> &dic)
{
    std::string line;
    struct stat fileStat;
    size_t      dictSize = 0;

    if (!is->is_open())
    {
        std::cerr << "Error: Could not open dictionnary" << std::endl;
        return (false);
    }
    else if (!(stat("./words.txt", &fileStat) == 0 && S_ISREG(fileStat.st_mode)))
        return (closeError(is, "Error: ./words.txt is not a regular file"));
    while(std::getline(*is, line))
    {
        dictSize++;
        if (is->fail())
			return (closeError(is, "Error: fail while reading"));
		else if (dictSize > MAX_DICT_SIZE)
			return (closeError(is, "Error: dictionnary has more than " + std::to_string(MAX_DICT_SIZE) + " entries"));
		else if (line.size() != 5)
			return (closeError(is, "Error: dictionnary entry " + std::to_string(dictSize) + " does not have a length of " + std::to_string(WORD_SIZE)));
		for (unsigned int i = 0; i < line.size(); i++)
		{
            line[i] = std::tolower(line[i]);
			if (!islower(line[i]))
				return closeError(is, "Error: dictionnary entry " + std::to_string(dictSize) + " does not contain only alphabetic characters");
		}
        dic.push_back(line);
    }
    if (dictSize < MIN_DICT_SIZE)
        return (closeError(is, "Error: dictionnary has less than " + std::to_string(MIN_DICT_SIZE) + " entries"));
    else if (!is->eof() && !is->good())
		return (closeError(is, "Error: error while reading dictionnary file"));
    is->close();
    return (true);
}

int main() {
    std::vector<std::string> dic;
    std::ifstream is("words.txt"); //todo: parametre
    std::string line;
    std::string wordOfTheDay;
    int playerLives = LIVES;
    if (!parseDict(&is, dic))
    {
        std::cerr << "Error: BAD DICT" << std::endl;
        return 2;
    }
    wordOfTheDay = getRandomWord(dic, true);
    line.clear();
    while (playerLives > 0 && std::getline(std::cin, line))
    {
        if (line.empty())
            break;
        if (line.length() != WORD_SIZE)
        {
            std::cerr << CLR_LAST_LINE << RED + line + RESET << "(Wrong amount of letters)" << std::endl;
            sleep(1);
            std::cout << CLR_LAST_LINE;
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
                std::cout << CLR_LAST_LINE << GREEN + line << "\t CONGRATULATIONS !!!\n" << RESET;
                return 0;
            }
            --playerLives;
            std::cout << " [" RED "♥ " RESET <<  playerLives << "]" << std::endl;
        }
        else
        {
            std::cout << CLR_LAST_LINE << RED + line + RESET << std::endl;
            sleep(1);
            std::cout << CLR_LAST_LINE;
        }
    }
    std::cout << "You loose :'(\nIt was: " << wordOfTheDay << std::endl;
    return 0;
}
