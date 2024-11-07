#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
// #include <math.h>
#include <cstddef>
#include <deque>
#include <queue>
#include <random>
#include <string>

const int WIDTH = 800;
const int HEIGHT = 600;
const int CELL_SIZE = 20;
const float SPEED = 0.1f;
// const int SIZE = 2;

class Menu {
public:
    Menu();
    void draw(sf::RenderWindow &window);
    void moveUp();
    void moveDown();
    void setselectedItemIndex();
    int getPressedItem() const { return selectedItemIndex; }
    void reset();
    void reset1();
    void draw_a(float width, float height);

private:
    int selectedItemIndex;
    std::vector<sf::Text> menuItems;
    std::vector<sf::Text> settingItems;
    std::vector<sf::Text> colorItems;
    sf::Font font;
};

Menu::Menu() {
    if (!font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf")) {
    }

    menuItems.emplace_back("Start Game", font, 50);
    menuItems.emplace_back("Settings", font, 50);
    menuItems.emplace_back("Exit", font, 50);
    selectedItemIndex = 0;
}

void Menu::reset() {
    while (!menuItems.empty()) {
        menuItems.pop_back();
    }

    menuItems.emplace_back("Green", font, 50);
    menuItems.emplace_back("Blue", font, 50);
    menuItems.emplace_back("Yellow", font, 50);

    selectedItemIndex = 0;
}

void Menu::reset1() {
    while (!menuItems.empty()) {
        menuItems.pop_back();
    }

    menuItems.emplace_back("One", font, 50);
    menuItems.emplace_back("Two", font, 50);

    selectedItemIndex = 0;
}

