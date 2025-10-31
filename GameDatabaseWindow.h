#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "splay.h"
#include "json.hpp"

using json = nlohmann::json;

class GameDatabaseWindow {
private:
    sf::RenderWindow window;
    sf::Font font;

    Splay& splayTree;

    enum Screen {
        MAIN_MENU,
        VIEW_ALL_GAMES,
        SEARCH_GAME,
        ADD_RATING
    };
    Screen currentScreen;

    // Text elements
    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text statusText;

    // Input fields
    std::string searchInput;
    std::string ratingInput;
    std::string gameNameForRating;

    // Search result
    Splay::Node* searchResult;

    // Scroll offset
    float scrollOffset;

    int activeField;

    // Helper methods
    void setText(sf::Text &text, float x, float y);
    void clearInputs();
    void handleTextInput(sf::Uint32 unicode);
    void handleKeyPress(sf::Keyboard::Key key);
    void handleMouseClick(int x, int y);
    void handleScroll(float delta);

    // Rendering methods
    void renderMainMenu();
    void renderViewAllGames();
    void renderSearchGame();
    void renderAddRating();

    void drawButton(const std::string& text, float x, float y, float width, float height, sf::Color color);
    void drawInputBox(const std::string& label, const std::string& value, float x, float y, bool active);
    void drawGameCard(Splay::Node* game, float x, float y, float width);

    std::string* getActiveInput();
    std::string genresToString(const std::vector<std::string>& genres);

public:
    GameDatabaseWindow(int width, int height, Splay& tree);
    void run();
};