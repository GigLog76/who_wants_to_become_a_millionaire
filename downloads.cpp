#include "downloads.h"
#include <iostream>

bool loadFont(sf::Font& font) {
    if (!font.openFromFile("fonts/arial.ttf")) {
        std::cout << "Шрифт не загружен!" << std::endl;
        return false;
    }
    return true;
}

bool loadIcon(sf::Image& icon) {
    if (!icon.loadFromFile("image/icon.png")) {
        std::cout << "Иконка не загружена!" << std::endl;
        return false;
    }
    return true;
}

bool loadButtonTexture(sf::Texture& texture) {
    if (!texture.loadFromFile("images/button.png")) {
        std::cout << "Текстура кнопки не загружена!" << std::endl;
        return false;
    }
    return true;
}

bool loadBackgroundMusic(sf::Music& music) {
    if (!music.openFromFile("audio/music.mp3")) {
        std::cout << "Фоновая музыка не загружена!" << std::endl;
        return false;
    }
    music.setVolume(50.f);
    music.setLooping(true);
    return true;
}

bool loadQuestionMusic(sf::Music& music) {
    if (!music.openFromFile("audio/question.mp3")) {
        std::cout << "Музыка вопроса не загружена!" << std::endl;
        return false;
    }
    music.setVolume(50.f);
    music.setLooping(true);
    return true;
}

bool loadLoseMusic(sf::Music& music) {
    if (!music.openFromFile("audio/loseMusic.mp3")) {
        std::cout << "Музыка проигрыша не загружена!" << std::endl;
        return false;
    }
    music.setVolume(50.f);
    music.setLooping(true);
    return true;
}

bool loadQuitMusic(sf::Music& music) {
    if (!music.openFromFile("audio/quitMusic.mp3")) {
        std::cout << "Музыка выхода не загружена!" << std::endl;
        return false;
    }
    music.setVolume(50.f);
    music.setLooping(true);
    return true;
}

bool loadWinMusic(sf::Music& music) {
    if (!music.openFromFile("audio/winMusic.mp3")) {
        std::cout << "Музыка победы не загружена!" << std::endl;
        return false;
    }
    music.setVolume(50.f);
    music.setLooping(true);
    return true;
}

bool loadWaitingMusic(sf::Music& music) {
    if (!music.openFromFile("audio/waitingMusic.mp3")) {
        std::cout << "Музыка ожидания не загружена!" << std::endl;
        return false;
    }
    music.setVolume(50.f);
    music.setLooping(true);
    return true;
}