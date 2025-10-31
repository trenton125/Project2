#include "GameDatabaseWindow.h"
#include <iostream>
#include <fstream>
#include <algorithm>

GameDatabaseWindow::GameDatabaseWindow(int width, int height, Splay& tree)
    : splayTree(tree)
{
    window.create(sf::VideoMode(width, height), "Game Database - JSON Loaded", sf::Style::Close);
    window.clear(sf::Color(40, 44, 52));

    currentScreen = MAIN_MENU;
    activeField = -1;
    scrollOffset = 0;
    searchResult = nullptr;

    // Load font
    if (!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Failed to load font! Place font.ttf in files/ folder" << std::endl;
    }

    // Setup title text
    titleText.setFont(font);
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);

    // Setup subtitle text
    subtitleText.setFont(font);
    subtitleText.setCharacterSize(18);
    subtitleText.setFillColor(sf::Color(150, 150, 150));
    subtitleText.setStyle(sf::Text::Bold);

    // Setup status text
    statusText.setFont(font);
    statusText.setCharacterSize(16);
    statusText.setFillColor(sf::Color::Green);
}

void GameDatabaseWindow::setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

std::string GameDatabaseWindow::genresToString(const std::vector<std::string>& genres) {
    std::string result;
    for (size_t i = 0; i < genres.size(); i++) {
        result += genres[i];
        if (i < genres.size() - 1) result += ", ";
    }
    return result;
}

void GameDatabaseWindow::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
            else if (event.type == sf::Event::TextEntered) {
                handleTextInput(event.text.unicode);
            }
            else if (event.type == sf::Event::KeyPressed) {
                handleKeyPress(event.key.code);
            }
            else if (event.type == sf::Event::MouseWheelScrolled) {
                handleScroll(event.mouseWheelScroll.delta);
            }
        }

        // Clear window
        window.clear(sf::Color(40, 44, 52));

        // Render current screen
        switch (currentScreen) {
            case MAIN_MENU:
                renderMainMenu();
                break;
            case VIEW_ALL_GAMES:
                renderViewAllGames();
                break;
            case SEARCH_GAME:
                renderSearchGame();
                break;
            case ADD_RATING:
                renderAddRating();
                break;
        }

        window.display();
    }
}

void GameDatabaseWindow::handleTextInput(sf::Uint32 unicode) {
    if (activeField < 0) return;

    std::string* currentInput = getActiveInput();
    if (!currentInput) return;

    // Handle backspace
    if (unicode == 8) {
        if (!currentInput->empty()) {
            currentInput->pop_back();
        }
        return;
    }

    // Accept printable characters
    if (unicode >= 32 && unicode < 128) {
        if (currentInput->length() < 100) {
            *currentInput += static_cast<char>(unicode);
        }
    }
}

void GameDatabaseWindow::handleKeyPress(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Escape) {
        currentScreen = MAIN_MENU;
        clearInputs();
        activeField = -1;
        scrollOffset = 0;
    }
    else if (key == sf::Keyboard::Return) {
        if (currentScreen == SEARCH_GAME && !searchInput.empty()) {
            try {
                searchResult = splayTree.splaySearch(searchInput);
                if (searchResult) {
                    statusText.setString("Found: " + searchResult->title);
                    statusText.setFillColor(sf::Color::Green);
                } else {
                    statusText.setString("Game not found!");
                    statusText.setFillColor(sf::Color::Red);
                    searchResult = nullptr;
                }
            }
            catch (const std::exception& e) {
                statusText.setString("Search error!");
                statusText.setFillColor(sf::Color::Red);
                searchResult = nullptr;
                std::cerr << "Search error: " << e.what() << std::endl;
            }
        }
        else if (currentScreen == ADD_RATING && !ratingInput.empty() && !gameNameForRating.empty()) {
            try {
                Splay::Node* game = splayTree.splaySearch(gameNameForRating);
                if (game) {
                    game->user_rating = ratingInput;
                    statusText.setString("Rating added successfully!");
                    statusText.setFillColor(sf::Color::Green);
                } else {
                    statusText.setString("Game not found!");
                    statusText.setFillColor(sf::Color::Red);
                }
            }
            catch (const std::exception& e) {
                statusText.setString("Error adding rating!");
                statusText.setFillColor(sf::Color::Red);
                std::cerr << "Rating error: " << e.what() << std::endl;
            }
        }
    }
}

void GameDatabaseWindow::handleScroll(float delta) {
    if (currentScreen == VIEW_ALL_GAMES) {
        scrollOffset += delta * 30;
        if (scrollOffset > 0) scrollOffset = 0;
    }
}

