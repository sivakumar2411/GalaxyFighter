#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<vector>
#include<cstdlib>
#include"Enemy.h"
#include"Bullet.h"
using namespace std;
using namespace sf;


class Game {
private:

    RenderWindow* window;
    VideoMode videomode;
    Event event;
    Vector2f MousePos;
    RectangleShape LifeStats, ScoreStats, Start, Exit, Resume, LevelRect;
    Sprite backGround;
    Texture backtexture,UserTexture,EnemyTexure,BulletTexture;
    Font font;
    Music bg_music, gameover_music;
    Sound fire_sound, hit_sound;
    SoundBuffer fs_buffer, hs_buffer;


    float EnemySpawnTimer,EnemySpawnTimeMax,levelTimer,level;
    int points, maxEnemies,health,enemyShots,enemieRow,Options; //Options 0 - Home , 1 - Game Replay , 2 - Pause
    bool gameStats,isLevel;

    vector<Enemy>Enemies;
    vector<Bullet>Bullets;
    vector<Texture>Explosion;
    SpaceF UserSS;

    void initVariables();
    void initWindow();
    void initBackGround();

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
    void showLevel();
    void initiateSpaceship();
    void UserActions();

    void Shoot(float velocity);
    void updateBullet();
    void BulletRendering();
    void MousePressed();
    Vector2f MousePosition();
};

