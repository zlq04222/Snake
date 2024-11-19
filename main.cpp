#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
// #include <math.h>
#include <bits/ranges_algo.h>
#include <cstddef>
#include <deque>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <utility>

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

enum Direction { Up, Down, Left, Right };

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

int Countlength(const sf::Vector2i head, const sf::Vector2i food) {

    return abs(head.x - food.x) + abs(head.y - food.y);
}
bool orCollision(const Snake &snake, int num, int snake_index, const sf::Vector2i head) {

    if (head.x < 0 && head.x >= WIDTH / CELL_SIZE && head.y < 0 && head.y >= HEIGHT / CELL_SIZE)
        return false;
    for (int j = 0; j <= num - 1; j++) {
        if (j == snake_index) {
            for (size_t i = 1; i < snake.body[j].size(); ++i) {
                if (snake.body[j][i] == head)
                    return false;
            }
        }
        if (j != snake_index) {
            for (const auto &body : snake.body[j]) {
                if (body == head)
                    return false;
            }
        }
    }

    return true;
}
bool oorCollision(Snake snake, int num, int snake_index, sf::Vector2i head, sf::Vector2i head1) {

    if (head.x < 0 || head.x >= WIDTH / CELL_SIZE || head.y < 0 || head.y >= HEIGHT / CELL_SIZE ||
        head == head1)
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
Direction dir_find(Snake &snake, Direction snake1, Direction snake2, sf::Vector2i food) {
    sf::Vector2i head1 = snake.body[0].front();
    sf::Vector2i head2 = snake.body[1].front();
    sf::Vector2i headd = snake.body[1].front();
    sf::Vector2i headUp = head2;
    headUp.y--;
    sf::Vector2i headDown = head2;
    headDown.y++;
    sf::Vector2i headRight = head2;
    headRight.x++;
    sf::Vector2i headLeft = head2;
    headLeft.x--;
    Direction x = snake2;
    int j = 0;

    switch (snake1) {
    case Up:
        head1.y--;
        break;
    case Down:
        head1.y++;
        break;
    case Right:
        head1.x++;
        break;
    case Left:
        head1.x--;
        break;
    }
    switch (snake2) {
    case Up:
        headd.y--;
        break;
    case Down:
        headd.y++;
        break;
    case Right:
        headd.x++;
        break;
    case Left:
        headd.x--;
        break;
    }
    int dis = Countlength(headd, food);

    if (snake2 == Up) {
        const sf::Vector2i head[3] = {headUp, headLeft, headRight};
        const Direction dir[3] = {Up, Left, Right};

        for (int i = 0; i < 3; i++) {
            if (orCollision(snake, 2, 1, head[i]) && head[i] != head1) {

                if (Countlength(head[i], food) <= dis || oorCollision(snake, 2, 1, headd, head1)) {

                    dis = Countlength(head[i], food);
                    x = dir[i];

                    if (j && dis >= Countlength(head[i - 1], food) &&
                        orCollision(snake, 2, 1, head[i - 1]) && head[i - 1] != head1) {
                        dis = Countlength(head[i - 1], food);
                        x = dir[i - 1];
                    }
                    j = 1;
                }
            }
        }

    } else if (snake2 == Down) {
        const sf::Vector2i head2[3] = {headDown, headLeft, headRight};
        const Direction dir2[3] = {Down, Left, Right};

        for (int i = 0; i < 3; i++) {
            if (orCollision(snake, 2, 1, head2[i]) && head2[i] != head1) {

                if (Countlength(head2[i], food) <= dis || oorCollision(snake, 2, 1, headd, head1)) {

                    dis = Countlength(head2[i], food);
                    x = dir2[i];
                    if (j && dis >= Countlength(head2[i - 1], food) &&
                        orCollision(snake, 2, 1, head2[i - 1]) && head2[i - 1] != head1) {
                        dis = Countlength(head2[i - 1], food);
                        x = dir2[i - 1];
                    }
                    j = 1;
                }
            }
        }
    } else if (snake2 == Right) {
        const sf::Vector2i head2[3] = {headRight, headDown, headUp};
        const Direction dir2[3] = {Right, Down, Up};

        for (int i = 0; i < 3; i++) {
            if (orCollision(snake, 2, 1, head2[i]) && head2[i] != head1) {

                if (Countlength(head2[i], food) <= dis || oorCollision(snake, 2, 1, headd, head1)) {
                    dis = Countlength(head2[i], food);
                    x = dir2[i];
                    if (j && dis >= Countlength(head2[i - 1], food) &&
                        orCollision(snake, 2, 1, head2[i - 1]) && head2[i - 1] != head1) {
                        dis = Countlength(head2[i - 1], food);
                        x = dir2[i - 1];
                    }
                    j = 1;
                }
            }
        }
    }

    else if (snake2 == Left) {
        const sf::Vector2i head2[3] = {headLeft, headDown, headUp};
        const Direction dir2[3] = {Left, Down, Up};

        for (int i = 0; i < 3; i++) {
            if (orCollision(snake, 2, 1, head2[i]) && head2[i] != head1) {

                if (Countlength(head2[i], food) <= dis || oorCollision(snake, 2, 1, headd, head1)) {
                    dis = Countlength(head2[i], food);
                    x = dir2[i];

                    if (j && dis >= Countlength(head2[i - 1], food) &&
                        orCollision(snake, 2, 1, head2[i - 1]) && head2[i - 1] != head1) {
                        dis = Countlength(head2[i - 1], food);
                        x = dir2[i - 1];
                    }
                    j = 1;
                }
            }
        }
    }
    return x;
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
bool In_closedlist(std::deque<sf::Vector2i> closedlist, sf::Vector2i node) {
    if (node.x < 0 || node.x >= WIDTH / CELL_SIZE || node.y < 0 || node.y >= HEIGHT / CELL_SIZE)
        return true;
    while (!closedlist.empty()) {
        sf::Vector2i node_1 = closedlist.back();
        if (node_1 == node) {
            return true;
        }
        closedlist.pop_back();
    }
    return false;
}
struct PS {
    sf::Vector2i location;
    int score;
};
class Node {
public:
    sf::Vector2i location;
    std::shared_ptr<Node> next;
    int score;

    // explicit Node(sf::Vector2i s, int x, int y) : location(s), score(x), id(y) {}
    // struct Compare {
    //     bool operator()(const Node &t1, const Node &t2) {
    //         // 返回 true 表示 t1 的优先级低于 t2，故 t1 应该在 t2 后面
    //         if (t1.score == t2.score) {
    //             return t1.id < t2.id;
    //         }
    //         return t1.score >= t2.score; // 最x堆
    //     }
    // };
};
std::deque<sf::Vector2i>
Extend(Snake &snake, sf::Vector2i food, std::deque<sf::Vector2i> &closedlist) {
    sf::Vector2i head = snake.body[1].front();

    sf::Vector2i temp;
    std::deque<sf::Vector2i> d;
    int sum = 4;
    sf::Vector2i sun_xp = food;
    sf::Vector2i sun_xc = food;
    sf::Vector2i sun_yp = food;
    sf::Vector2i sun_yc = food;
    sun_xp.x++;
    sun_xc.x--;
    sun_yp.y++;
    sun_yc.y--;
    sf::Vector2i sun_node[4] = {sun_xp, sun_xc, sun_yp, sun_yc};
    if (In_closedlist(closedlist, sun_xp)) {
        sun_node[0] = head;
        sum = sum - 1;
    }
    if (In_closedlist(closedlist, sun_xc)) {
        sun_node[1] = head;
        sum = sum - 1;
    }
    if (In_closedlist(closedlist, sun_yp)) {
        sun_node[2] = head;
        sum = sum - 1;
    }
    if (In_closedlist(closedlist, sun_yc)) {
        sun_node[3] = head;
        sum = sum - 1;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 1; j < 4; j++) {
            if (Countlength(sun_node[j], head) >= Countlength(sun_node[j - 1], head)) {
                temp = sun_node[j];
                sun_node[j] = sun_node[j - 1];
                sun_node[j - 1] = temp;
            }
        }
    }
    // std::cout << sun_node[0].y << std::endl;

    for (int i = 0; i < sum; i++) {
        d.push_front(sun_node[i]);
    }

    return d;
}
// Direction Find_Direction(Snake &snake, sf::Vector2i food, Direction dir[]) {
//     sf::Vector2i head1 = snake.body[0].front();
//     std::deque<sf::Vector2i> closedlist;
//     int father_index = 0;
//     Node *path_clone[1000];

//     for (auto body : snake.body[0]) {
//         closedlist.push_back(body);
//     }
//     for (size_t i = 1; i < snake.body[1].size(); ++i) {
//         closedlist.push_back(snake.body[1][i]);
//     }

//     switch (dir[0]) {
//     case Up:
//         head1.y--;
//         break;
//     case Down:
//         head1.y++;
//         break;
//     case Right:
//         head1.x++;
//         break;
//     case Left:
//         head1.x--;
//         break;
//     }
//     sf::Vector2i head = snake.body[1].front();
//     std::deque<sf::Vector2i> openlist;

//     openlist.push_front(food);
//     closedlist.push_front(food);
//     closedlist.push_front(head1);
//     path_clone[0] = new Node;
//     path_clone[0]->location = food;
//     path_clone[0]->next = nullptr;
//     while (!openlist.empty() && openlist.back() != head) {

//         sf::Vector2i new_head = openlist.back();
//         int start_index = father_index;
//         openlist.pop_back();
//         std::deque<sf::Vector2i> openlist_c;
//         openlist_c = Extend(snake, new_head, head1, closedlist);
//         while (!openlist_c.empty()) {
//             path_clone[++father_index] = new Node;
//             path_clone[father_index]->location = openlist_c.back();
//             path_clone[father_index]->next = path_clone[start_index];
//             openlist.push_back(openlist_c.back());
//             closedlist.push_back(openlist_c.back());
//             openlist_c.pop_back();
//         }
//     }

//     if (openlist.empty()) {
//         return dir[1];
//     }
//     sf::Vector2i next_step = path_clone[father_index]->location;
//     if (next_step.x - head.x > 0.5)
//         return Right;
//     else if (next_step.x - head.x < -0.5)
//         return Left;
//     else if (next_step.y - head.y > 0.5)
//         return Down;
//     else
//         return Up;
// }
std::deque<PS> picks(std::deque<PS> openlist, int size) {
    PS temp;
    for (int i = 0; i < size; i++) {
        for (int j = 1; j < size; j++) {
            if (openlist[j].score >= openlist[j - 1].score) {
                temp = openlist[j - 1];
                openlist[j - 1] = openlist[j];
                openlist[j] = temp;
            }
        }
    }
    return openlist;
}
Direction find_Direction(Snake &snake, sf::Vector2i &food, Direction dir[]) {
    // 确保路径克隆使用智能指针
    std::shared_ptr<Node> path_clone[1000];
    int step = 0;
    sf::Vector2i head1 = snake.body[0].front();
    std::deque<sf::Vector2i> closedlist;

    // 将蛇的身体添加到 closedlist
    for (const auto &body : snake.body[0]) {
        closedlist.push_back(body);
    }
    for (size_t i = 1; i < snake.body[1].size(); ++i) {
        closedlist.push_back(snake.body[1][i]);
    }

    // 根据方向更新头部位置
    switch (dir[0]) {
    case Up:
        head1.y--;
        break;
    case Down:
        head1.y++;
        break;
    case Right:
        head1.x++;
        break;
    case Left:
        head1.x--;
        break;
    }

    sf::Vector2i head = snake.body[1].front();
    std::deque<sf::Vector2i> openlist;

    openlist.push_front(food);

    closedlist.push_front(food);
    closedlist.push_front(head1);

    path_clone[0] = std::make_shared<Node>(); // 使用智能指针
    path_clone[0]->location = food;
    path_clone[0]->next = nullptr;
    path_clone[0]->score = Countlength(food, head);
    std::deque<sf::Vector2i> openlist_c;

    int father_index = 0;

    while (!openlist.empty() && openlist.back() != head) {
        std::cout << food.x << food.y << std::endl;
        std::cout << (openlist).size() << std::endl;
        std::cout << head.x << head.y << std::endl;
        int start_index = father_index;
        step += 1;

        sf::Vector2i new_head = openlist.back();

        std::cout << new_head.x << " " << new_head.y << std::endl;

        openlist.pop_back();

        openlist_c = Extend(snake, new_head, closedlist);

        while (!openlist_c.empty()) {
            // std::cout << father_index << std::endl;

            if (father_index < 999) { // 防止数组越界
                father_index++;
                path_clone[father_index] = std::make_shared<Node>();
                path_clone[father_index]->location = openlist_c.back();
                path_clone[father_index]->next = path_clone[start_index];
                path_clone[father_index]->score = step + Countlength(openlist_c.back(), head);
                // path_clone[start_index].get(); // 使用 get() 获取原始指针
                openlist.push_back(openlist_c.back());
                closedlist.push_back(openlist_c.back());
            }
            openlist_c.pop_back();
        }
    }
    if (openlist.empty()) {
        return dir_find(snake, dir[0], dir[1], food);
    }
    auto current = path_clone[father_index];
    current = current->next;
    sf::Vector2i next_step = current->location;
    std::cout << int(next_step.x) << int(next_step.y) << std::endl;
    std::cout << int(next_step.x - head.x) << std::endl;
    if (next_step.x > head.x)
        return Right;
    else if (next_step.x < head.x)
        return Left;
    else if (next_step.y > head.y)
        return Down;
    else
        return Up; // 作为示例返回一个默认方向，实际中应根据逻辑返回合适的方向
}
void moveSnake(Snake &snake, int num) {

    sf::Vector2i newHead[2];

    for (int i = 0; i <= num - 1; i++) {
        newHead[i] = snake.body[i].front();
        Direction clone_dir;
        clone_dir = snake.dir[i];
        // if (snake.dir[i] == W) {
        //     clone_dir = Up;
        // }
        // if (snake.dir[i] == A) {
        //     clone_dir = Left;
        // }
        // if (snake.dir[i] == D) {
        //     clone_dir = Right;
        // }
        // if (snake.dir[i] == S) {
        //     clone_dir = Down;
        // }

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
bool foodd(sf::Vector2i food, Snake &snake, int num) {
    for (int i = 0; i <= num - 1; i++) {
        for (const auto &body : snake.body[i]) {
            if (body == food)
                return true;
        }
    }
    return false;
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
                // if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                //     if ((Myqueue[1].empty() && Myqueue[1].size() < 2 && snake.dir[1] != S &&
                //          snake.dir[1] != W) ||
                //         (Myqueue[1].front() != S && Myqueue[1].size() == 1 &&
                //          Myqueue[1].front() != W)) {
                //         Myqueue[1].push(W);
                //     }
                // }
                // if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                //     if ((Myqueue[1].empty() && Myqueue[1].size() < 2 && snake.dir[1] != S &&
                //          snake.dir[1] != W) ||
                //         (Myqueue[1].front() != S && Myqueue[1].size() == 1 &&
                //          Myqueue[1].front() != W)) {
                //         Myqueue[1].push(S);
                //     }
                // }
                // if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                //     if ((Myqueue[1].empty() && Myqueue[1].size() < 2 && snake.dir[1] != A &&
                //          snake.dir[1] != D) ||
                //         (Myqueue[1].front() != A && Myqueue[1].size() == 1 &&
                //          Myqueue[1].front() != D)) {
                //         Myqueue[1].push(A);
                //     }
                // }
                // if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                //     if ((Myqueue[1].empty() && Myqueue[1].size() < 2 && snake.dir[1] != A &&
                //          snake.dir[1] != D) ||
                //         (Myqueue[1].front() != A && Myqueue[1].size() == 1 &&
                //          Myqueue[1].front() != D)) {
                //         Myqueue[1].push(D);
                //     }
                // }
                if (num == 2) {

                    Direction now = find_Direction(
                        snake, food,
                        snake.dir); // dir_find(snake, snake.dir[0], snake.dir[1], food);
                    if (!Myqueue[0].empty()) {
                        snake.dir[0] = Myqueue[0].front();
                        now = find_Direction(snake, food, snake.dir);
                    }
                    if (snake.dir[1] != now) {
                        Myqueue[1].push(now);
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
                        while (foodd(food, snake, num)) {
                            food = sf::Vector2i(distX(gen), distY(gen));
                        }
                    }
                }
                clock.restart();
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
