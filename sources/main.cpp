#include "../includes/main.hpp"
#include "../includes/Letter.hpp"
#include "../includes/Game.hpp"

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
		std::cout << CYAN << welcomeStr[i] << RESET << std::endl;
}

void	printRules(unsigned long dictSize)
{
	std::cout << std::endl << std::endl;
	std::cout << "RULES OF THE GAME:" << std::endl << std::endl;
	std::cout << "0#   find the WORDLE" << std::endl << std::endl;
	std::cout << "1#   " << WORD_SIZE + 1 << " guesses to find the WORDLE" << std::endl;
	std::cout << "2#   you can only enter " << WORD_SIZE <<" letters WORDLES" << std::endl;
	std::cout << "3#   the letters can only be lowercase" << std::endl;
	std::cout << "4#   a "<< GREEN "GREEN" RESET << " letter means it is valid and at the right place in the WORDLE" << std::endl;
	std::cout << "5#   a " << YELLOW "YELLOW" RESET << " letter means it is valid but not at the right place in the WORDLE" << std::endl;
	std::cout << "6#   a "<< GREY "GREY" RESET << " letter means it is not valid for the WORDLE" << std::endl;
    std::cout << "7#   to loose press ENTER without any letter" << std::endl;
	std::cout << std::endl << "Today the pool of WORDLE size is: " + std::to_string(dictSize) << std::endl;
	std::cout << std::endl << std::endl;
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

int main(int argc, char *argv[]) {
    std::vector<std::string> dic;
    std::string dicPath = "words.txt";
    if (argc == 2)
        dicPath = argv[1];
    std::ifstream is(dicPath);

    if (!parseDict(&is, dic))
        return 2;
    printWelcome();
    printRules(dic.size());
    Game game(LIVES, dic);
    game.play();
    return 0;
}
