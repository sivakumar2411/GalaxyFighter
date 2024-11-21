#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<vector>
#include<cstdlib>
#include"Enemy.h"
using namespace std;
using namespace sf;


class Game {
private:

    RenderWindow* window;
    VideoMode videomode;
    Event event;
    Vector2f MousePos;
    RectangleShape LifeStats,ScoreStats,Start,Exit,backGround;

    float EnemySpawnTimer,EnemySpawnTimeMax;
    int points, maxEnemies,health,enemieRow;
    bool gameStats,isHome;

    vector<Enemy>Enemies;


    void initVariables();
    void initWindow();

public:

    Game();
    virtual ~Game();

    const bool running() const;

    void pollEvents();
    void update();
    void render();
    void renderSpaceShip(RectangleShape SS);
    void reStart();
    void spawnEnemy();
    void updateEnemy();
    void renderEnemy();
    void showHomePage();
    void showGameStats();

    void MousePressed();
    Vector2f MousePosition();
};

