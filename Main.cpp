#include <SFML/Graphics.hpp>
#include "UserDefinedHeaders\\Random.h"
#include <iostream>

void restart(sf::RectangleShape& rect1, sf::RectangleShape& rect2, sf::CircleShape& ball, int wWIDTH, int wHEIGHT)
{
    rect1.setPosition({wWIDTH-940.0f, wHEIGHT/2.0f});

    rect2.setPosition({wWIDTH-40.0f, wHEIGHT/2.0f});

    ball.setPosition({wWIDTH/2.0f, wHEIGHT/2.0f});
}

void bounceYDirection(int yDirection, float* ballYSpeed)
{
    // 0 - straight direction
    // 1 - up direction
    // 2 - down direction
    switch(yDirection)
    {
        case 0:
            *ballYSpeed = 0.0f;
            break;
        case 1:
            *ballYSpeed = -7.0f;
            break;
        case 2:
            *ballYSpeed = 7.0f;
            break;             
    }
}

void checkRectBorderCollision(sf::RectangleShape& rect, int wHEIGHT)
{
    // if it hits the bottom
    if(rect.getPosition().y + rect.getSize().y / 2.0f > wHEIGHT)
    {
        rect.setPosition
        ({
            rect.getPosition().x,
            wHEIGHT - rect.getSize().y / 2.0f
        });
    } 

    // if it hits the top
    if(rect.getPosition().y - rect.getSize().y / 2.0f < 0)
    {
        rect.setPosition
        ({
            rect.getPosition().x,
            rect.getSize().y / 2.0f
        });
    }
}

bool checkBallBorderCollisionY(sf::CircleShape& ball, int wHEIGHT)
{
    if(ball.getPosition().y + ball.getRadius() >= wHEIGHT || ball.getPosition().y - ball.getRadius() <= 0)
    {
        return true;
    }
    return false;
}

bool checkBallBorderCollisionX(sf::CircleShape& ball, int wWIDTH, float *ballXSpeed, int *player1Score, int *player2Score)
{
    if(ball.getPosition().x + ball.getRadius() >= wWIDTH ){
        *player1Score+=1;
        *ballXSpeed = -7.0f;
        return true;
    }
    if(ball.getPosition().x - ball.getRadius() <= 0)
    {
        *player2Score+=1;
        *ballXSpeed = 7.0f;
        return true;
    }
    return false;
}

void checkCollisionBetweenBallAndRects(sf::RectangleShape& rect1, sf::RectangleShape& rect2, sf::CircleShape& ball, float* ballXSpeed, float* ballYSpeed)
{
    if(ball.getGlobalBounds().findIntersection(rect1.getGlobalBounds()) || ball.getGlobalBounds().findIntersection(rect2.getGlobalBounds()))
    {
        int yDirection {static_cast<int>(Random::get(0, 2))};

        *ballXSpeed*=-1.0;
        bounceYDirection(yDirection, ballYSpeed);
    }
}   

void ball_movement(sf::CircleShape& ball, float ballXSpeed, float ballYSpeed)
{
    ball.move({ballXSpeed, ballYSpeed});
}

void rect2_movement(sf::RectangleShape& rect2, float rectYSpeed)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        rect2.move({0.f, -rectYSpeed});
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        rect2.move({0.f, rectYSpeed});
    }
}

void rect1_movement(sf::RectangleShape& rect1, float rectYSpeed)
{
    // rect1 movement
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        rect1.move({0.f, -rectYSpeed});
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        rect1.move({0.f, rectYSpeed});
    }
}

int main() 
{
    const int wWIDTH {1000};
    const int wHEIGHT {520};

    sf::RenderWindow window(sf::VideoMode({wWIDTH, wHEIGHT}), "PING PONG GAME", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::RectangleShape rect1({30, 100});
    rect1.setOrigin({rect1.getSize().x / 2.0f, rect1.getSize().y / 2.0f});
    rect1.setPosition({wWIDTH-960.0f, wHEIGHT/2.0f});
    rect1.setFillColor(sf::Color(30, 42, 86));

    sf::RectangleShape rect2({30, 100});
    rect2.setOrigin({rect2.getSize().x / 2.0f, rect2.getSize().y / 2.0f});
    rect2.setPosition({wWIDTH-40.0f, wHEIGHT/2.0f});
    rect2.setFillColor(sf::Color(128, 0, 0));

    float rectYSpeed {8.0};

    sf::CircleShape ball(20);
    ball.setOrigin({ball.getRadius(), ball.getRadius()});
    ball.setPosition({wWIDTH/2.0f, wHEIGHT/2.0f});
    ball.setFillColor(sf::Color::Yellow);

    float ballXSpeed {7.0f};
    float ballYSpeed {0.0f};

    sf::RectangleShape border1({3.0f, wHEIGHT});
    border1.setPosition({wWIDTH/2.0f, 0.0f});
    border1.setFillColor(sf::Color::White);

    sf::RectangleShape border2({wWIDTH, 3.0f});
    border2.setPosition({0.0f, wHEIGHT/2.0f});
    border2.setFillColor(sf::Color::White);

    int player1Score{0};
    int player2Score{0};

    sf::Font font("Fonts\\Roboto\\Roboto-Regular.ttf");
    sf::Text player1ScoreText(font);
    player1ScoreText.setCharacterSize(20);
    player1ScoreText.setFillColor(sf::Color::White);
    player1ScoreText.setPosition({wWIDTH-810, wHEIGHT-500});

    sf::Text player2ScoreText(font);
    player2ScoreText.setCharacterSize(20);
    player2ScoreText.setFillColor(sf::Color::White);
    player2ScoreText.setPosition({wWIDTH-310, wHEIGHT-500});

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        ball_movement(ball, ballXSpeed, ballYSpeed);
        rect1_movement(rect1, rectYSpeed);
        rect2_movement(rect2, rectYSpeed);

        checkRectBorderCollision(rect1, wHEIGHT);
        checkRectBorderCollision(rect2, wHEIGHT);

        checkCollisionBetweenBallAndRects(rect1, rect2, ball, &ballXSpeed, &ballYSpeed);
        
        if(checkBallBorderCollisionX(ball, wWIDTH, &ballXSpeed, &player1Score, &player2Score))
        {
            restart(rect1, rect2, ball, wWIDTH, wHEIGHT);
            sf::sleep(sf::seconds(3));
        }
        if(checkBallBorderCollisionY(ball, wHEIGHT))
        {
            ballYSpeed *= -1.0f;
        }

        player1ScoreText.setString("PLAYER 1: "+ std::to_string(player1Score));
        player2ScoreText.setString("PLAYER 2: "+ std::to_string(player2Score));

        window.clear(sf::Color(6, 64, 43));
        window.draw(border1);
        window.draw(border2);
        window.draw(rect1);
        window.draw(rect2);
        window.draw(ball);
        window.draw(player1ScoreText);
        window.draw(player2ScoreText);
        window.display();
    }

    return 0;
}