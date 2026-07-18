#include "game.h"
#include "Question.h"
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "downloads.h"
#include "hints.h"
#include <chrono>
#include "records.h"


auto startTime = std::chrono::steady_clock::now();

int Prize = 0;
int Stage = 0;
bool Lose = false;
bool Exit = false;
HintState hints;
std::vector<Record> records;




std::array<int, 16> prizeTable = {
    0, 500, 1000, 2000, 3000, 5000, 10000, 25000, 50000, 75000,
    100000, 200000, 400000, 800000, 1500000, 3000000
};

sf::String toSFString(const std::string& str) {
    return sf::String::fromUtf8(str.begin(), str.end());
}

int applyHalfHint(const Question& question, sf::Text optionTexts[4], HintState& hints) {
    if (hints.halfUsed) return 4; 

    std::vector<int> wrongs;
    for (int i = 0; i < 4; ++i) {
        if (i != question.correct) {
            wrongs.push_back(i);
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(wrongs.begin(), wrongs.end(), g);

    for (int i = 0; i < 2 && i < wrongs.size(); ++i) {
        hints.hidden[wrongs[i]] = true;
    }
    hints.halfUsed = true;
    return 2; 
}



void GameProcess() {
    sf::Font font;
    if (!loadFont(font)) return;
    
    sf::Image icon;
    loadIcon(icon);

    sf::RenderWindow window(sf::VideoMode({800, 650}), "Who wants to become a millionaire");
    window.setFramerateLimit(60);
    window.setIcon(icon);
    loadRecords(records); 
    sf::Music backgroundMusic;
    if (!loadBackgroundMusic(backgroundMusic)) {
        std::cout << "Не удалось загрузить фоновую музыку!" << std::endl;
    } else {
        backgroundMusic.play();
    }

    //НАЧАЛЬНЫЙ ЭКРАН
    bool startGame = false, exitGame = false, setting = false, rules = false,showRecords = false;

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

    sf::RectangleShape rulesButton(sf::Vector2f(330.f, 60.f));
    rulesButton.setFillColor(sf::Color(0, 0, 255));
    rulesButton.setOutlineThickness(3.f);
    rulesButton.setOutlineColor(sf::Color::White);
    rulesButton.setPosition({230.f, 350.f});

    sf::Text rulesText(font, "", 30);
    rulesText.setString(toSFString("Добавить свой вопрос"));
    rulesText.setFillColor(sf::Color::White);
    rulesText.setPosition({240.f, 360.f});

    sf::RectangleShape recordsButton(sf::Vector2f(200.f, 60.f));
    recordsButton.setFillColor(sf::Color(100, 100, 100));
    recordsButton.setOutlineThickness(3.f);
    recordsButton.setOutlineColor(sf::Color::White);
    recordsButton.setPosition({300.f, 450.f});

    sf::Text recordsText(font, "", 30);
    recordsText.setString(toSFString("Рекорды"));
    recordsText.setFillColor(sf::Color::White);
    recordsText.setPosition({340.f, 460.f});

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
    rulesTitleText.setString(toSFString("Скоро..."));
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
            if (event->is<sf::Event::Closed>()) window.close();

            if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2f worldPos = window.mapPixelToCoords(mouse->position);

                    if (!startGame && !exitGame && !rules && !setting && !showRecords) {
                        if (startButton.getGlobalBounds().contains(worldPos)) startGame = true;
                        else if (rulesButton.getGlobalBounds().contains(worldPos)) rules = true;
                        else if (recordsButton.getGlobalBounds().contains(worldPos)) showRecords = true;
                        else if (exitButtonMenu.getGlobalBounds().contains(worldPos)) {
                            exitGame = true;
                            window.close();
                        }
                    }
                    else if (rules) {
                        if (backButton.getGlobalBounds().contains(worldPos)) rules = false;
                    }
                    else if (setting) {
                        if (backButton.getGlobalBounds().contains(worldPos)) setting = false;
                    }
                    else if (showRecords) {
                        if (backButton.getGlobalBounds().contains(worldPos)) showRecords = false;
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 60));

if (!startGame && !exitGame && !rules && !setting && !showRecords) {
    window.draw(titleText);
    window.draw(startButton);
    window.draw(startText);
    window.draw(rulesButton);
    window.draw(rulesText);
    window.draw(recordsButton);
    window.draw(recordsText);
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
            settingsText.setString(toSFString("Скоро...)"));
            settingsText.setFillColor(sf::Color::White);
            settingsText.setPosition({100.f, 100.f});
            window.draw(settingsText);
            window.draw(backButton);
            window.draw(backText);
        }
        else if (showRecords) {
    sf::Text title(font, toSFString("Таблица рекордов"), 30);
    title.setPosition({200.f, 50.f});   
    title.setFillColor(sf::Color::Yellow);
    window.draw(title);

    sf::Text header(font, toSFString("Имя          Выигрыш     Время"), 20);
    header.setPosition({100.f, 120.f});
    header.setFillColor(sf::Color::White);
    window.draw(header);

    float y = 170.f;
    for (size_t i = 0; i < records.size() && i < 10; ++i) {
        const auto& rec = records[i];
        std::string line = rec.playerName + "          " + std::to_string(rec.prize) + " руб.     " + std::to_string(rec.seconds) + " с";
        sf::Text recordText(font, toSFString(line), 18);
        recordText.setPosition({100.f, y});
        recordText.setFillColor(sf::Color::White);
        window.draw(recordText);
        y += 30.f;
    }

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
    if (!loadQuestionMusic(questionMusic)) std::cout << "Не удалось загрузить музыку вопроса!" << std::endl;
    sf::Music loseMusic;
    if (!loadLoseMusic(loseMusic)) std::cout << "Не удалось загрузить музыку поражения!" << std::endl;
    sf::Music quitMusic;
    if (!loadQuitMusic(quitMusic)) std::cout << "Не удалось загрузить музыку выхода!" << std::endl;
    sf::Music winMusic;
    if (!loadWinMusic(winMusic)) std::cout << "Не удалось загрузить музыку победы!" << std::endl;

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

    sf::Text audienceTexts[4] = {
    sf::Text(font, "", 18),
    sf::Text(font, "", 18),
    sf::Text(font, "", 18),
    sf::Text(font, "", 18)
};
for (int i = 0; i < 4; ++i) {
    audienceTexts[i].setFillColor(sf::Color::Yellow);
    audienceTexts[i].setPosition({370.f, 155.f + i * 80.f}); 
}

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

    sf::RectangleShape halfButton({150.f, 40.f});
    halfButton.setFillColor(sf::Color(20, 0, 0, 200));
    halfButton.setPosition({650.f, 70.f});

    sf::Text halfText(font, "50/50", 20);
    halfText.setFillColor(sf::Color::White);
    halfText.setPosition({670.f, 75.f});

    sf::RectangleShape audienceButton({150.f, 40.f});
    audienceButton.setFillColor(sf::Color(20, 0, 0, 200));
    audienceButton.setPosition({650.f, 140.f});

    sf::Text audienceText(font, toSFString("Помощь зала"), 20);
    audienceText.setFillColor(sf::Color::White);
    audienceText.setPosition({670.f, 145.f});

    sf::RectangleShape secondChanceButton({150.f, 40.f});
    secondChanceButton.setFillColor(sf::Color(20, 0, 0, 200));
    secondChanceButton.setPosition({650.f, 210.f});

    sf::Text secondChanceText(font, toSFString("Второй шанс"), 20);
    secondChanceText.setFillColor(sf::Color::White);
    secondChanceText.setPosition({670.f, 215.f});

    std::random_device rd;
    std::mt19937 gen(rd());

    Question currentQuestion;
    bool needNewQuestion = true;
    hints.halfUsed = false;
    hints.secondChanceUsed = false;
    hints.audienceUsed = false;
    while (window.isOpen() && Stage < 15 && !Lose && !Exit) {
        if (needNewQuestion) {
            questionMusic.play();

            hints.audienceVotes = {0, 0, 0, 0};
            for (int i = 0; i < 4; ++i) {
                audienceTexts[i].setString("");
            }
        if (!hints.audienceUsed) {
            audienceText.setString(toSFString("Помощь зала"));
            audienceButton.setFillColor(sf::Color(20, 0, 0, 200));
        } else {
            audienceText.setString(toSFString("Помощь зала"));
            audienceButton.setFillColor(sf::Color(100, 100, 100));
        }

            hints.showAudience = false;   
            hints.audienceVotes = {0, 0, 0, 0};
            for (int i = 0; i < 4; ++i) {
                audienceTexts[i].setString("");
            }

            int currentLevel = Stage / 3;
            if (currentLevel >= 5) break;

            int start = currentLevel * 25;
            int end = start + 24;

            std::uniform_int_distribution<int> dist(start, end);
            int randomIndex = dist(gen);
            currentQuestion = questions[randomIndex];
            needNewQuestion = false;



            hints.hidden = {false, false, false, false};
            hints.wrongGuesses = {false, false, false, false};

       /*     halfText.setString("50/50");
            halfButton.setFillColor(sf::Color(20, 0, 0, 200));
            secondChanceText.setString(toSFString("Второй шанс"));
            secondChanceButton.setFillColor(sf::Color(20, 0, 0, 200));*/

            for (int i = 0; i < 4; i++) {
                optionButtons[i].setFillColor(sf::Color(100, 100, 100, 200));
            }
        }

        questionText.setString(toSFString(currentQuestion.question));

        for (int i = 0; i < 4; i++) {
            std::string label = std::string(1, char('A' + i)) + ") " + currentQuestion.options[i];
            optionTexts[i].setString(toSFString(label));
        }

        if (hints.halfUsed) {
            for (int i = 0; i < 4; ++i) {
                if (hints.hidden[i]) {
                    optionTexts[i].setString("");
                }
            }
        }

        prizeText.setString(toSFString("Выигрыш: " + std::to_string(Prize) + " руб."));
        stageText.setString(toSFString("Вопрос " + std::to_string(Stage + 1) + "/15"));

                if (hints.showAudience) {
                    for (int i = 0; i < 4; ++i) {
                        if (hints.hidden[i]) {
                            audienceTexts[i].setString("");
                        } else {
                            audienceTexts[i].setString(toSFString(std::to_string(hints.audienceVotes[i]) + "%"));
                        }
                    }
                } else {
                    for (int i = 0; i < 4; ++i) {
                        audienceTexts[i].setString("");
                    }
                }

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
                        if (optionTexts[i].getString().isEmpty() || hints.wrongGuesses[i]) {
                            break; 
                        }

                        char choice = char('A' + i);
                        if (currentQuestion.isCorrect(choice)) {
                            Stage++;
                            Prize = prizeTable[Stage];
                            std::cout << "Правильно! Этап: " << Stage << std::endl;
                            needNewQuestion = true;
                        } else {
                            if (hints.secondChanceUsed) {
                                hints.secondChanceUsed = false;   
                                hints.wrongGuesses[i] = true;     
                                optionButtons[i].setFillColor(sf::Color(200, 0, 0, 150));
                                std::cout << "Использован второй шанс! Попробуйте другой вариант." << std::endl;
                            } else {
                                Lose = true;
                                if (Stage >= 10) Prize = 100000;
                                else if (Stage >= 5) Prize = 5000;
                                else Prize = 0;
                                std::cout << "Неправильно!" << std::endl;
                            }
                        }
                        break; 
                    }   

                    }

                    if (exitButton.getGlobalBounds().contains(worldPos)) {
                        std::cout << "Игрок забрал " << Prize << " руб." << std::endl;
                        Exit = true;
                        break;
                    }

                    if (halfButton.getGlobalBounds().contains(worldPos)) {
                        if (!hints.halfUsed) {
                            applyHalfHint(currentQuestion, optionTexts, hints);
                            halfText.setString("50/50");
                            halfButton.setFillColor(sf::Color(100, 100, 100));
                        }
                    }
                    if (audienceButton.getGlobalBounds().contains(worldPos)) {
                        if (!hints.audienceUsed) {
                            generateAudienceVotes(currentQuestion, hints.audienceVotes);
                            hints.audienceUsed = true;
                            hints.showAudience = true; 
                            audienceText.setString(toSFString("Помощь зала"));
                            audienceButton.setFillColor(sf::Color(100, 100, 100));
                        }
                    }
                    if (secondChanceButton.getGlobalBounds().contains(worldPos)) {
                        if (!hints.secondChanceUsed) {
                            hints.secondChanceUsed = true;
                            secondChanceText.setString(toSFString("Второй шанс"));
                            secondChanceButton.setFillColor(sf::Color(100, 100, 100));
                        }
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
            window.draw(audienceTexts[i]);
        }
        window.draw(exitButton);
        window.draw(exitText);
        window.draw(halfButton);
        window.draw(halfText);
        window.draw(audienceButton);
        window.draw(audienceText);
        window.draw(secondChanceButton);
        window.draw(secondChanceText);
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
    else if (Exit) {
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

    auto endTime = std::chrono::steady_clock::now();
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    if (Prize > 0) {
    std::string playerName = "Игрок"; //ЗАГЛУШКА
    addRecord(records, playerName, Prize, seconds);
}

    window.draw(endText);
    window.draw(finalPrize);
    window.display();

    


    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }
    }
}