void Menu::draw_a(float width, float height) {
    for (size_t i = 0; i < menuItems.size(); i++) {
        menuItems[i].setPosition(
            width / 2 - menuItems[i].getGlobalBounds().width / 2,
            height / float(menuItems.size() + 1) * float(i + 1));
        menuItems[i].setFillColor(sf::Color::White);

        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::draw(sf::RenderWindow &window) {
    for (const auto &item : menuItems) {
        window.draw(item);
    }
}

void Menu::setselectedItemIndex() { selectedItemIndex = 0; }

void Menu::moveUp() {
    if (selectedItemIndex - 1 >= 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (float(selectedItemIndex + 1) < float(menuItems.size())) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::White);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

enum Direction { Up, Down, Left, Right, W, S, A, D };

struct Snake {
    std::deque<sf::Vector2i> body[2];

    Direction dir[2];
    std::queue<Direction> Myqueue[2];
};

void resetSnake(Snake &snake, int snake_index) {

    snake.body[snake_index].clear();
    snake.body[snake_index].emplace_back(10, 10 + 10 * snake_index);
    snake.dir[snake_index] = Right;
}
// void resetSnake1(Snake &snake) {
//     snake.body[1].clear();
//     snake.body[1].emplace_back(10, 20);
//     snake.dir[1] = Right;
// }
// void drawscore(size_t length, int num) {
//     std::vector<sf::Text> scoreItems;
//     sf::Font font;
//     scoreItems.emplace_back(
//         "Player " + std::to_string(num) + ":" + std::to_string(length), font, 50);
//     scoreItems[0].setPosition(float(20) + float((num - 1) * 400), 700);
//     scoreItems[0].setFillColor(sf::Color::White);

//     while (!scoreItems.empty()) {
//         scoreItems.pop_back();

//     }
// }
void moveSnake(Snake &snake, int num) {

    sf::Vector2i newHead[2];

    for (int i = 0; i <= num - 1; i++) {
        newHead[i] = snake.body[i].front();
        Direction clone_dir;
        clone_dir = snake.dir[i];
        if (snake.dir[i] == W) {
            clone_dir = Up;
        }
        if (snake.dir[i] == A) {
            clone_dir = Left;
        }
        if (snake.dir[i] == D) {
            clone_dir = Right;
        }
        if (snake.dir[i] == S) {
            clone_dir = Down;
        }

        switch (clone_dir) {
        case Up:
            newHead[i].y--;
            break;
        case Down:
            newHead[i].y++;
            break;
        case Left:
            newHead[i].x--;
            break;
        case Right:
            newHead[i].x++;
            break;
        default:
            break;
        }

        snake.body[i].push_front(newHead[i]);
        snake.body[i].pop_back();
    }
}

bool isCollision(const Snake &snake, int num, int snake_index) {

    const sf::Vector2i head = snake.body[snake_index].front();

    if (head.x < 0 || head.x >= WIDTH / CELL_SIZE || head.y < 0 || head.y >= HEIGHT / CELL_SIZE)
        return true;
    for (int j = 0; j <= num - 1; j++) {
        if (j == snake_index) {
            for (size_t i = 1; i < snake.body[j].size(); ++i) {
                if (snake.body[j][i] == head)
                    return true;
            }
        }
        if (j != snake_index) {
            for (const auto &body : snake.body[j]) {
                if (body == head)
                    return true;
            }
        }
    }

    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT + 200), "Snake Game");
    Snake snake;
    std::queue<Direction> Myqueue[2];
    int num = 1;
    int ge = 0;
    sf::Color snakeColor[2] = {sf::Color::Green, sf::Color::Yellow};
    std::random_device rd;  // 获取随机数种子
    std::mt19937 gen(rd()); // 初始化梅森旋转算法的生成器

    std::uniform_int_distribution<int> distX(0, WIDTH / CELL_SIZE - 1);
    std::uniform_int_distribution<int> distY(0, HEIGHT / CELL_SIZE - 1);

    Menu menu;
    bool inMenu = true;

    sf::Clock clock;
    sf::Vector2i food(distX(gen), distY(gen));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (inMenu) {
                menu.draw_a(float(window.getSize().x), float(window.getSize().y));
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        menu.moveUp();
                    } else if (event.key.code == sf::Keyboard::Down) {
                        menu.moveDown();
                    } else if (event.key.code == sf::Keyboard::Enter) {
                        switch (menu.getPressedItem() + ge) {
                        case 0:
                            window.clear();
                            ge += 6;
                            menu.reset1();
                            // inMenu = false;
                            break;
                        case 1:
                            window.clear();
                            ge += 3;
                            menu.reset();
                            break;
                        case 2:
                            window.close();
                            break;
                        case 3:
                            inMenu = false;
                            break;
                        case 4:
                            snakeColor[0] = sf::Color::Blue;
                            inMenu = false;
                            break;
                        case 5:
                            snakeColor[0] = sf::Color::Yellow;
                            inMenu = false;
                            break;
                        case 6:
                            resetSnake(snake, 0);
                            inMenu = false;
                            break;
                        case 7:
                            num = 2;
                            resetSnake(snake, 0);
                            resetSnake(snake, 1);
                            inMenu = false;
                            break;
                        }
                    }
                }
            }
        }

        window.clear();
        if (inMenu) {
            menu.draw(window);
        } else {

            if (clock.getElapsedTime().asSeconds() > SPEED) {

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    if ((Myqueue[0].empty() && Myqueue[0].size() < 2 && snake.dir[0] != Down &&
                         snake.dir[0] != Up) ||
                        (Myqueue[0].front() != Down && Myqueue[0].size() == 1 &&
                         Myqueue[0].front() != Up)) {
                        Myqueue[0].push(Up);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    if ((Myqueue[0].empty() && Myqueue[0].size() < 2 && snake.dir[0] != Down &&
                         snake.dir[0] != Up) ||
                        (Myqueue[0].front() != Down && Myqueue[0].size() == 1 &&
                         Myqueue[0].front() != Up)) {
                        Myqueue[0].push(Down);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    if ((Myqueue[0].empty() && Myqueue[0].size() < 2 && snake.dir[0] != Left &&
                         snake.dir[0] != Right) ||
                        (Myqueue[0].front() != Left && Myqueue[0].size() == 1 &&
                         Myqueue[0].front() != Right)) {
                        Myqueue[0].push(Left);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    if ((Myqueue[0].empty() && Myqueue[0].size() < 2 && snake.dir[0] != Left &&
                         snake.dir[0] != Right) ||
                        (Myqueue[0].front() != Left && Myqueue[0].size() == 1 &&
                         Myqueue[0].front() != Right)) {
                        Myqueue[0].push(Right);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    if ((Myqueue[1].empty() && Myqueue[1].size() < 2 && snake.dir[1] != S &&
                         snake.dir[1] != W) ||
                        (Myqueue[1].front() != S && Myqueue[1].size() == 1 &&
                         Myqueue[1].front() != W)) {
                        Myqueue[1].push(W);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    if ((Myqueue[1].empty() && Myqueue[1].size() < 2 && snake.dir[1] != S &&
                         snake.dir[1] != W) ||
                        (Myqueue[1].front() != S && Myqueue[1].size() == 1 &&
                         Myqueue[1].front() != W)) {
                        Myqueue[1].push(S);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    if ((Myqueue[1].empty() && Myqueue[1].size() < 2 && snake.dir[1] != A &&
                         snake.dir[1] != D) ||
                        (Myqueue[1].front() != A && Myqueue[1].size() == 1 &&
                         Myqueue[1].front() != D)) {
                        Myqueue[1].push(A);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    if ((Myqueue[1].empty() && Myqueue[1].size() < 2 && snake.dir[1] != A &&
                         snake.dir[1] != D) ||
                        (Myqueue[1].front() != A && Myqueue[1].size() == 1 &&
                         Myqueue[1].front() != D)) {
                        Myqueue[1].push(D);
                    }
                }
                for (int i = 0; i <= num - 1; i++) {

                    if (!Myqueue[i].empty()) {
                        snake.dir[i] = Myqueue[i].front();
                        Myqueue[i].pop();
                    }
                }
                moveSnake(snake, num);
                if (num == 2) {
                    if (isCollision(snake, 2, 0) && isCollision(snake, 2, 1)) {
                        resetSnake(snake, 0);
                        resetSnake(snake, 1);
                    }
                }
                for (int i = 0; i <= num - 1; i++) {
                    if (isCollision(snake, num, i)) {
                        resetSnake(snake, i);
                    } else if (snake.body[i].front() == food) {
                        snake.body[i].push_back(snake.body[i].back());
                        std::uniform_int_distribution<int> distX(0, WIDTH / CELL_SIZE - 1);
                        std::uniform_int_distribution<int> distY(0, HEIGHT / CELL_SIZE - 1);

                        food = sf::Vector2i(distX(gen), distY(gen));
                    }
                    clock.restart();
                }
            }

            window.clear();
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            sf::RectangleShape wall(sf::Vector2f(CELL_SIZE * 40, CELL_SIZE));
            wall.setPosition(0, 600);
            wall.setFillColor(sf::Color::White);
            window.draw(wall);
            for (int i = 0; i <= num - 1; i++) {
                for (const auto &segment : snake.body[i]) {
                    cell.setPosition(float(segment.x) * CELL_SIZE, float(segment.y) * CELL_SIZE);
                    cell.setFillColor(snakeColor[i]);

                    window.draw(cell);
                }
            }
            cell.setPosition(float(food.x) * CELL_SIZE, float(food.y) * CELL_SIZE);
            cell.setFillColor(sf::Color::Red);
            window.draw(cell);
            for (int i = 0; i <= num - 1; i++) {
                size_t length = snake.body[i].size();
                std::vector<sf::Text> scoreItems;
                sf::Font font;
                if (!font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/Arial.ttf")) {
                }
                scoreItems.emplace_back(
                    "Player " + std::to_string(i + 1) + "'s score:" + std::to_string(length), font,
                    40);
                scoreItems[0].setPosition(float(20) + float((i)*400), 700);
                scoreItems[0].setFillColor(sf::Color::White);
                window.draw(scoreItems[0]);
                while (!scoreItems.empty()) {
                    scoreItems.pop_back();
                }
            }
        }

        window.display();
    }

    return 0;
}
