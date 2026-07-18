#pragma once
#include <SFML/Graphics.hpp>
#include "Question.h"

struct HintState {
    bool halfUsed = false;      
    bool audienceUsed = false;  
    bool secondChanceUsed = false;
    bool showAudience = false;  
    std::array<bool, 4> hidden = {false, false, false, false};
    std::array<bool, 4> wrongGuesses = {false, false, false, false};
    std::array<int, 4> audienceVotes = {0, 0, 0, 0};
};

/*int applyHalfHint(const Question& question, sf::Text optionTexts[4], bool& usedFlag);*/

int applyHalfHint(const Question& question, sf::Text optionTexts[4], HintState& hints);

std::array<int, 4> getAudienceVotes(const Question& question, int seed = 0);

bool applySecondChance(bool& usedFlag);

void generateAudienceVotes(const Question& question, std::array<int, 4>& votes);