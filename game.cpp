#include "game.h"


auto startTime = std::chrono::steady_clock::now();

std::vector<Question> questions;
int Prize = 0;
int Stage = 0;
bool Lose = false;
bool Exit = false;
std::string selectedCategory = "Все";
std::vector<int> usedQuestionIds;
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
    loadQuestions(questions, "questions.json");

    std::vector<std::string> categories;
    for (const auto& q : questions) {
        if (std::find(categories.begin(), categories.end(), q.category) == categories.end()) {
            categories.push_back(q.category);
        }
    }
    categories.insert(categories.begin(), "Все");

    usedQuestionIds.clear();

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

    std::vector<InputField> fields;
    bool formInitialized = false;
    bool buttonsCreated = false;
    sf::RectangleShape saveButton;
    sf::RectangleShape cancelButton;
    std::unique_ptr<sf::Text> saveText;
    std::unique_ptr<sf::Text> cancelText;

    bool startGame = false, exitGame = false, setting = false, rules = false, showRecords = false, manageQuestions = false, addingQuestion = false;
    bool selectingCategory = false;
    bool categoryButtonsCreated = false;
    std::vector<sf::RectangleShape> categoryButtons;
    std::vector<sf::Text> categoryTexts;

    bool isWaiting = false;
    sf::Clock waitClock;
    const float waitDuration = 3.0f;
    int selectedOption = -1;
    bool answerResult = false;

    sf::Text titleText(font, "", 48);
    titleText.setString(toSFString("Кто хочет стать миллионером?"));
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition(sf::Vector2f(100.f, 100.f));

    sf::RectangleShape startButton(sf::Vector2f(200.f, 60.f));
    startButton.setFillColor(sf::Color(50, 150, 50));
    startButton.setOutlineThickness(3.f);
    startButton.setOutlineColor(sf::Color::White);
    startButton.setPosition(sf::Vector2f(300.f, 250.f));

    sf::Text startText(font, "", 30);
    startText.setString(toSFString("Начать игру"));
    startText.setFillColor(sf::Color::White);
    startText.setPosition(sf::Vector2f(320.f, 260.f));

    sf::RectangleShape manageQuestionsButton(sf::Vector2f(330.f, 60.f));
    manageQuestionsButton.setFillColor(sf::Color(0, 0, 255));
    manageQuestionsButton.setOutlineThickness(3.f);
    manageQuestionsButton.setOutlineColor(sf::Color::White);
    manageQuestionsButton.setPosition(sf::Vector2f(230.f, 350.f));

    sf::Text manageQuestionsText(font, "", 30);
    manageQuestionsText.setString(toSFString("Добавить свой вопрос"));
    manageQuestionsText.setFillColor(sf::Color::White);
    manageQuestionsText.setPosition(sf::Vector2f(240.f, 360.f));

    sf::RectangleShape recordsButton(sf::Vector2f(200.f, 60.f));
    recordsButton.setFillColor(sf::Color(100, 100, 100));
    recordsButton.setOutlineThickness(3.f);
    recordsButton.setOutlineColor(sf::Color::White);
    recordsButton.setPosition(sf::Vector2f(300.f, 450.f));

    sf::Text recordsText(font, "", 30);
    recordsText.setString(toSFString("Рекорды"));
    recordsText.setFillColor(sf::Color::White);
    recordsText.setPosition(sf::Vector2f(340.f, 460.f));

    sf::RectangleShape exitButtonMenu(sf::Vector2f(200.f, 60.f));
    exitButtonMenu.setFillColor(sf::Color(150, 50, 50));
    exitButtonMenu.setOutlineThickness(3.f);
    exitButtonMenu.setOutlineColor(sf::Color::White);
    exitButtonMenu.setPosition(sf::Vector2f(300.f, 550.f));

    sf::Text exitTextMenu(font, "", 30);
    exitTextMenu.setString(toSFString("Выход"));
    exitTextMenu.setFillColor(sf::Color::White);
    exitTextMenu.setPosition(sf::Vector2f(350.f, 560.f));

    sf::Text rulesTitleText(font, "", 30);
    rulesTitleText.setString(toSFString("Скоро..."));
    rulesTitleText.setFillColor(sf::Color::White);
    rulesTitleText.setPosition(sf::Vector2f(100.f, 100.f));

    sf::RectangleShape backButton(sf::Vector2f(150.f, 50.f));
    backButton.setFillColor(sf::Color(100, 100, 100));
    backButton.setOutlineThickness(2.f);
    backButton.setOutlineColor(sf::Color::White);
    backButton.setPosition(sf::Vector2f(300.f, 550.f));

    sf::Text backText(font, "", 25);
    backText.setString(toSFString("Назад"));
    backText.setFillColor(sf::Color::White);
    backText.setPosition(sf::Vector2f(340.f, 560.f));
    //МЕНЮ
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (addingQuestion) {
                for (auto& field : fields) {
                    field.handleEvent(*event);
                }

                if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos(static_cast<float>(mouse->position.x),
                                              static_cast<float>(mouse->position.y));

                        for (auto& field : fields) {
                            if (field.getBounds().contains(mousePos)) {
                                field.setActive(true);
                            } else {
                                field.setActive(false);
                            }
                        }

                        if (saveButton.getGlobalBounds().contains(mousePos)) {
                            std::string category = fields[0].getText();
                            std::string levelStr = fields[1].getText();
                            std::string questionText = fields[2].getText();
                            std::array<std::string, 4> options = {
                                fields[3].getText(),
                                fields[4].getText(),
                                fields[5].getText(),
                                fields[6].getText()
                            };
                            std::string correctStr = fields[7].getText();

                            if (category.empty() || levelStr.empty() || questionText.empty() ||
                                options[0].empty() || options[1].empty() || options[2].empty() || options[3].empty() ||
                                correctStr.empty()) {
                                std::cout << "Все поля должны быть заполнены!" << std::endl;
                            } else {
                                try {
                                    int level = std::stoi(levelStr);
                                    int correct = std::stoi(correctStr) - 1;
                                    if (level < 1 || level > 5 || correct < 0 || correct > 3) {
                                        std::cout << "Уровень должен быть 1-5, правильный ответ 1-4!" << std::endl;
                                    } else {
                                        std::vector<Question> allQuestions;
                                        if (!loadQuestions(allQuestions, "questions.json")) {
                                            std::cout << "Не удалось загрузить вопросы, создаётся новый файл." << std::endl;
                                        }
                                        int newId = allQuestions.empty() ? 1 : allQuestions.back().id + 1;
                                        Question newQ{newId, level, category, questionText, options, correct};
                                        allQuestions.push_back(newQ);
                                        if (saveQuestions(allQuestions, "questions.json")) {
                                            std::cout << "Вопрос успешно добавлен!" << std::endl;
                                            ::questions = allQuestions;
                                            addingQuestion = false;
                                            formInitialized = false;
                                            buttonsCreated = false;
                                            for (auto& field : fields) {
                                                field.setText("");
                                                field.setActive(false);
                                            }
                                            categoryButtonsCreated = false;
                                        } else {
                                            std::cout << "Ошибка сохранения!" << std::endl;
                                        }
                                    }
                                } catch (const std::exception& e) {
                                    std::cout << "Ошибка ввода числа: " << e.what() << std::endl;
                                }
                            }
                        }

                        if (cancelButton.getGlobalBounds().contains(mousePos)) {
                            addingQuestion = false;
                            formInitialized = false;
                            buttonsCreated = false;
                            for (auto& field : fields) {
                                field.setText("");
                                field.setActive(false);
                            }
                        }
                    }
                }
            }
            else if (selectingCategory) {
                if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos(static_cast<float>(mouse->position.x),
                                              static_cast<float>(mouse->position.y));
                        for (size_t i = 0; i < categoryButtons.size(); ++i) {
                            if (categoryButtons[i].getGlobalBounds().contains(mousePos)) {
                                selectedCategory = categories[i];
                                usedQuestionIds.clear();
                                selectingCategory = false;
                                startGame = true;
                                break;
                            }
                        }
                        if (backButton.getGlobalBounds().contains(mousePos)) {
                            selectingCategory = false;
                        }
                    }
                }
            }
            else {
                if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f worldPos = window.mapPixelToCoords(mouse->position);

                        if (!startGame && !exitGame && !rules && !setting && !showRecords && !manageQuestions) {
                            if (startButton.getGlobalBounds().contains(worldPos)) {
                                Stage = 0;
                                Prize = 0;
                                Lose = false;
                                Exit = false;
                                usedQuestionIds.clear();
                                hints.halfUsed = false;
                                hints.secondChanceUsed = false;
                                hints.secondChanceConsumed = false;
                                hints.audienceUsed = false;
                                selectingCategory = true;
                                categoryButtonsCreated = false;
                            } else if (manageQuestionsButton.getGlobalBounds().contains(worldPos)) {
                                addingQuestion = true;
                            } else if (recordsButton.getGlobalBounds().contains(worldPos)) {
                                showRecords = true;
                            } else if (exitButtonMenu.getGlobalBounds().contains(worldPos)) {
                                exitGame = true;
                                window.close();
                            }
                        } else if (manageQuestions) {
                            if (backButton.getGlobalBounds().contains(worldPos)) manageQuestions = false;
                        } else if (setting) {
                            if (backButton.getGlobalBounds().contains(worldPos)) setting = false;
                        } else if (showRecords) {
                            if (backButton.getGlobalBounds().contains(worldPos)) showRecords = false;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 60));

        if (!startGame && !exitGame && !rules && !setting && !showRecords && !addingQuestion && !selectingCategory) {
            window.draw(titleText);
            window.draw(startButton);
            window.draw(startText);
            window.draw(manageQuestionsButton);
            window.draw(manageQuestionsText);
            window.draw(recordsButton);
            window.draw(recordsText);
            window.draw(exitButtonMenu);
            window.draw(exitTextMenu);
        }
        else if (addingQuestion) {
            if (!formInitialized) {
                fields.clear();
                fields.emplace_back(font, "Категория:", sf::Vector2f(100.f, 100.f));
                fields.emplace_back(font, "Уровень (1-5):", sf::Vector2f(100.f, 150.f));
                fields.emplace_back(font, "Вопрос:", sf::Vector2f(100.f, 200.f));
                fields.emplace_back(font, "Вариант A:", sf::Vector2f(100.f, 250.f));
                fields.emplace_back(font, "Вариант B:", sf::Vector2f(100.f, 300.f));
                fields.emplace_back(font, "Вариант C:", sf::Vector2f(100.f, 350.f));
                fields.emplace_back(font, "Вариант D:", sf::Vector2f(100.f, 400.f));
                fields.emplace_back(font, "Правильный:", sf::Vector2f(100.f, 450.f));
                formInitialized = true;
                if (!fields.empty()) fields[0].setActive(true);
            }

            if (!buttonsCreated) {
                saveButton.setSize({120.f, 40.f});
                saveButton.setFillColor(sf::Color(50, 150, 50));
                saveButton.setPosition(sf::Vector2f(200.f, 520.f));
                saveText = std::make_unique<sf::Text>(font, toSFString("Сохранить"), 20);
                saveText->setFillColor(sf::Color::White);
                saveText->setPosition(sf::Vector2f(215.f, 528.f));

                cancelButton.setSize({120.f, 40.f});
                cancelButton.setFillColor(sf::Color(150, 50, 50));
                cancelButton.setPosition(sf::Vector2f(400.f, 520.f));
                cancelText = std::make_unique<sf::Text>(font, toSFString("Отмена"), 20);
                cancelText->setFillColor(sf::Color::White);
                cancelText->setPosition(sf::Vector2f(420.f, 528.f));

                buttonsCreated = true;
            }

            sf::Text header(font, toSFString("Добавление вопроса"), 30);
            header.setPosition(sf::Vector2f(200.f, 30.f));
            header.setFillColor(sf::Color::Yellow);
            window.draw(header);

            for (auto& field : fields) field.draw(window);
            window.draw(saveButton);
            window.draw(*saveText);
            window.draw(cancelButton);
            window.draw(*cancelText);
        }
        else if (selectingCategory) {
            if (!categoryButtonsCreated) {
                categoryButtons.clear();
                categoryTexts.clear();
                float y = 120.f;
                for (const auto& cat : categories) {
                    sf::RectangleShape btn(sf::Vector2f(250.f, 40.f));
                    btn.setFillColor(sf::Color(50, 50, 50));
                    btn.setOutlineThickness(2.f);
                    btn.setOutlineColor(sf::Color::White);
                    btn.setPosition(sf::Vector2f(275.f, y));

                    sf::Text text(font, toSFString(cat), 20);
                    text.setFillColor(sf::Color::White);
                    text.setPosition(sf::Vector2f(285.f, y + 10.f));

                    categoryButtons.push_back(btn);
                    categoryTexts.push_back(text);
                    y += 50.f;
                }
                categoryButtonsCreated = true;
            }

            sf::Text title(font, toSFString("Выберите категорию"), 30);
            title.setPosition(sf::Vector2f(200.f, 50.f));
            title.setFillColor(sf::Color::Yellow);
            window.draw(title);

            for (size_t i = 0; i < categoryButtons.size(); ++i) {
                window.draw(categoryButtons[i]);
                window.draw(categoryTexts[i]);
            }
            window.draw(backButton);
            window.draw(backText);
        }
        else if (rules) {
            window.draw(rulesTitleText);
            window.draw(backButton);
            window.draw(backText);
        }
        else if (setting) {
            sf::Text settingsText(font, toSFString("Скоро...)"), 30);
            settingsText.setPosition(sf::Vector2f(100.f, 100.f));
            settingsText.setFillColor(sf::Color::White);
            window.draw(settingsText);
            window.draw(backButton);
            window.draw(backText);
        }
        else if (showRecords) {
            sf::Text title(font, toSFString("Таблица рекордов"), 30);
            title.setPosition(sf::Vector2f(200.f, 50.f));
            title.setFillColor(sf::Color::Yellow);
            window.draw(title);

            sf::Text header(font, toSFString("Имя          Выигрыш     Время"), 20);
            header.setPosition(sf::Vector2f(100.f, 120.f));
            header.setFillColor(sf::Color::White);
            window.draw(header);

            float y = 170.f;
            for (size_t i = 0; i < records.size() && i < 10; ++i) {
                const auto& rec = records[i];
                std::string line = rec.playerName + "          " + std::to_string(rec.prize) + " руб.     " + std::to_string(rec.seconds) + " с";
                sf::Text recordText(font, toSFString(line), 18);
                recordText.setPosition(sf::Vector2f(100.f, y));
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

    sf::Music waitingMusic;
    if (!loadWaitingMusic(waitingMusic)) {
        std::cout << "Не удалось загрузить музыку ожидания!" << std::endl;
    } else {
        waitingMusic.setVolume(50.f);
        waitingMusic.setLooping(true);
    }

    sf::Text questionText(font, "", 28);
    questionText.setPosition(sf::Vector2f(50.f, 50.f));
    questionText.setFillColor(sf::Color::White);

    sf::Text prizeText(font, "", 24);
    prizeText.setPosition(sf::Vector2f(50.f, 600.f));
    prizeText.setFillColor(sf::Color::Yellow);

    sf::Text stageText(font, "", 20);
    stageText.setPosition(sf::Vector2f(650.f, 600.f));
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
        audienceTexts[i].setPosition(sf::Vector2f(370.f, 155.f + i * 80.f));
    }

    sf::RectangleShape optionButtons[4];
    for (int i = 0; i < 4; i++) {
        optionButtons[i].setSize({300.f, 50.f});
        optionButtons[i].setFillColor(sf::Color(100, 100, 100, 200));
        optionButtons[i].setOutlineThickness(2.f);
        optionButtons[i].setOutlineColor(sf::Color::White);
        optionButtons[i].setPosition(sf::Vector2f(50.f, 150.f + i * 80.f));

        optionTexts[i].setPosition(sf::Vector2f(70.f, 155.f + i * 80.f));
        optionTexts[i].setFillColor(sf::Color::White);
    }

    sf::RectangleShape exitButton({100.f, 40.f});
    exitButton.setFillColor(sf::Color(200, 0, 0, 200));
    exitButton.setPosition(sf::Vector2f(700.f, 10.f));

    sf::Text exitText(font, "Exit", 20);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(sf::Vector2f(720.f, 15.f));

    sf::RectangleShape halfButton({150.f, 40.f});
    halfButton.setFillColor(sf::Color(50, 200, 50, 200));
    halfButton.setPosition(sf::Vector2f(650.f, 100.f));

    sf::Text halfText(font, "50/50", 20);
    halfText.setFillColor(sf::Color::White);
    halfText.setPosition(sf::Vector2f(670.f, 105.f));

    sf::RectangleShape audienceButton({150.f, 40.f});
    audienceButton.setFillColor(sf::Color(50, 100, 255, 200));
    audienceButton.setPosition(sf::Vector2f(650.f, 170.f));

    sf::Text audienceText(font, toSFString("Помощь зала"), 20);
    audienceText.setFillColor(sf::Color::White);
    audienceText.setPosition(sf::Vector2f(670.f, 175.f));

    sf::RectangleShape secondChanceButton({150.f, 40.f});
    secondChanceButton.setFillColor(sf::Color(255, 165, 0, 200));
    secondChanceButton.setPosition(sf::Vector2f(650.f, 240.f));

    sf::Text secondChanceText(font, toSFString("Второй шанс"), 20);
    secondChanceText.setFillColor(sf::Color::White);
    secondChanceText.setPosition(sf::Vector2f(670.f, 245.f));

    std::random_device rd;
    std::mt19937 gen(rd());

    Question currentQuestion;
    bool needNewQuestion = true;
    hints.halfUsed = false;
    hints.secondChanceUsed = false;
    hints.secondChanceConsumed = false;
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

            std::vector<Question> available;
            for (const auto& q : questions) {
                if ((selectedCategory == "Все" || q.category == selectedCategory) &&
                    q.level == currentLevel + 1 &&
                    std::find(usedQuestionIds.begin(), usedQuestionIds.end(), q.id) == usedQuestionIds.end()) {
                    available.push_back(q);
                }
            }

            if (available.empty()) {
                std::cout << "Нет доступных вопросов для уровня " << currentLevel+1 << " в категории " << selectedCategory << std::endl;
                Stage = 15;
                break;
            }

            std::uniform_int_distribution<int> dist(0, available.size() - 1);
            int randomIndex = dist(gen);
            currentQuestion = available[randomIndex];
            usedQuestionIds.push_back(currentQuestion.id);
            needNewQuestion = false;

            hints.hidden = {false, false, false, false};
            hints.wrongGuesses = {false, false, false, false};

            for (int i = 0; i < 4; i++) {
                optionButtons[i].setFillColor(sf::Color(100, 100, 100, 200));
                optionButtons[i].setOutlineColor(sf::Color::White);
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

                            if (isWaiting) break;

                            selectedOption = i;
                            char choice = char('A' + i);
                            answerResult = currentQuestion.isCorrect(choice);
                            isWaiting = true;
                            waitClock.restart();

                            questionMusic.stop();
                            if (waitingMusic.getStatus() != sf::SoundSource::Status::Playing) {
                                waitingMusic.play();
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
                            hints.secondChanceConsumed = false; // сбрасываем на случай, если раньше использовался
                            secondChanceText.setString(toSFString("Второй шанс (активен)"));
                            secondChanceButton.setFillColor(sf::Color(100, 100, 100));
                        }
                    }
                }
            }
        }

        if (isWaiting) {
            float elapsed = waitClock.getElapsedTime().asSeconds();
            float phase = std::sin(elapsed * 12.0f);
            sf::Color blinkColor = (phase > 0) ? sf::Color(255, 255, 0, 220) : sf::Color(150, 150, 150, 220);
            optionButtons[selectedOption].setFillColor(blinkColor);
            optionButtons[selectedOption].setOutlineColor(sf::Color::Yellow);

            if (elapsed >= waitDuration) {
                isWaiting = false;
                waitingMusic.stop();

                if (answerResult) {
                    Stage++;
                    Prize = prizeTable[Stage];
                    std::cout << "Правильно! Этап: " << Stage << std::endl;
                    needNewQuestion = true;
                    optionButtons[selectedOption].setFillColor(sf::Color(100, 100, 100, 200));
                    optionButtons[selectedOption].setOutlineColor(sf::Color::White);
                } else {
                    if (hints.secondChanceUsed && !hints.secondChanceConsumed) {
                        hints.secondChanceConsumed = true;
                        hints.wrongGuesses[selectedOption] = true;
                        optionButtons[selectedOption].setFillColor(sf::Color(200, 0, 0, 150));
                        optionButtons[selectedOption].setOutlineColor(sf::Color::Red);
                        std::cout << "Использован второй шанс! Попробуйте другой вариант." << std::endl;
                        questionMusic.play();
                        selectedOption = -1;
                    } else {
                        Lose = true;
                        if (Stage >= 10) Prize = 100000;
                        else if (Stage >= 5) Prize = 5000;
                        else Prize = 0;
                        std::cout << "Неправильно!" << std::endl;
                        optionButtons[selectedOption].setFillColor(sf::Color(200, 0, 0, 150));
                        optionButtons[selectedOption].setOutlineColor(sf::Color::Red);
                    }
                }
                selectedOption = -1;
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

    questionMusic.stop();
    auto endTime = std::chrono::steady_clock::now();
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    if (Prize > 0) {
        sf::Text nameLabel(font, toSFString("Введите ваше имя:"), 24);
        nameLabel.setPosition(sf::Vector2f(200.f, 150.f));
        nameLabel.setFillColor(sf::Color::White);

        InputField nameField(font, "", sf::Vector2f(200.f, 200.f), 24);
        nameField.setActive(true);

        sf::RectangleShape saveRecordButton(sf::Vector2f(150.f, 50.f));
        saveRecordButton.setFillColor(sf::Color(50, 150, 50));
        saveRecordButton.setPosition(sf::Vector2f(300.f, 300.f));

        sf::Text saveRecordText(font, toSFString("Сохранить"), 20);
        saveRecordText.setFillColor(sf::Color::White);
        saveRecordText.setPosition(sf::Vector2f(330.f, 310.f));

        bool nameEntered = false;
        while (window.isOpen() && !nameEntered) {
            while (auto event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();
                nameField.handleEvent(*event);
                if (auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f mousePos(static_cast<float>(mouse->position.x),
                                              static_cast<float>(mouse->position.y));
                        if (nameField.getBounds().contains(mousePos)) {
                            nameField.setActive(true);
                        } else {
                            nameField.setActive(false);
                        }
                        if (saveRecordButton.getGlobalBounds().contains(mousePos)) {
                            std::string playerName = nameField.getText();
                            if (playerName.empty()) playerName = "Игрок";
                            addRecord(records, playerName, Prize, seconds);
                            nameEntered = true;
                        }
                    }
                }
                if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Enter) {
                        std::string playerName = nameField.getText();
                        if (playerName.empty()) playerName = "Игрок";
                        addRecord(records, playerName, Prize, seconds);
                        nameEntered = true;
                    }
                }
            }

            window.clear(sf::Color(30, 30, 60));
            window.draw(nameLabel);
            nameField.draw(window);
            window.draw(saveRecordButton);
            window.draw(saveRecordText);
            window.display();
        }
    }

    window.clear(sf::Color(30, 30, 60));
    //ФИНАЛЬНЫЙ ЭКРАН
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
    endText.setPosition(sf::Vector2f(200.f, 200.f));

    sf::Text finalPrize(font, "", 30);
    finalPrize.setString(toSFString("Ваш выигрыш: " + std::to_string(Prize) + " руб."));
    finalPrize.setFillColor(sf::Color::Yellow);
    finalPrize.setPosition(sf::Vector2f(200.f, 300.f));

    window.draw(endText);
    window.draw(finalPrize);
    window.display();

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }
    }
}