#include "game.h"
#include "Question.h"
#include <iostream>
#include "QuestionManager.h" 
#include <clocale>
#include <windows.h>


extern std::vector<Question> questions;

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    try {
        if (!loadQuestions(questions, "questions.json")) {
            std::cerr << "Не удалось загрузить вопросы!" << std::endl;
            return 1;
        }
        std::cout << "Loaded " << questions.size() << " questions.\n";
        GameProcess();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}