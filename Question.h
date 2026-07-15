#pragma once

#include <string>
#include <array>
#include <vector>
#include "json.hpp"   

using json = nlohmann::json;

struct Question {
    int id;
    int level;
    std::string question;
    std::array<std::string, 4> options;
    int correct;      

    Question();   
    Question(int i, int lvl, const std::string& q,
             const std::array<std::string, 4>& opts, int c);

    bool isCorrect(char answerIndex) const;
    void print() const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Question, id, level, question, options, correct)

std::vector<Question> loadQuestionsFromFile(const std::string& filename);