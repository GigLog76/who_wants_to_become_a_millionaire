#include "Hints.h"
#include <random>
#include <algorithm>


std::array<int, 4> getAudienceVotes(const Question& question, int seed) {
    std::array<int, 4> votes = {0, 0, 0, 0};
    std::mt19937 gen(seed != 0 ? seed : std::random_device{}());
    std::uniform_int_distribution<int> dist(10, 40);

    int correctVotes = 60 + dist(gen) % 20; 
    int remaining = 100 - correctVotes;

    for (int i = 0; i < 4; ++i) {
        if (i == question.correct) votes[i] = correctVotes;
        else {
            int part = 0;
            if (remaining > 0) {
                part = dist(gen) % (remaining / 2 + 1);
                if (part > remaining) part = remaining;
                remaining -= part;
            }
            votes[i] = part;
        }
    }
    return votes;
}

void generateAudienceVotes(const Question& question, std::array<int, 4>& votes) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distCorrect(60, 80);
    int correctPercent = distCorrect(gen);

    int remaining = 100 - correctPercent;
    std::array<int, 4> wrongIndices;
    int idx = 0;
    for (int i = 0; i < 4; ++i) {
        if (i != question.correct) wrongIndices[idx++] = i;
    }
    std::shuffle(wrongIndices.begin(), wrongIndices.begin() + 3, gen);

    std::uniform_int_distribution<int> distWrong(0, remaining);
    int used = 0;
    for (int i = 0; i < 3; ++i) {
        int v = (i == 2) ? remaining - used : distWrong(gen) % (remaining - used + 1);
        votes[wrongIndices[i]] = v;
        used += v;
    }
    votes[question.correct] = correctPercent;
}

bool applySecondChance(bool& usedFlag) {
    if (usedFlag) return false; 
    usedFlag = true;
    return true; 
}
