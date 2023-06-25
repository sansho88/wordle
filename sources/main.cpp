#include "../includes/main.hpp"
#include "../includes/Letter.hpp"
#include "../includes/Game.hpp"


bool    parseDict(std::ifstream *is, std::vector<std::string> &dic)
{
    std::string line;

    if (!is->is_open())
        return (false);
    while(std::getline(*is, line))
        dic.push_back(line);
    return (true);
}

int main(int argc, char *argv[]) {
    std::vector<std::string> dic;
    std::string dicPath = "words.txt";
    if (argc == 2)
        dicPath = argv[1];
    std::ifstream is(dicPath);

    if (!parseDict(&is, dic))
    {
        std::cerr << "Error: BAD DICT" << std::endl;
        return 2;
    }
    Game game(LIVES, dic);
    game.play();
    return 0;
}
