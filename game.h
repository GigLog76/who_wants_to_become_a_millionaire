#pragma once


#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <random>
#include "Question.h"
#include "json.hpp"
#include "hints.h"
extern std::vector<Question> questions; 

void GameProcess();