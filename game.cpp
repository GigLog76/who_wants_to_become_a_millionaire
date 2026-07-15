#include "game.h"
#include "Question.h"
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "downloads.h"

int Prize = 0;
int Stage = 0;
bool Lose = false;
bool Exit = false;

std::array<int, 16> prizeTable = {
    0, 500, 1000, 2000, 3000, 5000, 10000, 25000, 50000, 75000,
    100000, 200000, 400000, 800000, 1500000, 3000000
};

sf::String toSFString(const std::string& str) {
    return sf::String::fromUtf8(str.begin(), str.end());
}

void GameProcess() {

    sf::Font font;
    if (!loadFont(font)) {
        return;
    }
    
    sf::Image icon;
    loadIcon(icon);
  

    sf::RenderWindow window(sf::VideoMode({800, 650}), "Who wants to become a millionaire");
    window.setFramerateLimit(60);

    window.setIcon(icon);

    sf::Music backgroundMusic;
    if (!loadBackgroundMusic(backgroundMusic)) {
        std::cout << "Не удалось загрузить фоновую музыку!" << std::endl;
    } else {
        backgroundMusic.play();
    }
    //НАЧАЛЬНЫЙ ЭКРАН
    bool startGame = false;
    bool exitGame = false;
    bool setting = false;
    bool rules = false;

    sf::Text titleText(font, "", 48);
    titleText.setString(toSFString("Кто хочет стать миллионером?"));
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({100.f, 100.f});

    sf::RectangleShape startButton(sf::Vector2f(200.f, 60.f));
    startButton.setFillColor(sf::Color(50, 150, 50));
    startButton.setOutlineThickness(3.f);
    startButton.setOutlineColor(sf::Color::White);
    startButton.setPosition({300.f, 250.f});

    sf::Text startText(font, "", 30);
    startText.setString(toSFString("Начать игру"));
    startText.setFillColor(sf::Color::White);
    startText.setPosition({320.f, 260.f});

    sf::RectangleShape rulesButton(sf::Vector2f(200.f, 60.f));
    rulesButton.setFillColor(sf::Color(0, 0, 255));
    rulesButton.setOutlineThickness(3.f);
    rulesButton.setOutlineColor(sf::Color::White);
    rulesButton.setPosition({300.f, 350.f});

    sf::Text rulesText(font, "", 30);
    rulesText.setString(toSFString("Правила"));
    rulesText.setFillColor(sf::Color::White);
    rulesText.setPosition({340.f, 360.f});

    sf::RectangleShape settingButton(sf::Vector2f(200.f, 60.f));
    settingButton.setFillColor(sf::Color(100, 100, 100));
    settingButton.setOutlineThickness(3.f);
    settingButton.setOutlineColor(sf::Color::White);
    settingButton.setPosition({300.f, 450.f});

    sf::Text settingText(font, "", 30);
    settingText.setString(toSFString("Настройки"));
    settingText.setFillColor(sf::Color::White);
    settingText.setPosition({330.f, 460.f});

    sf::RectangleShape exitButtonMenu(sf::Vector2f(200.f, 60.f));
    exitButtonMenu.setFillColor(sf::Color(150, 50, 50));
    exitButtonMenu.setOutlineThickness(3.f);
    exitButtonMenu.setOutlineColor(sf::Color::White);
    exitButtonMenu.setPosition({300.f, 550.f});

    sf::Text exitTextMenu(font, "", 30);
    exitTextMenu.setString(toSFString("Выход"));
    exitTextMenu.setFillColor(sf::Color::White);
    exitTextMenu.setPosition({350.f, 560.f});



sf::Text rulesTitleText(font, "", 30);
rulesTitleText.setString(toSFString("Правила игры:\n1. Отвечайте на вопросы.\n2. Выбирайте вариант.\n3. ..."));
rulesTitleText.setFillColor(sf::Color::White);
rulesTitleText.setPosition({100.f, 100.f});

sf::RectangleShape backButton(sf::Vector2f(150.f, 50.f));
backButton.setFillColor(sf::Color(100, 100, 100));
backButton.setOutlineThickness(2.f);
backButton.setOutlineColor(sf::Color::White);
backButton.setPosition({300.f, 500.f});

sf::Text backText(font, "", 25);
backText.setString(toSFString("Назад"));
backText.setFillColor(sf::Color::White);
backText.setPosition({340.f, 510.f});


while (window.isOpen()) {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse->button == sf::Mouse::Button::Left) {
                sf::Vector2f worldPos = window.mapPixelToCoords(mouse->position);

                
                if (!startGame && !exitGame && !rules && !setting) {
                    if (startButton.getGlobalBounds().contains(worldPos)) {
                        startGame = true;
                    } else if (rulesButton.getGlobalBounds().contains(worldPos)) {
                        rules = true;
                    } else if (settingButton.getGlobalBounds().contains(worldPos)) {
                        setting = true;
                    } else if (exitButtonMenu.getGlobalBounds().contains(worldPos)) {
                        exitGame = true;
                        window.close();
                    }
                }
                else if (rules) {
                    if (backButton.getGlobalBounds().contains(worldPos)) {
                        rules = false;  
                    }
                }
                else if (setting) {
                    if (backButton.getGlobalBounds().contains(worldPos)) {
                        setting = false; 
                    }
                }
                else if (startGame) {
                    
                    
                }
            }
        }
    }

    window.clear(sf::Color(30, 30, 60));

    if (!startGame && !exitGame && !rules && !setting) {
        //МЕНЮ
        window.draw(titleText);
        window.draw(startButton);
        window.draw(startText);
        window.draw(rulesButton);
        window.draw(rulesText);
        window.draw(settingButton);
        window.draw(settingText);
        window.draw(exitButtonMenu);
        window.draw(exitTextMenu);
    }
    else if (rules) {
        window.draw(rulesTitleText);
        window.draw(backButton);
        window.draw(backText);
    }
    else if (setting) {
        sf::Text settingsText(font, "", 30);
        settingsText.setString(toSFString("Настройки (скоро будут)"));
        settingsText.setFillColor(sf::Color::White);
        settingsText.setPosition({100.f, 100.f});
        window.draw(settingsText);
        window.draw(backButton);
        window.draw(backText);
    }
    else if (startGame) {
        break;
    }
    window.display();
}


    //ИГРОВОЙ ПРОЦЕСС
    backgroundMusic.stop();
    sf::Music questionMusic;
    if (!loadQuestionMusic(questionMusic)) {
        std::cout << "Не удалось загрузить музыку вопроса!" << std::endl;
    }
    
    sf::Music loseMusic;
    if (!loadLoseMusic(loseMusic)) {
        std::cout << "Не удалось загрузить музыку поражения!" << std::endl;
    }

    sf::Music quitMusic;
    if (!loadQuitMusic(quitMusic)) {
        std::cout << "Не удалось загрузить музыку выхода!" << std::endl;
    } 

    sf::Music winMusic;
    if (!loadWinMusic(winMusic)) {
        std::cout << "Не удалось загрузить музыку победы!" << std::endl;
    }

    sf::Text questionText(font, "", 28);
    questionText.setPosition({50.f, 50.f});
    questionText.setFillColor(sf::Color::White);

    sf::Text prizeText(font, "", 24);
    prizeText.setPosition({50.f, 600.f});
    prizeText.setFillColor(sf::Color::Yellow);

    sf::Text stageText(font, "", 20);
    stageText.setPosition({650.f, 600.f});
    stageText.setFillColor(sf::Color::Cyan);

    sf::Text optionTexts[4] = {
        sf::Text(font, "", 22),
        sf::Text(font, "", 22),
        sf::Text(font, "", 22),
        sf::Text(font, "", 22)
    };

    sf::RectangleShape optionButtons[4];
    for (int i = 0; i < 4; i++) {
        optionButtons[i].setSize({300.f, 50.f});
        optionButtons[i].setFillColor(sf::Color(100, 100, 100, 200));
        optionButtons[i].setOutlineThickness(2.f);
        optionButtons[i].setOutlineColor(sf::Color::White);
        optionButtons[i].setPosition({50.f, 150.f + i * 80.f});

        optionTexts[i].setPosition({70.f, 155.f + i * 80.f});
        optionTexts[i].setFillColor(sf::Color::White);
    }

    sf::RectangleShape exitButton({100.f, 40.f});
    exitButton.setFillColor(sf::Color(200, 0, 0, 200));
    exitButton.setPosition({700.f, 10.f});

    sf::Text exitText(font, "Exit", 20);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition({720.f, 15.f});

    std::random_device rd;
    std::mt19937 gen(rd());

    Question currentQuestion;
    bool needNewQuestion = true;

    while (window.isOpen() && Stage < 15 && !Lose && !Exit) {
        

        if (needNewQuestion) {
            questionMusic.play();
            int currentLevel = Stage / 3;          
            if (currentLevel >= 5) break;          

            int start = currentLevel * 25;         
            int end = start + 24;                  

            std::uniform_int_distribution<int> dist(start, end);
            int randomIndex = dist(gen);
            currentQuestion = questions[randomIndex];
            needNewQuestion = false;
        }

        questionText.setString(toSFString(currentQuestion.question));
        for (int i = 0; i < 4; i++) {
            std::string label = std::string(1, char('A' + i)) + ") " + currentQuestion.options[i];
            optionTexts[i].setString(toSFString(label));
        }

        prizeText.setString(toSFString("Выигрыш: " + std::to_string(Prize) + " руб."));
        stageText.setString(toSFString("Вопрос " + std::to_string(Stage + 1) + "/15"));

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Q) {
                    std::cout << "Игрок забрал " << Prize << " руб. (выход)" << std::endl;
                    window.close();
                }
            }

            if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2f worldPos = window.mapPixelToCoords(mouse->position);

                    for (int i = 0; i < 4; i++) {
                        if (optionButtons[i].getGlobalBounds().contains(worldPos)) {
                            char choice = char('A' + i);
                            if (currentQuestion.isCorrect(choice)) {
                                Stage++;
                                Prize = prizeTable[Stage];
                                std::cout << "Правильно! Этап: " << Stage << std::endl;
                                needNewQuestion = true;
                            } else {
                                Lose = true;
                                if (Stage >= 10) Prize = 100000;
                                else if (Stage >= 5) Prize = 5000;
                                else Prize = 0;
                                std::cout << "Неправильно!" << std::endl;
                            }
                            break;
                        }
                    }

                    if (exitButton.getGlobalBounds().contains(worldPos)) {
                        std::cout << "Игрок забрал " << Prize << " руб." << std::endl;
                        Exit = true;
                        break;
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 60));
        window.draw(questionText);
        window.draw(prizeText);
        window.draw(stageText);
        for (int i = 0; i < 4; i++) {
            window.draw(optionButtons[i]);
            window.draw(optionTexts[i]);
        }
        window.draw(exitButton);
        window.draw(exitText);
        window.display();
    }

    //ФИНАЛЬНЫЙ ЭКРАН
    questionMusic.stop();
    window.clear(sf::Color(30, 30, 60));

    sf::Text endText(font, "", 40);
    if (Lose) {
        endText.setString(toSFString("Вы проиграли!"));
        endText.setFillColor(sf::Color::Red);
        loseMusic.play();
    }
    else if (Exit){
        endText.setString(toSFString("Удачи в следующий раз!"));
        endText.setFillColor(sf::Color::Green);
        quitMusic.play();
    }
    else {
        endText.setString(toSFString("ПОБЕДА! Вы миллионер!"));
        endText.setFillColor(sf::Color::Green);
        winMusic.play();
    }
    endText.setPosition({200.f, 200.f});

    sf::Text finalPrize(font, "", 30);
    finalPrize.setString(toSFString("Ваш выигрыш: " + std::to_string(Prize) + " руб."));
    finalPrize.setFillColor(sf::Color::Yellow);
    finalPrize.setPosition({200.f, 300.f});

    window.draw(endText);
    window.draw(finalPrize);
    window.display();

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }
    }
}