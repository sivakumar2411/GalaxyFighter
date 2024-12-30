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
    this->cash = 0;
    if (!this->font.loadFromFile("Tamil.ttf")) {
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
    this->SelectedSkin = 0;
    this->SelectedVersion = 0;
    this->UserTexture.resize(11,vector<Texture>(2));
    for (int i = 0;i < 11;i++) {
        Texture utexture;
        if (!utexture.loadFromFile("Images/User/" + to_string(i+1) + ".png")) cout << "Error at Loading User Image.\n";
        this->UserTexture[i][0] = utexture;
        if (!utexture.loadFromFile("Images/User/" + to_string(i+1) + "B.png")) cout << "Error at Loading User Image.\n";
        this->UserTexture[i][1] = utexture;
    }
    this->SSUnlocked.resize(this->UserTexture.size(), vector<bool>(2, 0));
    this->SSUnlocked[0][0] = 1;
}

void Game::initBackGround() {
    if (!this->backtexture.loadFromFile("Images/BGScenary/Space_BG.png")) {
        cout << "Error at Loading Image for BackGround.\n";
    }
    //this->backtexture.setRepeated(1);
    this->backGround.setTexture(this->backtexture);
    //this->backGround.setTextureRect(IntRect(0,0,this->window->getSize().x, this->window->getSize().y));
    float scaleX = static_cast<float>(this->window->getSize().x) / this->backtexture.getSize().x;
    float scaleY = static_cast<float>(this->window->getSize().y) / this->backtexture.getSize().y;
    this->backGround.setScale(scaleX,scaleY);
    this->bg_music.setLoop(true);
    this->bg_music.play();
}

void Game::initWindow()
{
    this->videomode.height = 1080;
    this->videomode.width = 1920;

    this->window = new RenderWindow(this->videomode, "Game1", Style::Fullscreen);
    this->window->setFramerateLimit(60);
}

void Game::initiateSpaceship() {
    if (!this->EnemyTexture.loadFromFile("Images/Enemy.png")) cout << "Error at Loading User Image.\n";
    if (this->UserTexture.size() != 0) {
        this->UserSS.getSpaceShip().setTexture(this->UserTexture[this->SelectedVersion][this->SelectedSkin]);
        Vector2u Scale = this->UserTexture[this->SelectedVersion][this->SelectedSkin].getSize();
        this->UserSS.getSpaceShip().setScale(100.f / static_cast<float>(Scale.x), 100.f / static_cast<float>(Scale.y));
        this->UserSS.getSpaceShip().setOrigin(0.f, 0.f);
        this->UserSS.getSpaceShip().setPosition(this->window->getSize().x / 2.f - 50, this->window->getSize().y - 100);
    }
    this->UserSS.setExplosionFrame(0);
    this->UserSS.setIsExploding(0);
    //this->UserSS.setVelocity(0.f, 0.f);
}

void Game::UserActions() {

    Vector2f pos = this->UserSS.getSpaceShip().getPosition();
    float rightBoundary = this->window->getSize().x - this->UserSS.getSpaceShip().getGlobalBounds().width;
    float bottomBoudary = this->window->getSize().y - this->UserSS.getSpaceShip().getGlobalBounds().height;

    if (Keyboard::isKeyPressed(Keyboard::Right) && pos.x < rightBoundary) {
        if(!this->UserSS.getIsExploding())
        this->UserSS.getSpaceShip().move(10.f, 0.f);
        //this->UserSS.getSpaceShip().setPosition(pos+this->UserSS.getVelocity());
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left) && pos.x > 0) {
        if(!this->UserSS.getIsExploding())
        this->UserSS.getSpaceShip().move(-10.f, 0.f);
    }
    if (Keyboard::isKeyPressed(Keyboard::Up) && pos.y > 0) {
        if(!this->UserSS.getIsExploding())
        this->UserSS.getSpaceShip().move(0.f,-10.f);
        //this->UserSS.getSpaceShip().setPosition(pos+this->UserSS.getVelocity());
    }
    else if (Keyboard::isKeyPressed(Keyboard::Down) && pos.y < bottomBoudary) {
        if(!this->UserSS.getIsExploding())
        this->UserSS.getSpaceShip().move(0.f,10.f);
    }
    if (Keyboard::isKeyPressed(Keyboard::LControl)) {
        if (!this->UserSS.getIsExploding() && this->UserShotCD.getElapsedTime().asMilliseconds() >= 150) {
            this->Shoot(-15.f, this->UserSS);
            this->UserShotCD.restart();
        }
    }
}

