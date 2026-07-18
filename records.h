#pragma once
#include <string>
#include <vector>
#include "json.hpp"   

struct Record {
    std::string playerName;
    int prize;
    int seconds;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Record, playerName, prize, seconds)

bool loadRecords(std::vector<Record>& records, const std::string& filename = "records.json");
bool saveRecords(const std::vector<Record>& records, const std::string& filename = "records.json");
void addRecord(std::vector<Record>& records, const std::string& name, int prize, int seconds);
void sortRecords(std::vector<Record>& records);