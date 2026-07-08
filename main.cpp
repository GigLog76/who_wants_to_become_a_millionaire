#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <random>
#include <SFML/Graphics.hpp>

// --- Глобальные переменные ---
int Prize = 0;
int Stage = 0;
bool Lose = false;

// --- Структура вопроса ---
struct Question {
    int id;
    int level;
    sf::String question;
    std::array<sf::String, 4> options;
    int correct;

    Question() : id(0), level(1), correct(0) {
        options.fill("");
    }

    Question(int i, int lvl, const sf::String& q,
             const std::array<sf::String, 4>& opts, int c)
        : id(i), level(lvl), question(q), options(opts), correct(c) {}

    bool isCorrect(char answerIndex) const {
        return (answerIndex - 'A') == correct;
    }
};

// --- Вопросы ---
std::vector<Question> questions = {
    {1, 1, "What is the capital of France?", {"London", "Berlin", "Paris", "Madrid"}, 2},
    {2, 1, "How many days in a week?", {"5", "6", "7", "8"}, 2},
    {3, 1, "Which gas do we breathe from the air?", {"Oxygen", "Carbon Dioxide", "Nitrogen", "Hydrogen"}, 0},
    {4, 1, "Who wrote the fairy tale 'Kolobok'?", {"Pushkin", "Folk", "Ershov", "Tolstoy"}, 1},
    {5, 1, "Which is the largest continent?", {"Africa", "North America", "Eurasia", "South America"}, 2},
    {6, 2, "How many planets in the Solar System?", {"8", "9", "7", "10"}, 0},
    {7, 2, "What element is denoted by 'Fe'?", {"Fluorine", "Phosphorus", "Iron", "Francium"}, 2},
    {8, 2, "When did man first step on the Moon?", {"1965", "1969", "1972", "1959"}, 1},
    {9, 2, "What animal is the symbol of Australia?", {"Koala", "Kangaroo", "Platypus", "Wombat"}, 1},
    {10, 2, "How many players in a football team (incl. goalkeeper)?", {"10", "11", "12", "9"}, 1},
    {11, 3, "Which Russian writer was a Nobel Prize laureate?", {"Tolstoy", "Dostoevsky", "Bunin", "Chekhov"}, 2},
    {12, 3, "Which element is the most abundant in the Universe?", {"Oxygen", "Hydrogen", "Helium", "Carbon"}, 1},
    {13, 3, "Which country has the longest coastline?", {"USA", "Russia", "Canada", "Australia"}, 2},
    {14, 3, "In which Shakespeare work do Romeo and Juliet appear?", {"Hamlet", "Othello", "Romeo and Juliet", "King Lear"}, 2},
    {15, 3, "What is the process of turning liquid into vapor?", {"Condensation", "Evaporation", "Sublimation", "Crystallization"}, 1},
    {16, 4, "Who founded the Rurik dynasty?", {"Rurik", "Oleg", "Igor", "Svyatoslav"}, 0},
    {17, 4, "In which year was the Battle of Grunwald?", {"1410", "1380", "1420", "1453"}, 0},
    {18, 4, "Which physicist proposed the theory of relativity?", {"Newton", "Galileo", "Einstein", "Bohr"}, 2},
    {19, 4, "What plant is the largest flower in the world?", {"Victoria", "Rafflesia", "Amorphophallus", "Giant cactus"}, 1},
    {20, 4, "Who was the Emperor of the Holy Roman Empire?", {"Napoleon", "Charles V", "Peter I", "Frederick II"}, 1},
    {21, 5, "Which metal has the highest melting point?", {"Platinum", "Tungsten", "Rhenium", "Osmium"}, 1},
    {22, 5, "What is the deepest point of the World Ocean?", {"Mariana Trench", "Tonga Trench", "Philippine Trench", "Kermadec"}, 0},
    {23, 5, "Who first crossed the Arctic Ocean in a balloon?", {"Amundsen", "Nansen", "Sedov", "Wilkins"}, 3},
    {24, 5, "What work is considered the first novel?", {"The Tale of Genji", "Don Quixote", "Gargantua", "The Decameron"}, 0},
    {25, 5, "How many chromosomes does a human somatic cell have?", {"44", "46", "48", "23"}, 1}
};

// --- Призы ---
std::array<int, 16> prizeTable = {
    0, 500, 1000, 2000, 3000, 5000, 10000, 25000, 50000, 75000,
    100000, 200000, 400000, 800000, 1500000, 3000000
};

