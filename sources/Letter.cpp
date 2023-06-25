//
// Created by sansh on 24/06/2023.
//

#include "../includes/Letter.hpp"

Letter::Letter() = default;

Letter::Letter(char letter) {
    this->c = letter;
    this->state = NONE;
}

Letter::Letter(char letter, int state) {
    this->c = letter;
    this->state = (Letter::e_state)state;
}

Letter::Letter(const Letter &copy) {
    this->c = copy.c;
    this->state = copy.state;
}

Letter::~Letter() = default;

char Letter::getC() const {
    return this->c;
}

Letter::e_state Letter::getState() const{
    return this->state;
}

void Letter::setState(Letter::e_state state) {
    Letter::state = state;
}

