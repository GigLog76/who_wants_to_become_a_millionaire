#include "records.h"
#include <fstream>
#include <algorithm>
#include <iostream>

using json = nlohmann::json;

bool loadRecords(std::vector<Record>& records, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        records.clear();
        return true;
    }
    try {
        json j;
        file >> j;
        records = j.get<std::vector<Record>>();
        sortRecords(records);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки рекордов: " << e.what() << std::endl;
        return false;
    }
}

bool saveRecords(const std::vector<Record>& records, const std::string& filename) {
    json j = records;  
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл для записи рекордов!" << std::endl;
        return false;
    }
    file << j.dump(4);
    return true;
}

void addRecord(std::vector<Record>& records, const std::string& name, int prize, int seconds) {
    records.push_back({name, prize, seconds});
    sortRecords(records);
    saveRecords(records);
}

void sortRecords(std::vector<Record>& records) {
    std::sort(records.begin(), records.end(),
              [](const Record& a, const Record& b) {
                  if (a.prize != b.prize)
                      return a.prize > b.prize;      
                  return a.seconds < b.seconds;      
              });
}