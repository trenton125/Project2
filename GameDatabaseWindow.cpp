#include "GameDatabaseWindow.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>

GameDatabaseWindow::GameDatabaseWindow(int width, int height, Splay& splay, MaxHeap& heap,
                                       double splayTime, double heapTime)
    : splayTree(splay), maxHeap(heap)
{
    window.create(sf::VideoMode(width, height), "Game Database - Heap vs Splay Tree", sf::Style::Close);
    window.clear(sf::Color(40, 44, 52));

    currentScreen = MAIN_MENU;
    currentDataStructure = USING_SPLAY;
    activeField = -1;
    scrollOffset = 0;
    splaySearchResult = nullptr;
    heapSearchResult = nullptr;
    lastSearchTime = 0.0;

    splayBuildTime = splayTime;
    heapBuildTime = heapTime;

    // Load font
    if (!font.loadFromFile("../font.ttf")) {
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
                if (currentDataStructure == USING_SPLAY) {
                    splaySearchResult = splayTree.splaySearch(searchInput);

                    // Get the time that was already calculated in splaySearch
                    lastSearchTime = splayTree.getLastSearchTime();

                    if (splaySearchResult) {
                        statusText.setString("Found in Splay Tree!");
                        statusText.setFillColor(sf::Color::Green);
                    } else {
                        statusText.setString("Game not found in Splay Tree!");
                        statusText.setFillColor(sf::Color::Red);
                        splaySearchResult = nullptr;
                    }
                } else {
                    auto start = std::chrono::high_resolution_clock::now();
                    heapSearchResult = maxHeap.searchGame(searchInput);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end - start;
                    lastSearchTime = elapsed.count();

                    if (heapSearchResult) {
                        statusText.setString("Found in Max Heap!");
                        statusText.setFillColor(sf::Color::Green);
                    } else {
                        statusText.setString("Game not found in Heap!");
                        statusText.setFillColor(sf::Color::Red);
                    }
                }
            }
            catch (const std::exception& e) {
                statusText.setString("Search error!");
                statusText.setFillColor(sf::Color::Red);
                splaySearchResult = nullptr;
                heapSearchResult = nullptr;
                std::cerr << "Search error: " << e.what() << std::endl;
            }
        }
        else if (currentScreen == ADD_RATING && !ratingInput.empty() && !gameNameForRating.empty()) {
            try {
                if (currentDataStructure == USING_SPLAY) {
                    Splay::Node* game = splayTree.splaySearch(gameNameForRating);
                    if (game) {
                        game->user_rating = ratingInput;
                        statusText.setString("Rating added to Splay Tree!");
                        statusText.setFillColor(sf::Color::Green);
                    } else {
                        statusText.setString("Game not found!");
                        statusText.setFillColor(sf::Color::Red);
                    }
                } else {
                    statusText.setString("Add rating only works with Splay Tree!");
                    statusText.setFillColor(sf::Color(255, 165, 0));
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
            // Toggle switch click (moved position)
            if (x >= 650 && x <= 850 && y >= 160 && y <= 200) {
                currentDataStructure = (currentDataStructure == USING_SPLAY) ? USING_HEAP : USING_SPLAY;
            }
            else if (y >= 300 && y <= 350) {
                if (x >= 300 && x <= 700) {
                    currentScreen = VIEW_ALL_GAMES;
                }
            }
            else if (y >= 370 && y <= 420) {
                if (x >= 300 && x <= 700) currentScreen = SEARCH_GAME;
            }
            else if (y >= 440 && y <= 490) {
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
    splaySearchResult = nullptr;
    heapSearchResult = nullptr;
    lastSearchTime = 0.0;
}

void GameDatabaseWindow::renderMainMenu() {
    titleText.setString("GAME DATABASE SYSTEM");
    setText(titleText, window.getSize().x / 2.0f, 80);
    window.draw(titleText);

    subtitleText.setString("Loaded from games.json - Compare Data Structures");
    setText(subtitleText, window.getSize().x / 2.0f, 120);
    window.draw(subtitleText);

    // Draw toggle switch - moved to avoid overlap
    drawToggleSwitch(650, 160);

    // Build time stats
    sf::Text timingText;
    timingText.setFont(font);
    timingText.setCharacterSize(14);
    timingText.setFillColor(sf::Color::Yellow);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    oss << "Splay Tree Build Time: " << splayBuildTime << "s";
    timingText.setString(oss.str());
    setText(timingText, window.getSize().x / 2.0f, 180);
    window.draw(timingText);

    oss.str("");
    oss << "Max Heap Build Time: " << heapBuildTime << "s";
    timingText.setString(oss.str());
    setText(timingText, window.getSize().x / 2.0f, 210);
    window.draw(timingText);

    // Current data structure indicator
    sf::Text dsText;
    dsText.setFont(font);
    dsText.setCharacterSize(18);
    dsText.setFillColor(sf::Color::White);
    dsText.setStyle(sf::Text::Bold);
    std::string dsName = (currentDataStructure == USING_SPLAY) ? "Splay Tree" : "Max Heap";
    dsText.setString("Currently Using: " + dsName);
    setText(dsText, window.getSize().x / 2.0f, 240);
    window.draw(dsText);

    // Buttons
    drawButton("View Data Structure Info", 300, 300, 400, 50, sf::Color(33, 150, 243));
    drawButton("Search Game", 300, 370, 400, 50, sf::Color(255, 152, 0));
    drawButton("Add User Rating (Splay Only)", 300, 440, 400, 50, sf::Color(76, 175, 80));

    // Instructions
    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(12);
    infoText.setFillColor(sf::Color(150, 150, 150));
    infoText.setString("Toggle switch at top right to switch between data structures");
    setText(infoText, window.getSize().x / 2.0f, 550);
    window.draw(infoText);
}

void GameDatabaseWindow::renderViewAllGames() {
    if (currentDataStructure == USING_SPLAY) {
        titleText.setString("SPLAY TREE INFO");
        setText(titleText, window.getSize().x / 2.0f, 80);
        window.draw(titleText);

        subtitleText.setString("(Self-balancing BST - Recently accessed nodes move to root)");
        setText(subtitleText, window.getSize().x / 2.0f, 130);
        window.draw(subtitleText);

        sf::Text infoText;
        infoText.setFont(font);
        infoText.setCharacterSize(16);
        infoText.setFillColor(sf::Color::White);

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6);
        oss << "Build Time: " << splayBuildTime << " seconds";
        infoText.setString(oss.str());
        setText(infoText, window.getSize().x / 2.0f, 200);
        window.draw(infoText);

        infoText.setString("Search Complexity: O(log n) amortized");
        setText(infoText, window.getSize().x / 2.0f, 250);
        window.draw(infoText);

        infoText.setString("Advantages: Fast access to recently searched items");
        setText(infoText, window.getSize().x / 2.0f, 300);
        window.draw(infoText);
    } else {
        titleText.setString("MAX HEAP INFO");
        setText(titleText, window.getSize().x / 2.0f, 80);
        window.draw(titleText);

        subtitleText.setString("(Complete binary tree - Max element at root)");
        setText(subtitleText, window.getSize().x / 2.0f, 130);
        window.draw(subtitleText);

        sf::Text infoText;
        infoText.setFont(font);
        infoText.setCharacterSize(16);
        infoText.setFillColor(sf::Color::White);

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6);
        oss << "Build Time: " << heapBuildTime << " seconds";
        infoText.setString(oss.str());
        setText(infoText, window.getSize().x / 2.0f, 200);
        window.draw(infoText);

        infoText.setString("Search Complexity: O(n) - Linear search required");
        setText(infoText, window.getSize().x / 2.0f, 250);
        window.draw(infoText);

        infoText.setString("Advantages: Fast access to max element O(1)");
        setText(infoText, window.getSize().x / 2.0f, 300);
        window.draw(infoText);

        if (!maxHeap.empty()) {
            try {
                infoText.setString("Heap has " + std::to_string(maxHeap.size()) + " games");
                infoText.setFillColor(sf::Color::Yellow);
                setText(infoText, window.getSize().x / 2.0f, 350);
                window.draw(infoText);
            } catch (...) {}
        }
    }

    drawButton("Back to Menu", 350, 600, 300, 50, sf::Color(100, 100, 100));
}

void GameDatabaseWindow::renderSearchGame() {
    titleText.setString("SEARCH FOR GAME");
    setText(titleText, window.getSize().x / 2.0f, 80);
    window.draw(titleText);

    std::string dsName = (currentDataStructure == USING_SPLAY) ? "Splay Tree" : "Max Heap";
    subtitleText.setString("(Using " + dsName + ")");
    setText(subtitleText, window.getSize().x / 2.0f, 130);
    window.draw(subtitleText);

    drawInputBox("Game Title:", searchInput, 100, 200, activeField == 0);

    drawButton("Search", 350, 270, 300, 50, sf::Color(255, 152, 0));

    // Display search result
    if (currentDataStructure == USING_SPLAY && splaySearchResult != nullptr) {
        float cardY = 350;
        drawGameCard(splaySearchResult, 100, cardY, 800);

        // Display search time
        sf::Text timeText;
        timeText.setFont(font);
        timeText.setCharacterSize(14);
        timeText.setFillColor(sf::Color::Cyan);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6);
        oss << "Search Time: " << lastSearchTime << " seconds";
        timeText.setString(oss.str());
        setText(timeText, window.getSize().x / 2.0f, 490);
        window.draw(timeText);
    }
    else if (currentDataStructure == USING_HEAP && heapSearchResult != nullptr) {
        float cardY = 350;
        drawGameCardHeap(*heapSearchResult, 100, cardY, 800);

        // Display search time
        sf::Text timeText;
        timeText.setFont(font);
        timeText.setCharacterSize(14);
        timeText.setFillColor(sf::Color::Cyan);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6);
        oss << "Search Time: " << lastSearchTime << " seconds";
        timeText.setString(oss.str());
        setText(timeText, window.getSize().x / 2.0f, 490);
        window.draw(timeText);
    }
    else if (!statusText.getString().isEmpty()) {
        setText(statusText, window.getSize().x / 2.0f, 400);
        window.draw(statusText);

        if (lastSearchTime > 0) {
            sf::Text timeText;
            timeText.setFont(font);
            timeText.setCharacterSize(14);
            timeText.setFillColor(sf::Color::Cyan);
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(6);
            oss << "Search Time: " << lastSearchTime << " seconds";
            timeText.setString(oss.str());
            setText(timeText, window.getSize().x / 2.0f, 440);
            window.draw(timeText);
        }
    }

    drawButton("Back to Menu", 350, 600, 300, 50, sf::Color(100, 100, 100));
}