void GameDatabaseWindow::handleMouseClick(int x, int y) {
    switch (currentScreen) {
        case MAIN_MENU:
            if (y >= 250 && y <= 300) {
                if (x >= 300 && x <= 700) {
                    currentScreen = VIEW_ALL_GAMES;
                }
            }
            else if (y >= 320 && y <= 370) {
                if (x >= 300 && x <= 700) currentScreen = SEARCH_GAME;
            }
            else if (y >= 390 && y <= 440) {
                if (x >= 300 && x <= 700) currentScreen = ADD_RATING;
            }
            break;

        case SEARCH_GAME:
            if (y >= 200 && y <= 230) activeField = 0;
            else if (y >= 270 && y <= 320 && x >= 350 && x <= 650) {
                handleKeyPress(sf::Keyboard::Return);
            }
            else if (y >= 600 && y <= 650 && x >= 350 && x <= 650) {
                currentScreen = MAIN_MENU;
                clearInputs();
            }
            break;

        case ADD_RATING:
            if (y >= 200 && y <= 230) activeField = 0;  // Game name
            else if (y >= 260 && y <= 290) activeField = 1;  // Rating
            else if (y >= 330 && y <= 380 && x >= 350 && x <= 650) {
                handleKeyPress(sf::Keyboard::Return);
            }
            else if (y >= 600 && y <= 650 && x >= 350 && x <= 650) {
                currentScreen = MAIN_MENU;
                clearInputs();
            }
            break;

        default:
            if (y >= 600 && y <= 650 && x >= 350 && x <= 650) {
                currentScreen = MAIN_MENU;
                scrollOffset = 0;
            }
            break;
    }
}

std::string* GameDatabaseWindow::getActiveInput() {
    if (currentScreen == SEARCH_GAME && activeField == 0) {
        return &searchInput;
    }
    else if (currentScreen == ADD_RATING) {
        if (activeField == 0) return &gameNameForRating;
        if (activeField == 1) return &ratingInput;
    }
    return nullptr;
}

void GameDatabaseWindow::clearInputs() {
    searchInput.clear();
    ratingInput.clear();
    gameNameForRating.clear();
    statusText.setString("");
    searchResult = nullptr;
}

void GameDatabaseWindow::renderMainMenu() {
    titleText.setString("GAME DATABASE SYSTEM");
    setText(titleText, window.getSize().x / 2.0f, 100);
    window.draw(titleText);

    subtitleText.setString("Loaded from games.json - Splay Tree Implementation");
    setText(subtitleText, window.getSize().x / 2.0f, 150);
    window.draw(subtitleText);

    // Stats
    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(16);
    statsText.setFillColor(sf::Color::Yellow);
    statsText.setString("Games loaded from JSON file");
    setText(statsText, window.getSize().x / 2.0f, 200);
    window.draw(statsText);

    // Buttons
    drawButton("View All Games", 300, 250, 400, 50, sf::Color(33, 150, 243));
    drawButton("Search Game", 300, 320, 400, 50, sf::Color(255, 152, 0));
    drawButton("Add User Rating", 300, 390, 400, 50, sf::Color(76, 175, 80));

    // Instructions
    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(14);
    infoText.setFillColor(sf::Color(150, 150, 150));
    infoText.setString("Data Structure: Self-Balancing Splay Tree");
    setText(infoText, window.getSize().x / 2.0f, 500);
    window.draw(infoText);
}

void GameDatabaseWindow::renderViewAllGames() {
    titleText.setString("ALL GAMES");
    setText(titleText, window.getSize().x / 2.0f, 80);
    window.draw(titleText);

    subtitleText.setString("(Splay Tree - Recently accessed games move to root)");
    setText(subtitleText, window.getSize().x / 2.0f, 130);
    window.draw(subtitleText);

    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(16);
    infoText.setFillColor(sf::Color::White);
    infoText.setString("Use Search to find specific games!");
    setText(infoText, window.getSize().x / 2.0f, 200);
    window.draw(infoText);

    sf::Text noteText;
    noteText.setFont(font);
    noteText.setCharacterSize(14);
    noteText.setFillColor(sf::Color(150, 150, 150));
    noteText.setString("Check console for pre-order traversal of all games");
    setText(noteText, window.getSize().x / 2.0f, 300);
    window.draw(noteText);

    drawButton("Back to Menu", 350, 600, 300, 50, sf::Color(100, 100, 100));
}

void GameDatabaseWindow::renderSearchGame() {
    titleText.setString("SEARCH FOR GAME");
    setText(titleText, window.getSize().x / 2.0f, 80);
    window.draw(titleText);

    subtitleText.setString("(Uses Splay Operation - O(log n) amortized)");
    setText(subtitleText, window.getSize().x / 2.0f, 130);
    window.draw(subtitleText);

    drawInputBox("Game Title:", searchInput, 100, 200, activeField == 0);

    drawButton("Search", 350, 270, 300, 50, sf::Color(255, 152, 0));

    // Display search result
    if (searchResult != nullptr) {
        float cardY = 350;
        drawGameCard(searchResult, 100, cardY, 800);
    }
    else if (!statusText.getString().isEmpty()) {
        setText(statusText, window.getSize().x / 2.0f, 400);
        window.draw(statusText);
    }

    drawButton("Back to Menu", 350, 600, 300, 50, sf::Color(100, 100, 100));
}

