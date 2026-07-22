#pragma once

#include <string>
#include <array>
#include <vector>
#include "json.hpp"   

using json = nlohmann::json;

struct Question {
    int id;
    int level;
    std::string category;
    std::string question;
    std::array<std::string, 4> options;
    int correct;

    Question() : id(0), level(1), correct(0) { options.fill(""); }
    Question(int i, int lvl, const std::string& cat, const std::string& q,
             const std::array<std::string, 4>& opts, int c)
        : id(i), level(lvl), category(cat), question(q), options(opts), correct(c) {}

    bool isCorrect(char answerIndex) const;
    void print() const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Question, id, level, category, question, options, correct)


std::vector<Question> loadQuestionsFromFile(const std::string& filename);