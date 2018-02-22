#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <ctime> 
#include <math.h>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#define DEGTORAD 3.141592653589793f/180.0f
#define FPS 60
#define TIMESTEP 1.f/60.f
#define TIMESTEPSQR TIMESTEP * TIMESTEP
#define WINDOW_W 1440 //changed
#define WINDOW_H 900 //changed
#define PI 3.14159265
#define FORCE 10000.f
#define MASSINV 1.f/50.f
#define MASSINVOTHER 1.f/1.f
#define ACCELERATION 2.5f
#define RADIUS 30.0f
#define ECOEFF 1.f
#define CSIZE 6
using namespace std;

sf::Vector2f operator+(const sf::Vector2f  lhs, const sf::Vector2f& rhs) {
	return sf::Vector2f{ lhs.x + rhs.x, lhs.y + rhs.y };
}
sf::Vector2f operator-(const sf::Vector2f  lhs, const sf::Vector2f& rhs) {
	return sf::Vector2f{ lhs.x - rhs.x, lhs.y - rhs.y };
}
float operator*(const sf::Vector2f&  lhs, const sf::Vector2f& rhs) {
	return lhs.x * rhs.x + lhs.y * rhs.y;
}
sf::Vector2f operator*(float lhs, const sf::Vector2f rhs) {
	return sf::Vector2f{ lhs * rhs.x, lhs * rhs.y };
}
sf::Vector2f operator*(const sf::Vector2f lhs, float rhs) {
	return sf::Vector2f{ lhs.x * rhs, lhs.y * rhs };
}
sf::Vector2f operator/(const sf::Vector2f lhs, float rhs) {
	return sf::Vector2f{ lhs.x / rhs, lhs.y / rhs };
}
sf::Vector2f operator+=(const sf::Vector2f& lhs, const sf::Vector2f& rhs) {

	return sf::Vector2f{ lhs.x + rhs.x,lhs.y + rhs.y };
}
sf::Vector2f operator-=(const sf::Vector2f& lhs, const sf::Vector2f& rhs) {

	return sf::Vector2f{ lhs.x - rhs.x,lhs.y - rhs.y };
}
float operator%(sf::Vector2f lhs, sf::Vector2f rhs) { //cross
	return lhs.x * rhs.y - lhs.y * rhs.x;
}
ostream& operator<<(ostream &o, const sf::Vector2f &v) {
	o << v.x << ' ' << v.y;
	return o;
}
istream& operator>>(istream &i, sf::Vector2f &v) {
	i >> v.x >> v.y;
	return i;
}
map<string, sf::Texture> textures;
struct Entity
{
	public:
		bool onscreen;
		sf::Sprite sprite;
		sf::ConvexShape SAT;
		sf::Texture texture;
		sf::RectangleShape spriteBounds;
		sf::RectangleShape AABB;
		sf::Vector2f minPoint, maxPoint;
		sf::Vector2f velocity;
		string name;
		void move(sf::Vector2f offset) {
			sprite.move(offset);
			SAT.move(offset);
			AABB.move(offset);
			spriteBounds.move(offset);
			minPoint = AABB.getPosition();
			maxPoint = AABB.getPosition() + AABB.getSize();
		}
		void setPosition(sf::Vector2f position) {
			sprite.setPosition(position);
			SAT.setPosition(position);
			spriteBounds.setPosition(position);
			makeAABB();
			minPoint = AABB.getPosition();
			maxPoint = AABB.getPosition() + AABB.getSize();
		}
		void draw(sf::RenderWindow& window) {
			window.draw(spriteBounds);
			window.draw(SAT);
			window.draw(AABB);
			window.draw(sprite);
		}
		void makeAABB() {
			sf::Vector2f* AABBPoints = new sf::Vector2f[4];
			for (int i = 0; i < 4; i++) {
				AABBPoints[i] = spriteBounds.getTransform().transformPoint(spriteBounds.getPoint(i));
			}
			sf::Vector2f minPoint = AABBPoints[0];
			sf::Vector2f maxPoint = AABBPoints[0];

			for (int x = 0; x < 4; x++)
			{
				if (AABBPoints[x].x < minPoint.x)
					minPoint.x = AABBPoints[x].x;
				if (AABBPoints[x].y < minPoint.y)
					minPoint.y = AABBPoints[x].y;
				if (AABBPoints[x].x > maxPoint.x)
					maxPoint.x = AABBPoints[x].x;
				if (AABBPoints[x].y > maxPoint.y)
					maxPoint.y = AABBPoints[x].y;
			}

			sf::Vector2f sizeRect = maxPoint - minPoint;

			//AABB still spawns in 0,0 with origin at the same place.
			AABB.setPosition(sprite.getPosition()-(maxPoint - minPoint)/2);
			AABB.setSize(sizeRect);
			minPoint = AABB.getPosition();
			maxPoint = AABB.getPosition() + AABB.getSize();
			//AABB.setOrigin(spriteBounds.getOrigin());
			delete[] AABBPoints;
		}
		void rotate(float angle) {
			sprite.rotate(angle);
			SAT.rotate(angle);
			spriteBounds.rotate(angle);
			makeAABB();
		}
		void spawn(sf::Vector2f position) {
			onscreen = true;
			setPosition(position);
		}
		void destroy() {
			onscreen = false;
			setPosition(sf::Vector2f{-6969.f,-6969.f });
		}
		Entity() {}
		Entity(string n, string textureName) {
			name = n;
			cout << name << endl;
			if (name == "ShipRight") velocity = sf::Vector2f{ 100.f,0.f };
			else if (name == "ShipLeft") velocity = sf::Vector2f{ -100.f,0.f };
			sprite.setTexture(textures[textureName]);
			sf::FloatRect temp = sprite.getGlobalBounds();
			sf::Vector2f org = sf::Vector2f{ temp.width / 2,temp.height / 2 }+sprite.getPosition();
			spriteBounds.setSize(sf::Vector2f{ temp.width, temp.height });
			spriteBounds.setFillColor(sf::Color::Transparent);
			spriteBounds.setOutlineColor(sf::Color::Yellow);
			spriteBounds.setOutlineThickness(1.f);
			spriteBounds.setOrigin(org);

			AABB.setSize(sf::Vector2f{ temp.width, temp.height });
			AABB.setFillColor(sf::Color::Transparent);
			AABB.setOutlineColor(sf::Color::Blue);
			AABB.setOutlineThickness(1.f);

			SAT.setFillColor(sf::Color::Transparent);
			SAT.setOutlineColor(sf::Color::Red);
			SAT.setOutlineThickness(1.f);
			SAT.setOrigin(org);

			sprite.setOrigin(org);
			makeAABB();
		}
};
struct Darth : Entity {
	public:
		Entity saber;
		sf::Vector2f off;
		string textureNamez;
		Darth() : Entity() {}
		Darth(string n, string textureName, Entity* s, sf::Vector2f offset) : Entity(n, textureName) {
			saber = *s;
			saber.move(offset);
			off = offset;
			saber.SAT.setPointCount(4);
			for (int i = 0; i < 4; i++) {
				//cout << saber.spriteBounds.getPoint(i) << endl;
				saber.SAT.setPoint(i, saber.spriteBounds.getPoint(i));
			}
			textureNamez = textureName;
		}
		void move(sf::Vector2f offset) {
			Entity::move(offset);
			saber.move(offset);
		}
		void draw(sf::RenderWindow& window) {
			Entity::draw(window);
			saber.draw(window);
		}
		void activatePowerup()
		{
			string textTemp = textureNamez;

			textTemp += "a";
			string saberTemp = saber.name + "a";
			textureNamez = textTemp;
			saber.name = saberTemp;
			//cout << " Hello world " << textTemp << " " << saberTemp;
			Entity::sprite.setTexture(textures[textTemp]);
			saber.sprite.setTexture(textures[saberTemp]);
		}
		void keepSprite()
		{
			Entity::sprite.setTexture(textures[textureNamez]);
			saber.sprite.setTexture(textures[saber.name]);
		}
		void deactivatePowerup()
		{
			string textTemp = textureNamez;
			textTemp = textureNamez.substr(0,textureNamez.length() - 1);
			string saberTemp = saber.name.substr(0,saber.name.length() - 1);
			textureNamez = textTemp;
			saber.name = saberTemp;
			//cout << " Hello world " << textTemp << " " << saberTemp;
			Entity::sprite.setTexture(textures[textTemp]);
			saber.sprite.setTexture(textures[saberTemp]);
		}
		void rotate(float angle) {
			Entity::rotate(angle);
			saber.rotate(angle);
		}
		void setPosition(sf::Vector2f position) {
			Entity::setPosition(position);
			saber.setPosition(position);
			saber.move(off);
		}
};
//Darth vader, maul;
Darth darths[2];
// Entity leftShips[8], rightShips[8];
vector<Entity*> movingShips;
void spawnShip(vector<string> &fileContents, int& currentLine, vector<Entity*> &ships, Entity* ship);
void SATBorderSetup(Entity& entity,ifstream& file,string filename);
sf::Vector2f getMinMax(sf::Vector2f axis, sf::ConvexShape shape);
bool subSAT(sf::ConvexShape shape, sf::ConvexShape otherShape);
bool SAT(sf::ConvexShape i, sf::ConvexShape j);
int main(int argc, char *argv[])
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Flappy Bird (Star Wars version)");
	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);

	//input
	int myFlags = 0;
	int mouseFlag = 0;
	sf::Keyboard::Key keyUp = sf::Keyboard::W;
	sf::Keyboard::Key keyDown = sf::Keyboard::S;
	sf::Keyboard::Key keyLeft = sf::Keyboard::A;
	sf::Keyboard::Key keyRight = sf::Keyboard::D;
	sf::Keyboard::Key keyQuit = sf::Keyboard::Escape;
	sf::Vector2f components[15];
	components[1] = { 0,-ACCELERATION }; //up
	components[2] = { 0, ACCELERATION }; //down
	components[4] = { ACCELERATION, 0 }; //right
	components[8] = { -ACCELERATION,0 }; //left
	components[5] = { ACCELERATION,-ACCELERATION }; //upright
	components[9] = { -ACCELERATION,-ACCELERATION }; //upleft
	components[10] = { -ACCELERATION,ACCELERATION }; //downleft
	components[6] = { ACCELERATION,ACCELERATION }; //downright
																						   //timer
	sf::Clock clock;
	sf::Time iterationTime = sf::seconds(0);
	sf::Time elapsedTime = sf::seconds(0);
	sf::Time totalElapsedTime = sf::seconds(0);
	int currentLine = 0, fileContentSize = 0;

	//Graphics Setup
	string textureNames[] = { "Maula","Vadera","Sabera","DeathStar",
		"Maul","Saber","Vader","Ship","ShipLeft","ShipRight","UpgradeEnergy","UpgradeInvul","Background"};
	for (int i = 0; i < 13; i++) {
		if (!textures[textureNames[i]].loadFromFile("sprites/" + textureNames[i] + ".png")) cout << "Sprite not found.\n";
	}
	sf::Texture backgroundText;
	if(!backgroundText.loadFromFile("sprites/Background.png"))
	{}
	sf::Sprite background(backgroundText);
	
	//movingShips.push_back(Entity("ShipRight", "ShipRight"));
	//movingShips.push_back(Entity("ShipLeft", "ShipLeft"));
	//movingShips[0].move(sf::Vector2f{ 400.f,200.f});
	//movingShips[1].move(sf::Vector2f{ 400.f,500.f });

	// caching data from text file
	vector<string> fileContents;
	ifstream file;
	string line;
	file.open("Text.txt");
	if (file.is_open())
	{
		//cout << "File successfully open";
	}
	else { cout << "Error opening file"; }
	while (getline(file, line))
	{
		//cout << line << endl;
		fileContents.push_back(line);
		fileContentSize++;
	}
	file.close();

	//initialize darths with their sabers
	//setup SAT Borders as well
	darths[0] = Darth("Vader", "Vader", new Entity("Saber", "Saber"), sf::Vector2f{ 70.f,75.f });
	darths[0].move(sf::Vector2f{ 100.f,100.f });
	darths[0].saber.rotate(180.f);
	SATBorderSetup(darths[0], file, "VaderSATData.txt");
	darths[1] = Darth("Maul", "Maul", new Entity("Saber", "Saber"), sf::Vector2f{ 70.f,-75.f });
	darths[1].move(sf::Vector2f{ 100.f,800.f });
	SATBorderSetup(darths[1], file, "MaulSATData.txt");
	int currentShip = -1;

	/*
	for (int i = 0; i < 8; i++) {
		leftShips[i] = Entity("ShipLeft", "ShipLeft");
		leftShips[i].move(sf::Vector2f{ -6969.f, -6969.f });
		//leftShips[i].move(sf::Vector2f{ 200.f, 500.f});
		SATBorderSetup(leftShips[i], file, "ShipLeftSATData.txt");
		rightShips[i] = Entity("ShipRight", "ShipRight");
		leftShips[i].move(sf::Vector2f{ -6969.f, -6969.f });
		//rightShips[i].move(sf::Vector2f{ 200.f, 200.f });
		SATBorderSetup(rightShips[i], file, "ShipRightSATData.txt");
	}
	movingShips.push_back(&leftShips[0]);
	//movingShips.push_back(&rightShips[0]);
	(*movingShips[0]).setPosition(sf::Vector2f{ 400.f,200.f});
	//(*movingShips[1]).setPosition(sf::Vector2f{ 400.f,500.f });
	*/

	int powerup = 0; 
	bool initiated = false;
	float powerupClock;
	int charges = 2;

	int launch = 0;
	sf::Vector2f darthVelocity = { 0.f, 0.f };
	float launchOffset = -400.f; //adjust this to make him go higher
	float fallingOffset = 0.f;

	string thisLine = fileContents[currentLine];
	string delimiter = "|";
	cout << " time " <<  totalElapsedTime.asSeconds();
	float time = atof((thisLine.substr(0, thisLine.find(delimiter)).c_str()));
	thisLine.erase(0, thisLine.find(delimiter) + delimiter.length());

	while (window.isOpen())
	{
		sf::Event event;
		window.clear();
		//file input
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
					if(powerup == 0)
					{
						charges--;
						darths[0].activatePowerup();
						darths[1].activatePowerup();
						powerup = 1;
					}
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
						darths[0].keepSprite();
						darths[1].keepSprite();
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
					launch = -1;
					launchOffset = -400.f;
					fallingOffset = 0.f;
					break;
				case sf::Mouse::Right:
					launch = 1;
					launchOffset = -400.f;
					fallingOffset = 0.f;
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

		// cout << "line " << currentLine << endl;
		if (elapsedTime.asMilliseconds() > time)
		{	
			// cout << "current line: " << currentLine << " size: " << fileContentSize << endl;
			if (currentLine < fileContentSize)
			{	
				// create a new ship
				spawnShip(fileContents, currentLine, movingShips, new Entity());

				currentShip++;
				currentLine++;
				
				elapsedTime = sf::seconds(0);	

				thisLine = fileContents[currentLine];
				
				// cout << " time " <<  totalElapsedTime.asSeconds();
				float time = atof((thisLine.substr(0, thisLine.find(delimiter)).c_str()));
				thisLine.erase(0, thisLine.find(delimiter) + delimiter.length());			
			}

			if (currentLine >= fileContentSize){
				currentLine = 0;
			}
		}
		iterationTime = clock.restart();
		elapsedTime += iterationTime;
		totalElapsedTime += iterationTime;

		if(powerup != 0)
		{

			if(!initiated)
			{
				powerupClock = totalElapsedTime.asSeconds();
				initiated = true;
			}
			if(totalElapsedTime.asSeconds() >= powerupClock + 10.f)
			{
				powerup = 0;
				initiated = false;
				darths[0].deactivatePowerup();
				darths[1].deactivatePowerup();
			}
		}

		if((int)totalElapsedTime.asSeconds() % 15 == 0)
		{
			if(charges < 2)
			{
				charges++;
			}
		}

		//artificial physics
		if (launch != 0)
		{
			darthVelocity = { 100.f * launch, launchOffset }; // change X value of vector to adjust left and right movements
			launchOffset += 15.f; // adjust this to make his fall faster/slower
		}
		else
		{
			darthVelocity = { darthVelocity.x, fallingOffset };
			fallingOffset += 10.f; //adjust this to make his fall faster/slower (this is his natural falling down, not when he falls down from a jump)
		}

		if (launchOffset == 0.f)
		{
			launch = 0;
			launchOffset = -40.f;
		}

		//move
		for (int i = 0; i < 2; i++) {
			if (darths[i].sprite.getPosition().x<-30 || darths[i].sprite.getPosition().x>WINDOW_W + 30
				|| darths[i].sprite.getPosition().y<-200 || darths[i].sprite.getPosition().y>WINDOW_H + 200
				) {
				return 0;
			}
			darths[i].move(darthVelocity * TIMESTEP);
			//if(i==0)
			//darths[i].saber.rotate(components[myFlags].x);
			//else darths[i].saber.rotate(components[myFlags].y);
			//darths[i].move(4 * components[myFlags]);
		}
		for (int i = 0; i < movingShips.size(); i++){
			//(*movingShips[i]).move((*movingShips[i]).velocity * TIMESTEP);
			/*if (darths[0].sprite.getPosition().x<-30 || darths[0].sprite.getPosition().x>WINDOW_W + 30
				|| darths[0].sprite.getPosition().y<-50 || darths[0].sprite.getPosition().y>WINDOW_H + 50
				) {
				return 0;
			}*/
			movingShips[i]->move(movingShips[i]->velocity);
		}
		//Collision
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < movingShips.size(); j++)
			{
				//check for collision with darths
				if (!(darths[i].minPoint.x > (*movingShips[j]).maxPoint.x
					|| darths[i].minPoint.y >(*movingShips[j]).maxPoint.y
					|| (*movingShips[j]).minPoint.x > darths[i].maxPoint.x
					|| (*movingShips[j]).minPoint.y > darths[i].maxPoint.y)) {
					//cout << "AABB" << endl;
					//movingShips[j].AABB.setFillColor(sf::Color::Yellow);
					//darths[i].AABB.setFillColor(sf::Color::Yellow);
					
					if (SAT(darths[i].SAT, (*movingShips[j]).SAT))
						{
						//cout << "true" << endl;
						//cout << "colliding" << endl;
						if (powerup == 1)
							movingShips[j]->destroy();
						else
							{
								darths[i].SAT.setFillColor(sf::Color::Red);
								(*movingShips[j]).SAT.setFillColor(sf::Color::Red);
								cout << "YOU LOSE" << endl;
								//return 0;
							}
					}
				}

				//check for collision with sabers
				if (!(darths[i].saber.minPoint.x > (*movingShips[j]).maxPoint.x
					|| darths[i].saber.minPoint.y > (*movingShips[j]).maxPoint.y
					|| (*movingShips[j]).minPoint.x > darths[i].saber.maxPoint.x
					|| (*movingShips[j]).minPoint.y > darths[i].saber.maxPoint.y)) {
					//cout << "AABBSaber" << endl;
					if (SAT(darths[i].saber.SAT, (*movingShips[j]).SAT))
					{
						//cout << "true" << endl;
						//cout << "colliding" << endl;
						darths[i].SAT.setFillColor(sf::Color::Red);
						(*movingShips[j]).SAT.setFillColor(sf::Color::Red);
						if (powerup == 1)
							movingShips[j]->destroy();
					}
				}
				/*else {
					movingShips[j].AABB.setFillColor(sf::Color::Transparent);
					darths[i].AABB.setFillColor(sf::Color::Transparent);
				}*/
			}
		}

		//draw
		window.draw(background);
		for (int i = 0; i < 2; i++) {
			darths[i].draw(window);
		}
		

		if (currentShip > -1)
		{	
			// cout << "size " << movingShips.size() << endl;
			for (int i = 0; i < movingShips.size(); i++){
				// cout << "ship " << i << " " << movingShips[i]->name << endl;
				(*movingShips[i]).draw(window);
			}
		}
		window.display();
	}
	return 0;
}

