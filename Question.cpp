#include "Question.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

Question::Question() : id(0), level(1), correct(0) {
    options.fill("");
}

Question::Question(int i, int lvl, const std::string& q,
                   const std::array<std::string, 4>& opts, int c)
    : id(i), level(lvl), question(q), options(opts), correct(c) {}

bool Question::isCorrect(char answerIndex) const {
    return (answerIndex - 'A') == correct;
}

void Question::print() const {
    std::cout << question << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << "  " << char('A' + i) << ") " << options[i] << "\n";
    }
}

std::vector<Question> loadQuestionsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }
    json j;
    file >> j;
    return j.get<std::vector<Question>>();
}