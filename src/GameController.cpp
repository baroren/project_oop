
#include "GameController.h"


#include <iostream>

using std::cout;
using std::endl;


GameController::GameController() {
    m_players.push_back(make_unique<King>(m_window.calculatePos('K'), 2, 4, 0.17, 3, "King.png", true));
    m_players.push_back(make_unique<Mage>(m_window.calculatePos('M'), 1, 3, 0.17, 3, "Mage.png", false));
    m_players.push_back(make_unique<Warrior>(m_window.calculatePos('W'), 1, 3, 0.17, 3, "Warrior.png", false));
    m_players.push_back(make_unique<Thief>(m_window.calculatePos('T'), 1, 3, 0.17, 3, "Thief.png", true));

    m_currPlayer.setSize(sf::Vector2f(30, 3));
    m_currPlayer.setOutlineColor(sf::Color::Transparent);
    m_currPlayer.setOutlineThickness(1);
    m_currPlayer.setOrigin(sf::Vector2f(m_currPlayer.getGlobalBounds().width-30.f,
                                        m_currPlayer.getGlobalBounds().height-40.f));

    m_playerShow.push_back(make_unique<King>(sf::Vector2f(100 ,800), 2, 4, 0.17, 5, "King.png", true));
    m_playerShow.push_back(make_unique<Mage>(sf::Vector2f(100 ,800), 1, 3, 0.17, 5, "Mage.png", false));
    m_playerShow.push_back(make_unique<Warrior>(sf::Vector2f(100 ,800), 1, 3, 0.17, 5, "Warrior.png", false));
    m_playerShow.push_back(make_unique<Thief>(sf::Vector2f(100 ,800), 1, 3, 0.17, 5, "Thief.png", true));
    m_blockObjects.push_back(make_unique<Wall>(m_window.calculatePos('='), 1, 4, 0.17, 3, "skeleton2_v2.png"));

    storeTeleproters();
    m_menu.createButton("New Game",100,150);
    m_menu.createButton("Help",100,225);
    m_menu.createButton("Main Menu",100,300);
    m_menu.createButton("Quit",100,375);

    //cout << m_teleporters[1]->getLinkdedTeleporter()->getLocation().x << ' ' << m_teleporters[1]->getLinkdedTeleporter()->getLocation().y << endl;
    //cout << m_teleporters[0]->getLinkdedTeleporter()->getLocation().x << ' ' << m_teleporters[0]->getLinkdedTeleporter()->getLocation().y;


    if (!m_font.loadFromFile("arcadeClassic.ttf")) {
        // error...
        std::cout << "error loading font";

    }


    m_timer.setFont(m_font);
    m_timer.setOrigin(sf::Vector2f(m_timer.getGlobalBounds().width / 2.f,
                                   m_timer.getGlobalBounds().height / 2.f));
    m_timer.setPosition(sf::Vector2f(50, 75));

}
void GameController::run()
{

    m_mainMenu.run(m_window.getWindow());

    sf::Clock clock;
    float deltaTime;
    int key = 0;
    sf::Vector2f moveDirection;
    m_clock =new Clock(80);
    m_clock->reset();
	while (m_window.isOpen()) {

        m_window.getWindow().clear(sf::Color(34, 20, 26));


        deltaTime = clock.restart().asSeconds();
        m_timer.setString(m_clock->countDown());




       // m_mainMenu.printHelp(m_window.getWindow(),m_buttons[1]);

        for (auto& currentPlayer : m_players)
        {

            handleCollision(*currentPlayer, moveDirection);
        }



        m_window.displayBoard();



//          draw all teleporters
        for (int i = 0; i < m_teleporters.size(); i++)
        {
            m_teleporters[i]->updateAndDraw(0, deltaTime, m_window.getWindow());
        }

//          draw all blocking objects
        for (int i = 0; i < m_blockObjects.size(); i++)
        {
            m_blockObjects[i]->updateAndDraw(0, deltaTime, m_window.getWindow());
        }

//          draw all players
        for (int i = 0; i < m_players.size(); i++)
        {

            if (i != key)
                m_players[i]->updateAndDraw(0, deltaTime, m_window.getWindow());
        }

        m_currPlayer.setPosition(m_players[key]->getLocation());

        m_players[key]->updateAndDraw(0, deltaTime, m_window.getWindow());
        m_window.getWindow().draw(m_currPlayer);

        for (int i = 0; i < m_static.size(); i++) {
            m_static[i]->updateAndDraw(0, deltaTime, m_window.getWindow());
        }

        m_playerShow[key]->updateAndDraw(0, deltaTime, m_window.getWindow());




        m_window.drawText(m_timer);
        m_menu.updateBt(m_window.getWindow());
       sf::Event event;
        if (m_window.getWindow().pollEvent(event))
        {

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if(  m_menu.handleClick(m_window.getWindow().
                            mapPixelToCoords(sf::Mouse::getPosition(m_window.getWindow())),m_window.getWindow())==0) {
                        cout<<"run";
                        run();

                    }
                    if(  m_menu.handleClick(m_window.getWindow().
                      mapPixelToCoords(sf::Mouse::getPosition(m_window.getWindow())),m_window.getWindow())==2)
                        m_mainMenu.run(m_window.getWindow());

                    if(  m_menu.handleClick(m_window.getWindow().
                    mapPixelToCoords(sf::Mouse::getPosition(m_window.getWindow())),m_window.getWindow())==3)
                        m_window.getWindow().close();
                    std::cout<<"yeah";
                }
            }
            switch(event.type) {
                case sf::Event::KeyReleased: {

                    if (event.key.code == sf::Keyboard::K) {
                        (key == m_players.size() - 1) ? key = 0 : key++;
                    }
                    if (event.key.code == sf::Keyboard::T) {
                        //          check collisions special for the player objects (throne, teleporters etc)

                            handleCollision(key);

                      cout<<"test"<<endl;
                    }
                    break;

                }

                case sf::Event::Closed: {
                    m_window.close();
                    break;
                }
            }
        }

        if(m_menu.helpPressed() ) {
            std::cout<<"help pressed"<<endl;
            m_window.getWindow().draw(m_menu.getHelp());
        }
        m_window.display();

        handleKey(deltaTime, key, moveDirection);
    }

}

