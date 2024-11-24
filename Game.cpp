#include"Game.h"
#include"SpaceF.h"
#include<vector>

Game::Game()
{
    this->initVariables();
    this->initWindow();
    this->initBackGround();
}

Game::~Game()
{
    if(this->window)
    delete this->window;
}

void Game::initVariables()
{
    this->window = nullptr;

    this->EnemySpawnTimeMax = 1000.0f;
    //this->maxEnemies = 25;
    this->Options = 0;
    if (!this->font.loadFromFile("arial.ttf")) {
        cout << "Error at Load UI font.\n";
    }
    if (!this->bg_music.openFromFile("Audios/Game_BG.mp3"))cout << "Error at Loading Background Music.\n";
    if(!this->gameover_music.openFromFile("Audios/Game Over.mp3"))cout << "Error at Loading Game Over Music.\n";
    if(!this->fs_buffer.loadFromFile("Audios/Fire.mp3"))cout << "Error at Loading Fire Sound.\n";
    this->fire_sound.setBuffer(this->fs_buffer);
    if(!this->hs_buffer.loadFromFile("Audios/Hit.mp3"))cout << "Error at Loading Fire Sound.\n";
    this->hit_sound.setBuffer(this->hs_buffer);
    if(!this->BulletTexture.loadFromFile("Images/Bullet.png")) cout << "Error at Loading Bullet Texture.\n";
    for (int i = 0;i < 10;i++) {
        Texture explode;
        if(!explode.loadFromFile("Images/Explosion/Explosion"+to_string(i+1) + ".png") ) cout<<"Error at Loading Explosion.\n";
        this->Explosion.push_back(explode);
    }
    this->UserSS.setExplosionFrame(0);
}

void Game::initBackGround() {
    if (!this->backtexture.loadFromFile("Space1.jpg")) {
        cout << "Error at Loading Image for BackGround.\n";
    }
    this->backGround.setTexture(this->backtexture);
    float scaleX = static_cast<float>(this->window->getSize().x) / this->backtexture.getSize().x;
    float scaleY = static_cast<float>(this->window->getSize().y) / this->backtexture.getSize().y;
    this->backGround.setScale(scaleX,scaleY);
    this->bg_music.play();
}

void Game::initWindow()
{
    this->videomode.height = 600;
    this->videomode.width = 800;

    this->window = new RenderWindow(this->videomode, "Game1", Style::Close);
    this->window->setFramerateLimit(120);
}

void Game::initiateSpaceship() {
    if (!this->UserTexture.loadFromFile("Images/User.png")) cout << "Error at Loading User Image.\n";
    if (!this->EnemyTexure.loadFromFile("Images/Enemy.png")) cout << "Error at Loading User Image.\n";
    this->UserSS.getSpaceShip().setTexture(this->UserTexture);
    Vector2u Scale = this->UserTexture.getSize();
    this->UserSS.getSpaceShip().setScale(50.f / static_cast<float>(Scale.x), 50.f / static_cast<float>(Scale.y));
    this->UserSS.getSpaceShip().setOrigin(0.f, 0.f);
    this->UserSS.getSpaceShip().setPosition(this->window->getSize().x/2.f - 25,this->window->getSize().y-50);
    //this->UserSS.setVelocity(0.f, 0.f);
}

void Game::UserActions() {

    Vector2f pos = this->UserSS.getSpaceShip().getPosition();
    float rightBoundary = this->window->getSize().x - this->UserSS.getSpaceShip().getGlobalBounds().width;

    if (Keyboard::isKeyPressed(Keyboard::Right) && pos.x < rightBoundary) {
        if(!this->UserSS.getIsExploding())
        this->UserSS.getSpaceShip().move(10.f, 0.f);
        //this->UserSS.getSpaceShip().setPosition(pos+this->UserSS.getVelocity());
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) && pos.x > 0) {
        if(!this->UserSS.getIsExploding())
        this->UserSS.getSpaceShip().move(-10.f, 0.f);
    }
    if (Keyboard::isKeyPressed(Keyboard::LControl)) {
        if(!this->UserSS.getIsExploding())
        this->Shoot(-5.f);
    }
}