void GameDatabaseWindow::renderAddRating() {
    titleText.setString("ADD USER RATING");
    setText(titleText, window.getSize().x / 2.0f, 80);
    window.draw(titleText);

    subtitleText.setString("(Add your personal rating to any game)");
    setText(subtitleText, window.getSize().x / 2.0f, 130);
    window.draw(subtitleText);

    drawInputBox("Game Title:", gameNameForRating, 100, 200, activeField == 0);
    drawInputBox("Your Rating (0-10):", ratingInput, 100, 260, activeField == 1);

    drawButton("Add Rating", 350, 330, 300, 50, sf::Color(76, 175, 80));

    if (!statusText.getString().isEmpty()) {
        setText(statusText, window.getSize().x / 2.0f, 450);
        window.draw(statusText);
    }

    drawButton("Back to Menu", 350, 600, 300, 50, sf::Color(100, 100, 100));
}

void GameDatabaseWindow::drawButton(const std::string& text, float x, float y, float width, float height, sf::Color color) {
    sf::RectangleShape button(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(color);
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(3);
    window.draw(button);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setStyle(sf::Text::Bold);
    setText(buttonText, x + width/2.0f, y + height/2.0f);
    window.draw(buttonText);
}

void GameDatabaseWindow::drawInputBox(const std::string& label, const std::string& value, float x, float y, bool active) {
    sf::Text labelText;
    labelText.setFont(font);
    labelText.setString(label);
    labelText.setCharacterSize(16);
    labelText.setFillColor(sf::Color::White);
    labelText.setStyle(sf::Text::Bold);
    labelText.setPosition(x, y);
    window.draw(labelText);

    sf::RectangleShape inputBox(sf::Vector2f(600, 30));
    inputBox.setPosition(x + 200, y);
    inputBox.setFillColor(sf::Color(60, 60, 70));
    inputBox.setOutlineColor(active ? sf::Color::Yellow : sf::Color(100, 100, 100));
    inputBox.setOutlineThickness(2);
    window.draw(inputBox);

    sf::Text valueText;
    valueText.setFont(font);
    valueText.setString(value + (active ? "|" : ""));
    valueText.setCharacterSize(16);
    valueText.setFillColor(sf::Color::White);
    valueText.setPosition(x + 210, y + 5);
    window.draw(valueText);
}

void GameDatabaseWindow::drawGameCard(Splay::Node* game, float x, float y, float width) {
    // Card background
    sf::RectangleShape card(sf::Vector2f(width, 120));
    card.setPosition(x, y);
    card.setFillColor(sf::Color(60, 60, 70));
    card.setOutlineColor(sf::Color(76, 175, 80));
    card.setOutlineThickness(2);
    window.draw(card);
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString(game->title);
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color(255, 215, 0)); // Gold
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(x + 10, y + 10);
    window.draw(titleText);

    // Platform
    sf::Text platformText;
    platformText.setFont(font);
    platformText.setString("Platform: " + game->platform);
    platformText.setCharacterSize(14);
    platformText.setFillColor(sf::Color::White);
    platformText.setPosition(x + 10, y + 40);
    window.draw(platformText);

    // Rating
    sf::Text ratingText;
    ratingText.setFont(font);
    ratingText.setString("IGN Rating: " + std::to_string(game->ign_rating).substr(0, 4));
    ratingText.setCharacterSize(14);
    ratingText.setFillColor(sf::Color(100, 255, 100));
    ratingText.setPosition(x + 10, y + 60);
    window.draw(ratingText);

    // User rating
    sf::Text userRatingText;
    userRatingText.setFont(font);
    std::string displayRating = (game->user_rating == "-1") ? "Not rated yet" : game->user_rating;
    userRatingText.setString("Your Rating: " + displayRating);
    userRatingText.setCharacterSize(14);
    userRatingText.setFillColor(sf::Color::Yellow);
    userRatingText.setPosition(x + 10, y + 80);
    window.draw(userRatingText);

    // Genres
    sf::Text genreText;
    genreText.setFont(font);
    genreText.setString("Genres: " + genresToString(game->genre));
    genreText.setCharacterSize(12);
    genreText.setFillColor(sf::Color(150, 150, 255));
    genreText.setPosition(x + 300, y + 60);
    window.draw(genreText);
}