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
    RectangleShape LifeStats, ScoreStats, Start, ChangeSkin, Exit, Resume, LevelRect,SpaceShipSkin,BulletSkin,RightLabel,LeftLabel,Select;
    Sprite backGround;
    Texture backtexture,EnemyTexture,BulletTexture;
    Font font;
    Music bg_music, gameover_music;
    Sound fire_sound, hit_sound;
    SoundBuffer fs_buffer, hs_buffer;
    Clock EnemyShotCooldown,UserShotCD;

    vector<vector<Texture>> UserTexture;
    vector<vector<bool>>SSUnlocked,BSUnlocked;
    int VersionTracker, SkinTracker,SelectedVersion,SelectedSkin;


    float EnemySpawnTimer,EnemySpawnTimeMax,levelTimer,level;
    int points, maxEnemies,health,enemieRow,Options,cash; //Options 0 - Home , 1 - Game Replay , 2 - Pause , 5 - ChangeSkin
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
    void reStart();
    void spawnEnemy();
    void updateEnemy();
    void renderEnemy();
    void showHomePage();
    void showGameStats();
    void showLevel();
    void initiateSpaceship();
    void UserActions();

    void Shoot(float velocity,SpaceF E);
    void Closet();
    void updateBullet();
    void BulletRendering();
    void MousePressed();
    Vector2f MousePosition();
};