void Game::showHomePage() {
    Text texts,texte,score,resume;

    if (this->Options == 2 || this->Options == 1) {
        RectangleShape FinalScore;
        FinalScore.setFillColor(Color::Transparent);
        FinalScore.setSize(Vector2f(150.f, 30.f));
        FinalScore.setPosition(320.f, this->Options == 1?200.f:150.f);

        
        score.setString("Your Score: " + to_string(this->points));
        score.setFont(this->font);
        score.setCharacterSize(20);
        score.setFillColor(Color::White);
        score.setOrigin(score.getGlobalBounds().left + score.getGlobalBounds().width / 2.f , score.getGlobalBounds().top + score.getGlobalBounds().height / 2.f);
        score.setPosition(FinalScore.getPosition().x + FinalScore.getSize().x / 2, FinalScore.getPosition().y + FinalScore.getSize().y / 2);
        
        this->window->draw(FinalScore);
        this->window->draw(score);
        if (this->Options == 2) {
            this->Resume.setFillColor(Color::Green);
            this->Resume.setSize(Vector2f(100.f, 30.f));
            this->Resume.setPosition(350.f, 200.f);

            resume.setString("Resume");
            resume.setFillColor(Color::White);
            resume.setFont(font);
            resume.setCharacterSize(20);
            resume.setOrigin(resume.getLocalBounds().left + resume.getGlobalBounds().width / 2, resume.getGlobalBounds().top + resume.getGlobalBounds().height / 2);
            resume.setPosition(this->Resume.getPosition().x + this->Resume.getSize().x / 2.f, this->Resume.getPosition().y + this->Resume.getSize().y / 2);
            this->window->draw(this->Resume);
            this->window->draw(resume);
        }
    }

    this->Start.setSize(Vector2f(100.f, 30.f));
    this->Start.setPosition(350.f, 250.f);
    this->Start.setFillColor(Color::Green);

    this->Exit.setSize(Vector2f(100.f, 30.f));
    this->Exit.setPosition(350.f, 300.f);
    this->Exit.setFillColor(Color::Red);

    texts.setFillColor(Color::White);
    texts.setString(this->Options == 0?"Start":"Restart");
    texts.setCharacterSize(20);
    texts.setFont(this->font);
    texts.setOrigin(texts.getGlobalBounds().left + texts.getGlobalBounds().width/2.f, texts.getGlobalBounds().top + texts.getGlobalBounds().height / 2.f);
    texts.setPosition(this->Start.getPosition().x + this->Start.getSize().x / 2,this->Start.getPosition().y + this->Exit.getSize().y / 2.f);

    texte.setString(this->Options == 0?"Exit":"Main menu");
    texte.setFillColor(Color::White);
    texte.setCharacterSize(20);
    texte.setFont(this->font);
    texte.setOrigin(texte.getGlobalBounds().left + texte.getGlobalBounds().width/2.f, texte.getGlobalBounds().top + texte.getGlobalBounds().height / 2.f);
    texte.setPosition(this->Exit.getPosition().x + this->Exit.getSize().x / 2.f,this->Exit.getPosition().y + this->Exit.getSize().y / 2.f);

    this->window->draw(this->Start);
    this->window->draw(this->Exit);
    this->window->draw(texts);
    this->window->draw(texte);
}

void Game::showLevel() {

    this->LevelRect.setFillColor(Color::Transparent);
    this->LevelRect.setSize(Vector2f(200.f, 50.f));
    this->LevelRect.setPosition(230.f, 240.f);

    Text ltext;
    ltext.setString("Level " + to_string(static_cast<int>(this->level)));
    ltext.setCharacterSize(50);
    ltext.setFillColor(Color::Blue);
    ltext.setOrigin(ltext.getGlobalBounds().left+ltext.getGlobalBounds().width/2.f,ltext.getGlobalBounds().top+ltext.getGlobalBounds().height/2.f);
    ltext.setPosition(this->LevelRect.getPosition().x + this->LevelRect.getSize().x / 2.f, this->LevelRect.getPosition().y + this->LevelRect.getSize().y / 2.f);
    ltext.setFont(this->font);

    this->window->draw(this->LevelRect);
    this->window->draw(ltext);
    this->levelTimer += 1 / 120.f;
    if (this->levelTimer >= 2.f) {
        this->isLevel = 0;
        this->levelTimer = 0.f;
    }
}

const bool Game::running() const {
    return this->window->isOpen();
}

void Game::spawnEnemy() {

    int n = this->enemieRow %2 == 0 ? 6 : 7;
    for (int i = 0;i < n;i++)
    {
        Enemy E;
        float x = 75.f;
        if (n == 6)
            x += 50.0f;
        E.getSpaceShip().setTexture(this->EnemyTexure);
        Vector2u Scale = this->EnemyTexure.getSize();
        E.getSpaceShip().setScale(50.f / static_cast<float>(Scale.x), 50.f / static_cast<float>(Scale.y));
        E.getSpaceShip().setOrigin(0.f, 0.f);
        E.getSpaceShip().setPosition(x + (i * 100.f),0.f);
        E.setExplosionFrame(0);
        E.setIsExploding(0);
        this->Enemies.push_back(E);
    }
    this->enemieRow += 1;
}

Vector2f Game::MousePosition() {
    return this->window->mapPixelToCoords(Mouse::getPosition(*window));
}

