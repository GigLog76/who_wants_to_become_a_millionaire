#include "Question.h"
#include <iostream>
#include <fstream>
#include <stdexcept>


bool Question::isCorrect(char answerIndex) const {
    return (answerIndex - 'A') == correct;
}

void Question::print() const {
    std::cout << question << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << "  " << char('A' + i) << ") " << options[i] << "\n";
    }
}