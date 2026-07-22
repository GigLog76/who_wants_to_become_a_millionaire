#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

bool loadFont(sf::Font& font);
bool loadIcon(sf::Image& icon);
bool loadButtonTexture(sf::Texture& texture);

bool loadBackgroundMusic(sf::Music& music);
bool loadQuestionMusic(sf::Music& music);
bool loadLoseMusic(sf::Music& music);
bool loadQuitMusic(sf::Music& music);
bool loadWinMusic(sf::Music& music);
bool loadWaitingMusic(sf::Music& music);