void Game::reStart() {
    this->Options = 3;
    this->health = 3;
    this->points = 0;
    this->gameStats = true;
    this->levelTimer = 0.f;
    this->level = 1;
    this->isLevel = true;
    this->enemyShots = 1;
    this->enemieRow = 1;
    this->EnemySpawnTimer = 500.f;
    this->Bullets.clear();
    this->Enemies.clear();
    this->initiateSpaceship();
}

void Game::MousePressed() {
    if (!this->gameStats){
        if (this->Start.getGlobalBounds().contains(this->MousePosition())) {
            this->reStart();
        }
        else if (this->Resume.getGlobalBounds().contains(this->MousePosition())) {
            this->Options = 3;
            this->gameStats = 1;
        }
        else if (this->Exit.getGlobalBounds().contains(this->MousePosition())) {
            if (this->Options == 0)
                this->window->close();
            else
                this->Options = 0;
        }
    }
}

void Game::Shoot(float velocity) {
    Bullet bullet = this->UserSS.getBullet(this->BulletTexture,velocity);
    bullet.getBullet().setPosition(this->UserSS.getSpaceShip().getPosition().x + this->UserSS.getSpaceShip().getGlobalBounds().width / 2, this->UserSS.getSpaceShip().getGlobalBounds().top);
    this->Bullets.push_back(bullet);
    this->fire_sound.play();
}

void Game::updateBullet() {
    bool nextLevel = 0;
    for (size_t i = 0;i < this->Bullets.size();i++) {
        this->Bullets[i].getBullet().move(0.f, Bullets[i].getBulletVelocity());
        if (this->Bullets[i].getBullet().getPosition().y <= 0 || this->Bullets[i].getBullet().getPosition().y >= this->window->getSize().y) {
            swap(this->Bullets[i], this->Bullets.back());
            this->Bullets.pop_back();
            --i;
            continue;
        }
        for (size_t j = 0;j < this->Enemies.size();j++) {
            if (this->Enemies[j].getSpaceShip().getGlobalBounds().intersects(this->Bullets[i].getBullet().getGlobalBounds())) {
                if (!this->Enemies[j].getIsExploding() && this->Enemies[j].getExplosionFrame() == 0) {
                    this->hit_sound.play();
                    this->points += 1;
                    if (this->points / (15 + (this->level * 5)) >= this->level) {
                        nextLevel = 1;
                    }
                    this->Enemies[j].setIsExploding(1);
                    this->Enemies[j].getExplodeClock().restart();


                    swap(this->Bullets[i], this->Bullets.back());
                    this->Bullets.pop_back();
                    --i;
                }
                break;
            }
        }
    }
    if (nextLevel) {
        this->level += 1;
        this->isLevel = 1;
        this->enemieRow = 1;
        this->Bullets.clear();
        this->enemyShots = (1+ (this->level/3 + 1));
        this->EnemySpawnTimer = max(500.f - (this->level * 50),0.f);
        this->Enemies.clear();
        nextLevel = 0;
    }
}

void Game::updateEnemy() {

    if (this->EnemySpawnTimer >= this->EnemySpawnTimeMax)
    {
        this->spawnEnemy();
        this->EnemySpawnTimer = 0.f;
    }
    else
        this->EnemySpawnTimer += this->level/3 * 4.f;

    for (int i = 0;i < this->Enemies.size();i++)
    {
        if (this->Enemies[i].getIsExploding()) {
            int etf = this->Enemies[i].getExplosionFrame();
            if (etf < this->Explosion.size()) {
                if (this->Enemies[i].getExplodeClock().getElapsedTime().asMilliseconds() > 50) {
                    this->Enemies[i].getSpaceShip().setTexture(this->Explosion[etf]);
                    this->Enemies[i].setExplosionFrame(etf += 1);
                    this->Enemies[i].getExplodeClock().restart();
                }
            }
            else this->Enemies[i].setIsExploding(0);
        }
        else if (!this->Enemies[i].getIsExploding() && this->Enemies[i].getExplosionFrame() != 0) {
            swap(this->Enemies[i], this->Enemies.back());
            this->Enemies.pop_back();
            --i;
        }
        else {
            this->Enemies[i].getSpaceShip().move(0, this->level / 4 * .6f);
            if (!this->Enemies[i].getIsExploding() && this->UserSS.getSpaceShip().getGlobalBounds().intersects(this->Enemies[i].getSpaceShip().getGlobalBounds())) {
                //this->Enemies.erase(this->Enemies.begin() + i);
                swap(this->Enemies[i], this->Enemies.back());
                this->Enemies.pop_back();
                this->health -= 1;
            }
            else if (this->Enemies[i].getSpaceShip().getGlobalBounds().top + this->Enemies[i].getSpaceShip().getGlobalBounds().height >= this->window->getSize().y) {
                swap(this->Enemies[i], this->Enemies.back());
                this->Enemies.pop_back();
            }
            if (this->health <= 0) {
                if (!this->UserSS.getIsExploding() && this->UserSS.getExplosionFrame() == 0) {
                    this->UserSS.setIsExploding(1);
                    this->UserSS.getExplodeClock().restart();
                }

                if (this->UserSS.getIsExploding()) {
                    if (this->UserSS.getExplosionFrame() < this->Explosion.size()) {
                        if (this->UserSS.getExplodeClock().getElapsedTime().asMilliseconds() > 150) {
                            this->UserSS.getSpaceShip().setTexture(this->Explosion[this->UserSS.getExplosionFrame()]);
                            this->UserSS.setExplosionFrame(this->UserSS.getExplosionFrame() + 1);
                            this->UserSS.getExplodeClock().restart();
                        }
                    }
                    else this->UserSS.setIsExploding(0);
                }
                else {
                    this->UserSS.setExplosionFrame(0);
                    this->gameover_music.play();
                    this->bg_music.play();
                    this->gameStats = 0;
                    this->Options = 1;
                }
            }
        }
    }
}

