#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class InputField {
public:
    InputField(sf::Font& font, const std::string& label, const sf::Vector2f& position, unsigned int charSize = 20);

    void setText(const std::string& text);
    std::string getText() const;
    void setActive(bool active);
    bool isActive() const;

    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);

    void setPosition(const sf::Vector2f& pos);
    sf::FloatRect getBounds() const;

private:
    sf::Text m_label;
    sf::Text m_text;
    sf::RectangleShape m_background;
    bool m_isActive = false;
    std::string m_content;
};