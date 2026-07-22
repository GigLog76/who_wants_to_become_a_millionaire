#pragma once
#include <vector>
#include <string>
#include "Question.h"

struct SessionData {
    int stage = 0;
    int prize = 0;
    bool lose = false;
    bool exit = false;
    std::string selectedCategory = "Все";
    std::vector<int> usedQuestionIds;
    bool halfUsed = false;
    bool audienceUsed = false;
    bool secondChanceUsed = false;
};

bool saveSession(const SessionData& data, const std::string& filename = "session.json");
bool loadSession(SessionData& data, const std::string& filename = "session.json");
bool sessionExists(const std::string& filename = "session.json");
void deleteSession(const std::string& filename = "session.json");