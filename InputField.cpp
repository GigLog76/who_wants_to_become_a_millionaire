#include "InputField.h"
#include <iostream>

InputField::InputField(sf::Font& font, const std::string& label, const sf::Vector2f& position, unsigned int charSize)
    : m_label(font, label, charSize),
      m_text(font, "", charSize)
{
    m_label.setString(sf::String::fromUtf8(label.begin(), label.end()));
    m_label.setFillColor(sf::Color::White);
    m_label.setPosition(position);

    m_text.setFillColor(sf::Color::Black);
    m_text.setPosition({position.x + 150.f, position.y});

    m_background.setSize({300.f, 30.f});
    m_background.setFillColor(sf::Color::White);
    m_background.setOutlineThickness(1.f);
    m_background.setOutlineColor(sf::Color::White);
    m_background.setPosition({position.x + 140.f, position.y - 5.f});
}

void InputField::setText(const std::string& text) {
    m_content = text;
    m_text.setString(sf::String::fromUtf8(m_content.begin(), m_content.end()));
}

std::string InputField::getText() const {
    return m_content;
}

void InputField::setActive(bool active) {
    m_isActive = active;
    if (active) {
        m_background.setOutlineColor(sf::Color::Yellow);
    } else {
        m_background.setOutlineColor(sf::Color::White);
    }
}

bool InputField::isActive() const {
    return m_isActive;
}

void InputField::handleEvent(const sf::Event& event) {
    if (!m_isActive) return;

    if (auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        if (textEvent->unicode == '\b' && !m_content.empty()) {
            size_t pos = m_content.size();
            while (pos > 0 && (m_content[pos-1] & 0xC0) == 0x80) {
                --pos;
            }
            if (pos > 0) {
                m_content.erase(m_content.begin() + pos - 1, m_content.end());
            }
        } else if (textEvent->unicode >= 32) { 
            std::string utf8char;
            uint32_t code = textEvent->unicode;
            if (code < 0x80) {
                utf8char = static_cast<char>(code);
            } else if (code < 0x800) {
                utf8char = static_cast<char>(0xC0 | (code >> 6));
                utf8char += static_cast<char>(0x80 | (code & 0x3F));
            } else if (code < 0x10000) {
                utf8char = static_cast<char>(0xE0 | (code >> 12));
                utf8char += static_cast<char>(0x80 | ((code >> 6) & 0x3F));
                utf8char += static_cast<char>(0x80 | (code & 0x3F));
            } else {
                utf8char = static_cast<char>(0xF0 | (code >> 18));
                utf8char += static_cast<char>(0x80 | ((code >> 12) & 0x3F));
                utf8char += static_cast<char>(0x80 | ((code >> 6) & 0x3F));
                utf8char += static_cast<char>(0x80 | (code & 0x3F));
            }
            m_content += utf8char;
        }
        m_text.setString(sf::String::fromUtf8(m_content.begin(), m_content.end()));
    }
}

void InputField::draw(sf::RenderWindow& window) {
    window.draw(m_background);
    window.draw(m_label);
    window.draw(m_text);
}

void InputField::setPosition(const sf::Vector2f& pos) {
    m_label.setPosition(pos);
    m_text.setPosition({pos.x + 150.f, pos.y});
    m_background.setPosition({pos.x + 140.f, pos.y - 5.f});
}

sf::FloatRect InputField::getBounds() const {
    return m_background.getGlobalBounds();
}