#include "game.h"
#include "Question.h"
#include <iostream>
#include <clocale>

std::vector<Question> questions;   

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        questions = loadQuestionsFromFile("questions.json");
        std::cout << "Loaded " << questions.size() << " questions.\n";
        GameProcess();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}