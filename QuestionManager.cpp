#include "QuestionManager.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;


bool loadQuestions(std::vector<Question>& questions, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        questions.clear();
        return true;
    }
    try {
        json j;
        file >> j;
        questions = j.get<std::vector<Question>>();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки вопросов: " << e.what() << std::endl;
        return false;
    }
}

bool saveQuestions(const std::vector<Question>& questions, const std::string& filename) {
    json j = questions;
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для записи вопросов!" << std::endl;
        return false;
    }
    file << j.dump(4);
    return true;
}