void Game::showGameStats() {

    Text lst, sst;
    this->LifeStats.setFillColor(Color::Transparent);
    this->LifeStats.setSize(Vector2f(100.f,30.f));
    this->LifeStats.setPosition(0.f, 0.f);

    lst.setFillColor(Color::White);
    lst.setCharacterSize(20);
    lst.setFont(this->font);
    lst.setString("Life: " + to_string(this->health));
    lst.setOrigin(lst.getGlobalBounds().left + lst.getGlobalBounds().width / 2 , lst.getGlobalBounds().top + lst.getGlobalBounds().height / 2);
    lst.setPosition(this->LifeStats.getPosition().x + this->LifeStats.getSize().x / 2, this->LifeStats.getPosition().y + this->LifeStats.getSize().y / 2);


    this->ScoreStats.setFillColor(Color::Transparent);
    this->ScoreStats.setSize(Vector2f(100.f,30.f));
    this->ScoreStats.setPosition(700.f, 0.f);

    sst.setFillColor(Color::White);
    sst.setCharacterSize(20);
    sst.setFont(this->font);
    sst.setString("Score: " + to_string(this->points));
    sst.setOrigin(sst.getGlobalBounds().left + sst.getGlobalBounds().width / 2, sst.getGlobalBounds().top + sst.getGlobalBounds().height / 2);
    sst.setPosition(this->ScoreStats.getPosition().x + this->ScoreStats.getSize().x / 2, this->ScoreStats.getPosition().y + this->ScoreStats.getSize().y / 2);


    this->window->draw(LifeStats);
    this->window->draw(ScoreStats);
    this->window->draw(lst);
    this->window->draw(sst);
}

void Game::BulletRendering() {
    for (auto e : this->Bullets) {
        this->window->draw(e.getBullet());
    }
}

void Game::renderEnemy() {

    for (auto& e : this->Enemies)
    {
        this->window->draw(e.getSpaceShip());
    }
}

void Game::pollEvents()
{
    while (this->window->pollEvent(this->event))
    {
        if (this->event.type == Event::Closed)
        {
            this->window->close();
            break;
        }
        else if (this->event.type == Event::MouseButtonPressed) {
            this->MousePressed();
        }
        else if(this->event.type == Event::KeyPressed){
            
            if (this->event.key.code == Keyboard::Escape) {
                if (this->Options == 2) {
                    this->Options = 3;
                    this->gameStats = 1;
                }
                else {
                    this->Options -= 1;
                    this->gameStats = 0;
                }
            }
            else if(!this->isLevel && this->gameStats)
            this->UserActions();
        }
    }
}

void Game::update() {
    this->pollEvents();

    if (this->gameStats) {
        this->updateEnemy();
        this->updateBullet();
    }
    //Update Mouse
    //cout << Mouse::getPosition(*window).x << " " << Mouse::getPosition(*window).y << endl;
}

void Game::renderSpaceShip(RectangleShape SS)
{
    this->window->clear(Color(0, 0, 0));
    this->window->draw(SS);
    this->window->display();
}

void Game::render() {
    this->window->clear(Color(0,0, 0));
    this->window->draw(this->backGround);
    if (this->gameStats) {
        if (this->isLevel)
            this->showLevel();
        else {
            this->showGameStats();
            this->window->draw(this->UserSS.getSpaceShip());
            this->BulletRendering();
            this->renderEnemy();
        }
    }
    else
        this->showHomePage();
    this->window->display();
}
