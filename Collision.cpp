#include <SFML/Graphics.hpp>
#include <cstdlib>              // header for srand() & rand()
#include <ctime>                // header for time()
#include <math.h>
#include <iostream>
#include <cmath>                //abs

#define FPS 60
#define TIMESTEP 1.f/60.f
#define TIMESTEPSQR TIMESTEP * TIMESTEP
#define WINDOW_W 1024
#define WINDOW_H 768
#define PI 3.14159265
#define FORCE 5000.f
#define MASSINV 1.f/5.f
#define ACCELERATION FORCE * MASSINV
#define RADIUS 30.0f
using namespace std;
int main(void)
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Memes melt steel beams.");
	sf::Vector2f velocity = {0.0f, 0.0f};
	//upright, upleft, downleft, downright, up, down, right, left
	//up = 1, down = 2, right = 4, left = 8, leftmousedown = 16
	sf::Vector2f components[15];
	components[1] = {0,-ACCELERATION}; //up
	components[2] = {0, ACCELERATION}; //down
	components[4] = { ACCELERATION, 0}; //right
	components[8] = {-ACCELERATION,0}; //left
	components[5] = { ACCELERATION *(float) cos(7 * PI / 4),ACCELERATION*(float)sin(7 * PI / 4) }; //upright
	components[9] = { ACCELERATION *(float) cos(5 * PI / 4),ACCELERATION*(float)sin(5 * PI / 4) }; //upleft
	components[10]= { ACCELERATION*(float) cos(3 * PI / 4), ACCELERATION*(float)sin(3 * PI / 4) }; //downleft
	components[6] = { ACCELERATION*(float) cos(PI / 4), ACCELERATION*(float)sin(PI / 4) }; //downright
	float frictionCoeff[2] = {0.0f,0.2f};
	int friction = 0;
	int myFlags = 0;
	int mouseFlag = 0;
	sf::Keyboard::Key keyUp = sf::Keyboard::W;
	sf::Keyboard::Key keyDown = sf::Keyboard::S;
	sf::Keyboard::Key keyLeft = sf::Keyboard::A;
	sf::Keyboard::Key keyRight = sf::Keyboard::D;
	sf::Keyboard::Key keyQuit = sf::Keyboard::Escape;

	sf::Vector2f mPos = {(float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y};
    sf::Vector2f cPos;

    sf::CircleShape circles[2];
    sf::Color colors[2];
    colors[0] = sf::Color::Red;
    colors[1] = sf::Color::Blue;
    
    for (int i = 0; i < 2; i++) {
        sf::CircleShape circ;
        circ.setPosition(rand() % WINDOW_W, rand() % WINDOW_H);
        circ.setRadius(RADIUS);
        circ.setOrigin(RADIUS, RADIUS);
        circ.setFillColor(colors[i]); 
        circles[i] = circ;   
    }    

	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
				if (event.key.code == keyUp)
				{
					myFlags += 1;
					//cout << myFlags << endl;
				}
				if (event.key.code == keyDown)
				{
					myFlags += 1 << 1;
					//cout << myFlags << endl;
				}
				if (event.key.code == keyRight)
				{
					myFlags += 1 << 2;
					//cout << myFlags << endl;
				}
				if (event.key.code == keyLeft)
				{
					myFlags += 1 << 3;
					//cout << myFlags << endl;
				}
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == keyUp)
				{
					myFlags -= 1;
					//cout << myFlags << endl;
				}
				if (event.key.code == keyDown)
				{
					myFlags -= 1 << 1;
					//cout << myFlags << endl;
				}
				if (event.key.code == keyRight)
				{
					myFlags -= 1 << 2;
					//cout << myFlags << endl;
				}
				if (event.key.code == keyLeft)
				{
					myFlags -= 1 << 3;
					//cout << myFlags << endl;
				}
				break;
			case sf::Event::MouseButtonPressed:
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					mouseFlag += 1;
					friction = (friction + 1) % 2;
					//cout << myFlags << endl;
					break;
				}
				break;
			case sf::Event::MouseButtonReleased:
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					mouseFlag -= 1;
					//cout << myFlags << endl;
					break;
				}
				break;
			}
		}
        window.clear();
        
        for (int i = 0; i < 2; i++) {
            if (circles[i].getPosition().x - RADIUS <= 0)
            {
                circles[i].setPosition(RADIUS, circles[i].getPosition().y);
                velocity.x = abs(velocity.x);
            }
            if (circles[i].getPosition().x + RADIUS >= WINDOW_W)
            {
                circles[i].setPosition(WINDOW_W-RADIUS, circles[i].getPosition().y);
                velocity.x = -abs(velocity.x);
            }
            if (circles[i].getPosition().y - RADIUS <= 0) {
                circles[i].setPosition(circles[i].getPosition().x, RADIUS);
                velocity.y = abs(velocity.y);
            }
            if (circles[i].getPosition().y + RADIUS >= WINDOW_H)
            {
                circles[i].setPosition(circles[i].getPosition().x, WINDOW_H-RADIUS);
                velocity.y = -abs(velocity.y);
            }
            if (myFlags > 0)
            {	
                velocity.x += components[myFlags].x * TIMESTEP;
                velocity.y += components[myFlags].y * TIMESTEP;
                // circles[i].move(sf::Vector2f{components[myFlags].x, components[myFlags].y });
                // cout << "velocity: " << velocity << endl;
            }
            if(abs(velocity.x) <= 1e-5){
                velocity.x = 0.0f;
            } 
            if(abs(velocity.y) <= 1e-5){
                velocity.y = 0.0f;	
            }
            velocity.x -= (frictionCoeff[friction] * velocity.x * MASSINV);
            velocity.y -= (frictionCoeff[friction] * velocity.y * MASSINV);
            circles[i].move((.5 * components[myFlags].x * TIMESTEPSQR) + (velocity.x * TIMESTEP), (.5 * components[myFlags].y * TIMESTEPSQR) + (velocity.y * TIMESTEP));
            cout << "x: " << (.5 * components[myFlags].x * TIMESTEPSQR) + (velocity.x * TIMESTEP) << endl;
            cout << "y: " << (.5 * components[myFlags].y * TIMESTEPSQR) + (velocity.y * TIMESTEP) << endl;

            window.draw(circles[i]);
        }
        
		window.display();
	}
	return 0;
}