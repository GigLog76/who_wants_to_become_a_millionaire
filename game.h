#pragma once

#include <string>
#include <vector>
#include <array>
#include "json.hpp"
#include "Question.h"
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "downloads.h"
#include "hints.h"
#include <chrono>
#include "records.h"
#include "InputField.h"
#include "QuestionManager.h"
extern std::vector<Question> questions; 

void GameProcess();