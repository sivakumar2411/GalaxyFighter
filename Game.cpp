#include"Game.h"
#include"SpaceF.h"
#include<vector>

Game::Game()
{
    this->initVariables();
    this->initWindow();
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
    this->maxEnemies = 25;
    this->isHome = true;

}

void Game::initWindow()
{
    this->videomode.height = 600;
    this->videomode.width = 800;

    this->window = new RenderWindow(this->videomode, "Game1", Style::Close);
    this->window->setFramerateLimit(60);
}

void Game::showHomePage() {
    Text texts,texte,score;
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error at Load Font";
    }
    if (!this->isHome) {
        RectangleShape FinalScore;
        FinalScore.setFillColor(Color::Transparent);
        FinalScore.setSize(Vector2f(150.f, 30.f));
        FinalScore.setPosition(320.f, 200.f);
        
        score.setString("Your Score: " + to_string(this->points));
        score.setFont(font);
        score.setCharacterSize(20);
        score.setFillColor(Color::White);
        score.setOrigin(score.getGlobalBounds().left + score.getGlobalBounds().width / 2.f , score.getGlobalBounds().top + score.getGlobalBounds().height / 2.f);
        score.setPosition(FinalScore.getPosition().x + FinalScore.getSize().x / 2, FinalScore.getPosition().y + FinalScore.getSize().y / 2);
        
        this->window->draw(FinalScore);
        this->window->draw(score);
    }

    this->Start.setSize(Vector2f(100.f, 30.f));
    this->Start.setPosition(350.f, 250.f);
    this->Start.setFillColor(Color::Green);

    this->Exit.setSize(Vector2f(100.f, 30.f));
    this->Exit.setPosition(350.f, 300.f);
    this->Exit.setFillColor(Color::Red);

    texts.setFillColor(Color::White);
    texts.setString(this->isHome?"Start":"Restart");
    texts.setCharacterSize(20);
    texts.setFont(font);
    texts.setOrigin(texts.getGlobalBounds().left + texts.getGlobalBounds().width/2.f, texts.getGlobalBounds().top + texts.getGlobalBounds().height / 2.f);
    texts.setPosition(this->Start.getPosition().x + this->Start.getSize().x / 2,this->Start.getPosition().y + this->Exit.getSize().y / 2.f);

    texte.setString(this->isHome?"Exit":"Main menu");
    texte.setFillColor(Color::White);
    texte.setCharacterSize(20);
    texte.setFont(font);
    texte.setOrigin(texte.getGlobalBounds().left + texte.getGlobalBounds().width/2.f, texte.getGlobalBounds().top + texte.getGlobalBounds().height / 2.f);
    texte.setPosition(this->Exit.getPosition().x + this->Exit.getSize().x / 2.f,this->Exit.getPosition().y + this->Exit.getSize().y / 2.f);

    this->window->draw(this->Start);
    this->window->draw(this->Exit);
    this->window->draw(texts);
    this->window->draw(texte);
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
        E.getSpaceShip().setPosition(
            x + (i * 100.f),
            0.f
        );
        E.getSpaceShip().setSize(Vector2f(50.f,50.f));
        this->Enemies.push_back(E);
    }
    this->enemieRow += 1;
}

Vector2f Game::MousePosition() {
    return this->window->mapPixelToCoords(Mouse::getPosition(*window));
}

void Game::reStart() {
    this->isHome = 0;
    this->health = 3;
    this->points = 0;
    this->enemieRow = 1;
    this->EnemySpawnTimer = 800.f;
    this->gameStats = true;
    this->Enemies.clear();
}

void Game::MousePressed() {
    if (this->gameStats)
        for (int i = 0;i < this->Enemies.size();i++) {
            if (this->Enemies[i].getSpaceShip().getGlobalBounds().contains(this->MousePosition())) {
                this->Enemies.erase(this->Enemies.begin() + i);
                --i;
                this->points += 1;
            }
        }
    else {
        if (this->Start.getGlobalBounds().contains(this->MousePosition())) {
            this->reStart();
        }
        else if (this->Exit.getGlobalBounds().contains(this->MousePosition())) {
            if (isHome)
                this->window->close();
            else
                isHome = 1;
        }
    }
}

void Game::updateEnemy() {
    if (this->Enemies.size() < this->maxEnemies)
    {
        if (this->EnemySpawnTimer >= this->EnemySpawnTimeMax)
        {
            this->spawnEnemy();
            this->EnemySpawnTimer = 0.f;
        }
        else
            this->EnemySpawnTimer += 5.f;
    }

    for (int i = 0;i < this->Enemies.size();i++)
    {
        this->Enemies[i].getSpaceShip().move(0, 1.0f);
        if (this->Enemies[i].getSpaceShip().getGlobalBounds().top + this->Enemies[i].getSpaceShip().getGlobalBounds().height >= this->window->getSize().y) {
            this->Enemies.erase(this->Enemies.begin() + i);
            this->health -= 1;
            --i;
        }
        if (this->health <= 0) {
            this->gameStats = 0;
        }
    }
}

void Game::showGameStats() {
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error at Load Font from File.\n";
    }
    Text lst, sst;
    this->LifeStats.setFillColor(Color::Transparent);
    this->LifeStats.setSize(Vector2f(100.f,30.f));
    this->LifeStats.setPosition(0.f, 0.f);

    lst.setFillColor(Color::White);
    lst.setCharacterSize(20);
    lst.setFont(font);
    lst.setString("Life: " + to_string(this->health));
    lst.setOrigin(lst.getGlobalBounds().left + lst.getGlobalBounds().width / 2 , lst.getGlobalBounds().top + lst.getGlobalBounds().height / 2);
    lst.setPosition(this->LifeStats.getPosition().x + this->LifeStats.getSize().x / 2, this->LifeStats.getPosition().y + this->LifeStats.getSize().y / 2);


    this->ScoreStats.setFillColor(Color::Transparent);
    this->ScoreStats.setSize(Vector2f(100.f,30.f));
    this->ScoreStats.setPosition(700.f, 0.f);

    sst.setFillColor(Color::White);
    sst.setCharacterSize(20);
    sst.setFont(font);
    sst.setString("Score: " + to_string(this->points));
    sst.setOrigin(sst.getGlobalBounds().left + sst.getGlobalBounds().width / 2, sst.getGlobalBounds().top + sst.getGlobalBounds().height / 2);
    sst.setPosition(this->ScoreStats.getPosition().x + this->ScoreStats.getSize().x / 2, this->ScoreStats.getPosition().y + this->ScoreStats.getSize().y / 2);


    this->window->draw(LifeStats);
    this->window->draw(ScoreStats);
    this->window->draw(lst);
    this->window->draw(sst);
}

void Game::renderEnemy() {

    for (auto& e : Enemies)
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
        if (this->event.type == Event::MouseButtonPressed) {
            this->MousePressed();
        }
    }
}

void Game::update() {
    this->pollEvents();

    if(this->gameStats)
    this->updateEnemy();
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
    if (this->gameStats) {
        this->showGameStats();
        this->renderEnemy();
    }
    else
        this->showHomePage();
    this->window->display();
}
