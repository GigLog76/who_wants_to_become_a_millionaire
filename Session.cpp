#include "Session.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SessionData, stage, prize, lose, exit, selectedCategory, usedQuestionIds, halfUsed, audienceUsed, secondChanceUsed)

bool saveSession(const SessionData& data, const std::string& filename) {
    json j = data;
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось сохранить сессию!" << std::endl;
        return false;
    }
    file << j.dump(4);
    return true;
}

bool loadSession(SessionData& data, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    try {
        json j;
        file >> j;
        data = j.get<SessionData>();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки сессии: " << e.what() << std::endl;
        return false;
    }
}

bool sessionExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.is_open();
}

void deleteSession(const std::string& filename) {
    std::remove(filename.c_str());
}