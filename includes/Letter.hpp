//
// Created by sansh on 24/06/2023.
//

#ifndef WORDLE_LETTER_HPP
#define WORDLE_LETTER_HPP


class Letter {
public:
    typedef enum {
        NONE,
        INWORD,
        CORRECT
    } e_state;

private:
    char c;
    e_state state;
    Letter();

public:
    explicit Letter(char letter);
    explicit Letter(char letter, int state);
    Letter(const Letter &copy);

    void setState(e_state state);

    ~Letter();

    char getC() const;
    e_state getState() const;

};

struct comp
{
    bool operator()(const Letter &let1, const Letter &let2) const
    {
        return (let1.getC() == let2.getC());
    }
};


#endif //WORDLE_LETTER_HPP