void spawnShip(vector<string>& fileContents, int& currentLine, vector<Entity*> &ships, Entity* ship) {
	/*KILL ME NOW*/
	//input will be time after the first enemy was released, from left or right, y range, speed
	string line = fileContents[currentLine];
	string delimiter = "|";
	string delimiter2 = " ";

	vector<string> copyData = fileContents;

	ifstream file;
	file.open("Text.txt");
	
	int time;
	string direction;
	float startRange, endRange, speed;
	file >> time >> direction >> startRange >> endRange >> speed;

	file.close();

	cout << " yo " << time << direction << startRange << endRange << speed;

	float randY = startRange + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(endRange-startRange)));

	// // time after enemy before
	// int time = stoi(line.substr(0, line.find(delimiter)));
	// line.erase(0, line.find(delimiter) + delimiter.length());

	// // direction of ship
	// string direction = line.substr(0, line.find(delimiter));
	// line.erase(0, line.find(delimiter) + delimiter.length());

	// // y range
	// string range = line.substr(0, line.find(delimiter));
	// float startRange = atof(range.substr(0, range.find(delimiter2)).c_str());
	// range.erase(0, range.find(delimiter2) + delimiter2.length());
	// float endRange = atof(range.substr(0, range.find(delimiter2)).c_str());
	// line.erase(0, line.find(delimiter) + delimiter.length());

	// float randY = startRange + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(endRange-startRange)));

	// // speed
	// float speed = atof(line.substr(0, line.find(delimiter)).c_str());

	// Instantiate the ship
	if(direction == "left")
	{
		*ship = Entity("ShipLeft", "ShipLeft");
		ship->move(sf::Vector2f{ 1440.f, randY });
		ship->velocity = sf::Vector2f{speed * -1, 0.f};
		//leftShips[i].move(sf::Vector2f{ 200.f, 500.f});
		SATBorderSetup(*ship, file, "ShipLeftSATData.txt");
	}
	else
	{
		*ship = Entity("ShipRight", "ShipRight");
		// ship.setPosition(sf::Vector2f{1000, 200});
		ship->move(sf::Vector2f{ -100.f, randY });
		ship->velocity = sf::Vector2f{speed, 0.f};
		//leftShips[i].move(sf::Vector2f{ 200.f, 500.f});
		SATBorderSetup(*ship, file, "ShipLeftSATData.txt");
	}

	// add ship to vector of ships
	ships.push_back(ship);
}
void SATBorderSetup(Entity& entity, ifstream& file, string filename) {
	file.open(filename);
	if (file.is_open()) { /*cout << "File successfully open";*/ }
	else { /*cout << "Error opening file"; */}
	int iterations;
	while (file >> iterations)
	{
		//cout << iterations << endl;
		entity.SAT.setPointCount(iterations);
		cout << "pointcount" << entity.SAT.getPointCount() << endl;
		for (int i = 0; i < iterations; i++) {
			sf::Vector2f point;
			file >> point;
			//cout << point << endl;
			entity.SAT.setPoint(i, point);
		}
	}
	file.close();
}
bool SAT(sf::ConvexShape i, sf::ConvexShape j) {
	int iPointCount = i.getPointCount(), jPointCount = j.getPointCount();
	return subSAT(i, j) && subSAT(j, i);
}
bool subSAT(sf::ConvexShape shape, sf::ConvexShape otherShape) {
	int pointCount = shape.getPointCount();
	sf::Vector2f axis;
	int mod = pointCount;
	for (int k = 0; k < pointCount; k++) {
		// cout << k % mod << " "<< (k + 1) % mod << endl;
		// cout << pointCount << " i shape iteration " << k << endl;
		axis = shape.getTransform().transformPoint(shape.getPoint((k + 1) % mod))
			- shape.getTransform().transformPoint(shape.getPoint((k % mod)));
		//cout << "axis before normalization" << axis << endl;
		axis = sf::Vector2f{ -axis.y,axis.x };
		//axis = axis / sqrt(axis * axis);
		//cout << "axis" << axis << endl;
		//project points
		sf::Vector2f iMinMax = getMinMax(axis, shape);
		sf::Vector2f jMinMax = getMinMax(axis, otherShape);

		//cout << "iMinMax: " << iMinMax << endl;
		//cout << "jMinMax: " << jMinMax << endl;
		//x for min, y for max
		//if there is a gap
		if (iMinMax.x > jMinMax.y || jMinMax.x > iMinMax.y) {
			// cout << "failed at " << k << " shape. Number of iterations: " << pointCount << endl;
			return false;
		}
	}
	return true;
}
sf::Vector2f getMinMax(sf::Vector2f axis, sf::ConvexShape shape) {
	sf::Vector2f minMax;
	minMax.x = shape.getTransform().transformPoint(shape.getPoint(0)) * axis;
	minMax.y = shape.getTransform().transformPoint(shape.getPoint(0)) * axis;
	// minMax.x = shape.getPoint(0) * axis, minMax.y = shape.getPoint(0) * axis;
	int pointCount = shape.getPointCount();
	for (int i = 1; i< pointCount; i++) {
		float proj = shape.getTransform().transformPoint(shape.getPoint(i)) * axis;
		if (proj < minMax.x)
			minMax.x = proj;
		if (proj > minMax.y)
			minMax.y = proj;
	}
	return minMax;
}