void GameController::storeTeleproters()
{
    sf::Vector2f foundPos_1, foundPos_2;

    int teleIndex = 0, nextTeleIndex = 1;
    while (true)
    {
        foundPos_1 = m_window.calculatePos('X');
        foundPos_2 = m_window.calculatePos('X');

        if (foundPos_1.x != -1 && foundPos_2.x != -1)
        {

            m_teleporters.push_back(make_unique<Teleporter>(foundPos_1, 1, 4, 0.17, 3, "Teleport.png"));
            m_teleporters.push_back(make_unique<Teleporter>(foundPos_2, 1, 4, 0.17, 3, "Teleport.png"));

            m_teleporters[teleIndex]->setLinkedTeleporter(*m_teleporters[nextTeleIndex]);
            m_teleporters[nextTeleIndex]->setLinkedTeleporter(*m_teleporters[teleIndex]);

            teleIndex += 2;
            nextTeleIndex += 2;
        }

        else
            break;
    }
}

//  handles collision that have the same affect on the moving object
void GameController::handleCollision(Moving& movingObject, const sf::Vector2f moveDirection)
{
//      check collisions with blocking objects
    for (auto& blockObject : m_blockObjects)
    {
        if (movingObject.checkCollision(*blockObject))
        {
            movingObject.handleCollision(*blockObject, moveDirection);
        }
    }
}

void GameController::handleCollision(const int key)
{
    for (auto& teleporter : m_teleporters)
    {
        if (m_players[key]->checkCollision(*teleporter))
        {
            m_players[key]->handleCollision(*teleporter);
            return;
        }
    }
}


void GameController::handleKey(float deltaTime,int &key, sf::Vector2f& moveDirection)
{

    if(sf::Keyboard::isKeyPressed (sf::Keyboard::P))
    {
        (key == m_players.size() - 1) ? key = 0 : key++;
    }

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
       // running = false;   //why tho?

        m_window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {


    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_players[key]->move(LEFT,deltaTime, moveDirection);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_players[key]->move(RIGHT,deltaTime, moveDirection);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        m_players[key]->move(UP,deltaTime, moveDirection);
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_players[key]->move(DOWN,deltaTime, moveDirection);
    }

}
bool GameController::isRunning()
{
    return running;
}