void Game::showHomePage() {
    Text texts, texte, textcs, score, resume;
    Vector2f ScreenSize = Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y));

    if (this->Options == 2 || this->Options == 1) {
        RectangleShape FinalScore;
        FinalScore.setFillColor(Color::Transparent);
        FinalScore.setSize(Vector2f(max(150.f, ScreenSize.x / 15.f), max(30.f, ScreenSize.y / 15.f)));
        FinalScore.setPosition((ScreenSize.x/2.f)-(FinalScore.getSize().x/2.f), this->Options == 1 ? 200.f : 150.f);


        score.setString("Your Score: " + to_string(this->points));
        score.setFont(this->font);
        score.setCharacterSize(25);
        score.setFillColor(Color::White);
        score.setOrigin(score.getGlobalBounds().left + score.getGlobalBounds().width / 2.f, score.getGlobalBounds().top + score.getGlobalBounds().height / 2.f);
        score.setPosition(FinalScore.getPosition().x + FinalScore.getSize().x / 2, FinalScore.getPosition().y + FinalScore.getSize().y / 2);

        this->window->draw(FinalScore);
        this->window->draw(score);
        if (this->Options == 2) {
            this->Resume.setFillColor(Color::Green);
            this->Resume.setSize(Vector2f(max(150.f, ScreenSize.x / 15.f), max(30.f, ScreenSize.y / 15.f)));
            this->Resume.setPosition((ScreenSize.x / 2.f)-(this->Resume.getSize().x/2.f), 200.f);

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
    if (this->Options == 0 || this->Options == 1 || this->Options == 2) {
        
        this->Start.setSize(Vector2f(max(150.f, ScreenSize.x / 10.f), max(30.f, ScreenSize.y / 15.f)));
        this->Start.setPosition((ScreenSize.x / 2.f)-(this->Start.getSize().x/2.f), this->Options == 0 ? 200.f : 250.f);
        this->Start.setFillColor(Color::Green);

        this->ChangeSkin.setSize(Vector2f(max(150.f, ScreenSize.x / 10.f), max(30.f, ScreenSize.y / 15.f)));
        this->ChangeSkin.setPosition((ScreenSize.x / 2.f)-(this->ChangeSkin.getSize().x/2.f), 250.f);
        this->ChangeSkin.setFillColor(Color::Blue);

        this->Exit.setSize(Vector2f(max(150.f, ScreenSize.x / 10.f), max(30.f, ScreenSize.y / 15.f)));
        this->Exit.setPosition((ScreenSize.x / 2.f)-(this->Exit.getSize().x/2.f), 300.f);
        this->Exit.setFillColor(Color::Red);

        texts.setFillColor(Color::White);
        texts.setString(this->Options == 0 ? "Start" : "Restart");
        texts.setCharacterSize(20);
        texts.setFont(this->font);
        texts.setOrigin(texts.getGlobalBounds().left + texts.getGlobalBounds().width / 2.f, texts.getGlobalBounds().top + texts.getGlobalBounds().height / 2.f);
        texts.setPosition(this->Start.getPosition().x + this->Start.getSize().x / 2, this->Start.getPosition().y + this->Start.getSize().y / 2.f);

        textcs.setFillColor(Color::White);
        textcs.setString("Store");
        textcs.setCharacterSize(20);
        textcs.setFont(this->font);
        textcs.setOrigin(textcs.getGlobalBounds().left + textcs.getGlobalBounds().width / 2.f, textcs.getGlobalBounds().top + textcs.getGlobalBounds().height / 2.f);
        textcs.setPosition(this->ChangeSkin.getPosition().x + this->ChangeSkin.getSize().x / 2, this->ChangeSkin.getPosition().y + this->ChangeSkin.getSize().y / 2.f);

        texte.setString(this->Options == 0 ? "Exit" : "Main menu");
        texte.setFillColor(Color::White);
        texte.setCharacterSize(20);
        texte.setFont(this->font);
        texte.setOrigin(texte.getGlobalBounds().left + texte.getGlobalBounds().width / 2.f, texte.getGlobalBounds().top + texte.getGlobalBounds().height / 2.f);
        texte.setPosition(this->Exit.getPosition().x + this->Exit.getSize().x / 2.f, this->Exit.getPosition().y + this->Exit.getSize().y / 2.f);

        this->window->draw(this->Start);
        this->window->draw(this->Exit);
        this->window->draw(texts);
        this->window->draw(texte);
        if (this->Options == 0) {
            this->window->draw(this->ChangeSkin);
            this->window->draw(textcs);
        }
    }
    if (this->Options > 4) {
        this->SpaceShipSkin.setSize(Vector2f(max(200.f, ScreenSize.x / 20.f), max(30.f, ScreenSize.y / 20.f)));
        this->SpaceShipSkin.setPosition(50.f, 100.f);
        this->SpaceShipSkin.setFillColor(Color::Blue);

        this->BulletSkin.setSize(Vector2f(max(200.f, ScreenSize.x / 20.f), max(30.f, ScreenSize.y / 20.f)));
        this->BulletSkin.setPosition(50.f, 150.f);
        this->BulletSkin.setFillColor(Color::Blue);

        textcs.setFillColor(Color::White);
        textcs.setString("SpaceShip");
        textcs.setCharacterSize(20);
        textcs.setFont(this->font);
        textcs.setOrigin(textcs.getGlobalBounds().left + textcs.getGlobalBounds().width / 2.f, textcs.getGlobalBounds().top + textcs.getGlobalBounds().height / 2.f);
        textcs.setPosition(this->SpaceShipSkin.getPosition().x + this->SpaceShipSkin.getSize().x / 2, this->SpaceShipSkin.getPosition().y + this->SpaceShipSkin.getSize().y / 2.f);

        texts.setFillColor(Color::White);
        texts.setString("Ammo");
        texts.setCharacterSize(20);
        texts.setFont(this->font);
        texts.setOrigin(texts.getGlobalBounds().left + texts.getGlobalBounds().width / 2.f, texts.getGlobalBounds().top + texts.getGlobalBounds().height / 2.f);
        texts.setPosition(this->BulletSkin.getPosition().x + this->BulletSkin.getSize().x / 2, this->BulletSkin.getPosition().y + this->BulletSkin.getSize().y / 2.f);

        this->window->draw(this->SpaceShipSkin);
        this->window->draw(textcs);
        this->window->draw(this->BulletSkin);
        this->window->draw(texts);
        this->Closet();
    }
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

void Game::Closet() {
    RectangleShape NameLabel,CoinLabel;
    Text nt,rt,lt,st,coin,tcoin;
    Sprite Show;

    nt.setFont(this->font);
    nt.setString("Space_Ship " + to_string(this->VersionTracker + 1));
    nt.setFillColor(Color::Black);
    nt.setCharacterSize(25);
    nt.setOrigin(nt.getGlobalBounds().left + nt.getGlobalBounds().width/2.f,nt.getGlobalBounds().top+nt.getGlobalBounds().height/2.f);

    rt.setFont(this->font);
    rt.setString("->");
    rt.setFillColor(Color::Black);
    rt.setCharacterSize(25);
    rt.setOrigin(rt.getGlobalBounds().left + rt.getGlobalBounds().width / 2.f, rt.getGlobalBounds().top + rt.getGlobalBounds().height / 2.f);

    lt.setFont(this->font);
    lt.setFillColor(Color::Black);
    lt.setCharacterSize(25);
    lt.setString("<-");
    lt.setOrigin(lt.getGlobalBounds().left + lt.getGlobalBounds().width / 2.f, lt.getGlobalBounds().top + lt.getGlobalBounds().height / 2.f);

    st.setFont(this->font);
    st.setFillColor(Color::Black);
    st.setCharacterSize(25);
    st.setString(this->SSUnlocked[this->VersionTracker][this->SkinTracker] ? (this->VersionTracker == this->SelectedVersion && this->SelectedSkin == this->SkinTracker) ? "Selected" : "Select" : "Unlock");
    st.setOrigin(st.getGlobalBounds().left + st.getGlobalBounds().width / 2.f, st.getGlobalBounds().top + st.getGlobalBounds().height / 2.f);

    coin.setFont(this->font);
    coin.setFillColor(Color::White);
    coin.setCharacterSize(30);
    coin.setString(to_string(this->VersionTracker*100+(this->SkinTracker*50)));
    coin.setOrigin(coin.getGlobalBounds().left + coin.getGlobalBounds().width / 2.f, coin.getGlobalBounds().top + coin.getGlobalBounds().height / 2.f);

    tcoin.setFont(this->font);
    tcoin.setFillColor(Color::Black);
    tcoin.setCharacterSize(20);
    tcoin.setString(to_string(this->cash));
    tcoin.setOrigin(tcoin.getGlobalBounds().left + tcoin.getGlobalBounds().width / 2.f, tcoin.getGlobalBounds().top + tcoin.getGlobalBounds().height / 2.f);

    NameLabel.setSize(Vector2f(300.f, 30.f));
    NameLabel.setPosition(400.f, 500.f);
    NameLabel.setFillColor(Color::Yellow);
    nt.setPosition(NameLabel.getPosition().x + NameLabel.getSize().x / 2.f, NameLabel.getPosition().y + NameLabel.getSize().y / 2.f);

    Show.setTexture(this->UserTexture[this->VersionTracker][this->SkinTracker]);
    Vector2u Scale = this->UserTexture[this->VersionTracker][this->SkinTracker].getSize();
    Show.setScale(300.f/Scale.x,300.f/Scale.y);
    Show.setPosition(400.f,100.f);
    if(!this->SSUnlocked[this->VersionTracker][this->SkinTracker])
    Show.setColor(Color::Black);
    coin.setPosition(Show.getGlobalBounds().left + Show.getGlobalBounds().width / 2.f, Show.getGlobalBounds().top + Show.getGlobalBounds().height / 2.f);

    this->RightLabel.setSize(Vector2f(30.f, 30.f));
    this->RightLabel.setPosition(720.f, 500.f);
    this->RightLabel.setFillColor(Color::Yellow);
    rt.setPosition(this->RightLabel.getPosition().x + this->RightLabel.getSize().x / 2.f, this->RightLabel.getPosition().y + this->RightLabel.getSize().y / 2.f);

    this->LeftLabel.setSize(Vector2f(30.f, 30.f));
    this->LeftLabel.setPosition(350.f, 500.f);
    this->LeftLabel.setFillColor(Color::Yellow);
    lt.setPosition(this->LeftLabel.getPosition().x + this->LeftLabel.getSize().x / 2.f, this->LeftLabel.getPosition().y + this->LeftLabel.getSize().y / 2.f);

    this->Select.setSize(Vector2f(200.f,30.f));
    this->Select.setPosition(450.f, 450.f);
    this->Select.setFillColor(Color::Yellow);
    st.setPosition(this->Select.getPosition().x + this->Select.getSize().x / 2.f, this->Select.getPosition().y + this->Select.getSize().y / 2.f);

    CoinLabel.setFillColor(Color::Yellow);
    CoinLabel.setSize(Vector2f(50.f,30.f));
    CoinLabel.setPosition(700.f,50.f);
    tcoin.setPosition(CoinLabel.getPosition().x + CoinLabel.getSize().x / 2.f, CoinLabel.getPosition().y + CoinLabel.getSize().y / 2.f);


    this->window->draw(NameLabel);
    this->window->draw(nt);
    this->window->draw(this->Select);
    this->window->draw(st);
    this->window->draw(Show);
    this->window->draw(CoinLabel);
    this->window->draw(tcoin);
    if (!this->SSUnlocked[this->VersionTracker][this->SkinTracker]) {
        this->window->draw(coin);
    }
    if (this->VersionTracker < this->UserTexture.size() - 1) {
        this->window->draw(this->RightLabel);
        this->window->draw(rt);
    }
    if (this->VersionTracker > 0) {
        this->window->draw(this->LeftLabel);
        this->window->draw(lt);
    }
}

const bool Game::running() const {
    return this->window->isOpen();
}

void Game::spawnEnemy() {

    float ScreenSizeX = static_cast<float>(this->window->getSize().x) / 15.f;

    int n = this->enemieRow %2 == 0 ? 6 : 7;
    for (int i = 0;i < n;i++)
    {
        Enemy E;
        float x = ScreenSizeX * (((i + 1) * 2) + (n == 6?0:-1));
        E.getSpaceShip().setTexture(this->EnemyTexture);
        Vector2u Scale = this->EnemyTexture.getSize();
        E.getSpaceShip().setScale(100.f / static_cast<float>(Scale.x), 100.f / static_cast<float>(Scale.y));
        E.getSpaceShip().setOrigin(0.f, 0.f);
        E.getSpaceShip().setPosition(x ,0.f);
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
    this->UserShotCD.restart();
    this->EnemyShotCooldown.restart();
    this->VersionTracker = this->SelectedVersion;
    this->SkinTracker = this->SelectedSkin;
    this->enemieRow = 1;
    this->EnemySpawnTimer = 500.f;
    this->Bullets.clear();
    this->Enemies.clear();
    this->initiateSpaceship();
}

void Game::MousePressed() {
    if (!this->gameStats){
        if (this->Start.getGlobalBounds().contains(this->MousePosition())) {
            if(this->Options != 4)
                this->reStart();
        }
        else if (this->Resume.getGlobalBounds().contains(this->MousePosition())) {
            this->Options = 3;
            this->gameStats = 1;
        }
        else if (this->Exit.getGlobalBounds().contains(this->MousePosition())) {
            if (this->Options == 0)
                this->window->close();
            else if(this->Options != 4){
                this->Options = 0;
            }
        }
        else if (this->ChangeSkin.getGlobalBounds().contains(this->MousePosition())) {
            this->Options = 5;
        }
        else if (this->RightLabel.getGlobalBounds().contains(this->MousePosition()) && this->VersionTracker < this->UserTexture.size()-1) {
            this->VersionTracker += 1;
        }
        else if (this->LeftLabel.getGlobalBounds().contains(this->MousePosition()) && this->VersionTracker > 0) {
            this->VersionTracker -= 1;
        }
        else if (this->Select.getGlobalBounds().contains(this->MousePosition())) {
            if (!this->SSUnlocked[this->VersionTracker][this->SkinTracker]) {
                if ((this->VersionTracker * 100) + (this->SkinTracker * 50) <= this->cash) {
                    this->SSUnlocked[this->VersionTracker][this->SkinTracker] = 1;
                    cash -= (this->VersionTracker * 100) + (this->SkinTracker * 50);
                }
            }
            else {
                if (this->VersionTracker != this->SelectedVersion || this->SkinTracker != this->SelectedSkin) {
                    this->SelectedVersion = this->VersionTracker;
                    this->SelectedSkin = this->SkinTracker;
                }
            }
        }
    }
}

void Game::Shoot(float velocity,SpaceF E) {
    Bullet bullet = E.getBullet(this->BulletTexture,velocity);
    bullet.getBullet().setPosition(E.getSpaceShip().getPosition().x + E.getSpaceShip().getGlobalBounds().width / 2.f, E.getSpaceShip().getGlobalBounds().top);
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
        if(this->Bullets[i].getBulletVelocity() < 0)
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
        else if (this->UserSS.getSpaceShip().getGlobalBounds().intersects(this->Bullets[i].getBullet().getGlobalBounds())) {
            this->health -= 1;
            swap(this->Bullets[i], this->Bullets.back());
            this->Bullets.pop_back();
            --i;
        }
    }
    if (nextLevel) {
        this->level += 1;
        this->isLevel = 1;
        this->enemieRow = 1;
        this->Bullets.clear();
        this->UserShotCD.restart();
        this->EnemyShotCooldown.restart();
        this->EnemySpawnTimer = 500.f;
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
        this->EnemySpawnTimer += min(this->level/3 * 4.f,150.f);

    if (this->Enemies.size() != 0 && this->EnemyShotCooldown.getElapsedTime().asMilliseconds() >= max(500.f, 4000.f - (level / 3.f * 50.f))) {
        int randnum = rand();
        if (this->Enemies[randnum % this->Enemies.size()].getShootCoolDown() <= 0.f && !this->Enemies[randnum % this->Enemies.size()].getIsExploding()) {
            this->Shoot(15.f, this->Enemies[randnum % this->Enemies.size()]);
            this->Enemies[randnum % this->Enemies.size()].setShootCoolDown(100.f);
            this->EnemyShotCooldown.restart();
        }
    }

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
            this->Enemies[i].getSpaceShip().move(0, min(this->level / 3 * .9f,10.f));
            if (this->Enemies[i].getShootCoolDown() > 0.f)
            this->Enemies[i].setShootCoolDown(this->Enemies[i].getShootCoolDown() - 20.f);
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
                    if (gameStats) {
                        this->gameover_music.play();
                        this->cash += (this->points * ((this->level + 2) / 3));
                    }
                    this->gameStats = 0;
                    this->Options = 1;
                }
            }
        }

    }
}

void Game::showGameStats() {

    Vector2f ScreenSize = Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y));

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
    this->ScoreStats.setPosition(ScreenSize.x - 100.f, 0.f);

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
                if (this->Options == 0) {

                }
                else if (this->Options == 2) {
                    this->Options = 3;
                    this->gameStats = 1;
                }
                else if (this->Options > 4) {
                    this->Options = 0;
                    this->VersionTracker = this->SelectedVersion;
                    this->SkinTracker = this->SelectedSkin;
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
