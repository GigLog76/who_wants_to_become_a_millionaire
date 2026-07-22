#pragma once
#include "Question.h"
#include <vector>
#include <string>

bool loadQuestions(std::vector<Question>& questions, const std::string& filename = "questions.json");
bool saveQuestions(const std::vector<Question>& questions, const std::string& filename = "questions.json");