// --- Главная функция ---
int main() {
    // Подключаем шрифт
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "No font found!" << std::endl;
        return 1;
    }

    // Создаём окно игры
    sf::RenderWindow window(sf::VideoMode({800, 650}), "Who wants to be a millionaire");
    window.setFramerateLimit(60);

    // Тексты для интерфейса
    sf::Text questionText(font, "", 28);
    questionText.setPosition({50, 50});
    questionText.setFillColor(sf::Color::White);

    sf::Text prizeText(font, "", 24);
    prizeText.setPosition({50, 600});
    prizeText.setFillColor(sf::Color::Yellow);

    sf::Text stageText(font, "", 20);
    stageText.setPosition({700, 600});
    stageText.setFillColor(sf::Color::Cyan);

    // --- ИСПРАВЛЕНИЕ: инициализируем массив сразу со шрифтом ---
    sf::Text optionTexts[4] = {
        sf::Text(font, "", 22),
        sf::Text(font, "", 22),
        sf::Text(font, "", 22),
        sf::Text(font, "", 22)
    };

    // Кнопки ответов
    sf::RectangleShape optionButtons[4];
    for (int i = 0; i < 4; i++) {
        optionButtons[i].setSize({300, 50});
        optionButtons[i].setFillColor(sf::Color(100, 100, 100, 200));
        optionButtons[i].setOutlineThickness(2);
        optionButtons[i].setOutlineColor(sf::Color::White);
        optionButtons[i].setPosition({50, 150 + i * 80});

        optionTexts[i].setPosition({70, 155 + i * 80});
        optionTexts[i].setFillColor(sf::Color::White);
    }

    // Кнопка Выход (Q)
    sf::RectangleShape exitButton({100, 40});
    exitButton.setFillColor(sf::Color(200, 0, 0, 200));
    exitButton.setPosition({700, 10});

    sf::Text exitText(font, "Quit", 20);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition({720, 15});

    // Генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());

    Question currentQuestion;
    bool needNewQuestion = true;

    // --- ГЛАВНЫЙ ИГРОВОЙ ЦИКЛ ---
    while (window.isOpen() && Stage < 15 && !Lose) {
        // Выбор нового вопроса
        if (needNewQuestion) {
            int currentLevel = Stage / 3;
            int start = currentLevel * 5;
            int end = start + 4;
            if (currentLevel >= 5) break;

            std::uniform_int_distribution<int> dist(start, end);
            int randomIndex = dist(gen);
            currentQuestion = questions[randomIndex];
            needNewQuestion = false;
        }

        // Обновляем текст на экране
        questionText.setString(currentQuestion.question);
        for (int i = 0; i < 4; i++) {
            std::string label = std::string(1, char('A' + i)) + ") " + currentQuestion.options[i].toAnsiString();
            optionTexts[i].setString(label);
        }
        prizeText.setString("Prize: " + std::to_string(Prize) + " rub.");
        stageText.setString("Q" + std::to_string(Stage + 1) + "/15");

        // --- ОБРАБОТКА СОБЫТИЙ ---
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Q) {
                    std::cout << "Player took " << Prize << " rub. (Quit)\n";
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
                                std::cout << "Correct! Stage: " << Stage << std::endl;
                                needNewQuestion = true;
                            } else {
                                Lose = true;
                                if (Stage >= 10) Prize = 100000;
                                else if (Stage >= 5) Prize = 5000;
                                else Prize = 0;
                                std::cout << "Wrong!" << std::endl;
                            }
                            break;
                        }
                    }

                    if (exitButton.getGlobalBounds().contains(worldPos)) {
                        std::cout << "Player took " << Prize << " rub.\n";
                        window.close();
                    }
                }
            }
        }

        // --- ОТРИСОВКА ---
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

    // --- ЭКРАН КОНЦА ИГРЫ ---
    window.clear(sf::Color(30, 30, 60));
    
    sf::Text endText(font, Lose ? "You lost!" : "VICTORY! You are a millionaire!", 40);
    endText.setFillColor(Lose ? sf::Color::Red : sf::Color::Green);
    endText.setPosition({200, 200});

    sf::Text finalPrize(font, "Your prize: " + std::to_string(Prize) + " rub.", 30);
    finalPrize.setFillColor(sf::Color::Yellow);
    finalPrize.setPosition({200, 300});

    window.draw(endText);
    window.draw(finalPrize);
    window.display();

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }
    }

    return 0;
}