void GameDatabaseWindow::renderAddRating() {
    titleText.setString("ADD USER RATING");
    setText(titleText, window.getSize().x / 2.0f, 80);
    window.draw(titleText);

    subtitleText.setString("(Only works with Splay Tree)");
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

void GameDatabaseWindow::drawToggleSwitch(float x, float y) {
    // Background
    sf::RectangleShape switchBg(sf::Vector2f(200, 40));
    switchBg.setPosition(x, y);
    switchBg.setFillColor(sf::Color(60, 60, 70));
    switchBg.setOutlineColor(sf::Color::White);
    switchBg.setOutlineThickness(2);
    window.draw(switchBg);

    // Active side highlight
    sf::RectangleShape activeHighlight(sf::Vector2f(100, 40));
    if (currentDataStructure == USING_SPLAY) {
        activeHighlight.setPosition(x, y);
        activeHighlight.setFillColor(sf::Color(156, 39, 176)); // Purple for Splay
    } else {
        activeHighlight.setPosition(x + 100, y);
        activeHighlight.setFillColor(sf::Color(33, 150, 243)); // Blue for Heap
    }
    window.draw(activeHighlight);

    // Labels
    sf::Text splayLabel;
    splayLabel.setFont(font);
    splayLabel.setString("Splay");
    splayLabel.setCharacterSize(16);
    splayLabel.setFillColor(sf::Color::White);
    splayLabel.setStyle(sf::Text::Bold);
    setText(splayLabel, x + 50, y + 20);
    window.draw(splayLabel);

    sf::Text heapLabel;
    heapLabel.setFont(font);
    heapLabel.setString("Heap");
    heapLabel.setCharacterSize(16);
    heapLabel.setFillColor(sf::Color::White);
    heapLabel.setStyle(sf::Text::Bold);
    setText(heapLabel, x + 150, y + 20);
    window.draw(heapLabel);
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

    // Game title
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

void GameDatabaseWindow::drawGameCardHeap(const Game& game, float x, float y, float width) {
    // Card background
    sf::RectangleShape card(sf::Vector2f(width, 120));
    card.setPosition(x, y);
    card.setFillColor(sf::Color(60, 60, 70));
    card.setOutlineColor(sf::Color(33, 150, 243)); // Blue for heap
    card.setOutlineThickness(2);
    window.draw(card);

    // Game title
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString(game.title);
    titleText.setCharacterSize(20);
    titleText.setFillColor(sf::Color(255, 215, 0)); // Gold
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(x + 10, y + 10);
    window.draw(titleText);

    // Platform
    sf::Text platformText;
    platformText.setFont(font);
    platformText.setString("Platform: " + game.platforms);
    platformText.setCharacterSize(14);
    platformText.setFillColor(sf::Color::White);
    platformText.setPosition(x + 10, y + 40);
    window.draw(platformText);

    // Rating
    sf::Text ratingText;
    ratingText.setFont(font);
    ratingText.setString("IGN Rating: " + std::to_string(game.rating).substr(0, 4));
    ratingText.setCharacterSize(14);
    ratingText.setFillColor(sf::Color(100, 255, 100));
    ratingText.setPosition(x + 10, y + 60);
    window.draw(ratingText);

    // User rating
    sf::Text userRatingText;
    userRatingText.setFont(font);
    std::string displayRating = (game.userRating == "-1") ? "Not rated yet" : game.userRating;
    userRatingText.setString("Your Rating: " + displayRating);
    userRatingText.setCharacterSize(14);
    userRatingText.setFillColor(sf::Color::Yellow);
    userRatingText.setPosition(x + 10, y + 80);
    window.draw(userRatingText);

    // Genres
    sf::Text genreText;
    genreText.setFont(font);
    genreText.setString("Genres: " + genresToString(game.genres));
    genreText.setCharacterSize(12);
    genreText.setFillColor(sf::Color(150, 150, 255));
    genreText.setPosition(x + 300, y + 60);
    window.draw(genreText);
}