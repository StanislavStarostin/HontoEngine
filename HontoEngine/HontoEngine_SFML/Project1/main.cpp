#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

// header ///////////////
inline void viewResize(sf::View& view, sf::RenderWindow& window, unsigned int& height, unsigned int& width);

class Program;

class DynamicBox
{
private:
	float lastX, lastY, xScalar, yScalar; // for bounceOf Program
public:
	friend Program;

	sf::RectangleShape shape;
	float speed;

	std::list<DynamicBox>* interactiveDynamicBoxes;

	DynamicBox* getThisConst()
	{
		return this;
	}
	bool checkCollision()
	{
		//for(int i = 0; i < interactiveDynamicBoxes->size(); i++)
			//if( shape.getGlobalBounds().intersects( interactiveDynamicBoxes[i]. ) ) ???

		for (std::list<DynamicBox>::iterator iter = interactiveDynamicBoxes->begin(); iter != interactiveDynamicBoxes->end(); iter++)
			if (this != iter->getThisConst() and shape.getGlobalBounds().intersects(iter->shape.getGlobalBounds()))
				return true;
		return false;
	}

	DynamicBox(sf::RectangleShape sh, float sp, std::list<DynamicBox>* idb) : shape(sh), speed(sp), interactiveDynamicBoxes(idb)
	{ }
	void shift(sf::Vector2f dest)
	{
		lastX = shape.getPosition().x;
		lastY = shape.getPosition().y;

		xScalar = (dest.x - shape.getSize().x / 2) - shape.getPosition().x;
		yScalar = (dest.y - shape.getSize().y / 2) - shape.getPosition().y;
		float divider = sqrt(xScalar * xScalar + yScalar * yScalar) / speed;

		if (divider == 0 or abs((dest.x - shape.getSize().x / 2) - shape.getPosition().x + xScalar / divider) < speed * 2 and abs((dest.y - shape.getSize().y / 2) - shape.getPosition().y + yScalar / divider) < speed * 2) // fix speed*2 someday 
		{
			shape.setPosition((dest.x - shape.getSize().x / 2), (dest.y - shape.getSize().y / 2));
		}
		else
		{
			shape.setPosition(shape.getPosition().x + xScalar / divider, shape.getPosition().y + yScalar / divider);
		}

		if (checkCollision())
		{
			shape.setPosition(lastX, lastY);

			if (speed > 1) // Not sure why but this condition seems to have helped
			{
				float ms = 0;
				do
				{
					ms += 0.5;
					divider = sqrt(xScalar * xScalar + yScalar * yScalar) / (speed - ms);
					shape.setPosition(lastX + xScalar / divider, lastY + yScalar / divider);
				} while (checkCollision());
			}
		}
		/*
		std::cout << "dest: " << dest.x << "; " << dest.y << std::endl;
		std::cout << "curr: " << shape.getPosition().x << "; " << shape.getPosition().y << std::endl;
		*/
	}
};

class Program
{
public:
	std::vector<float> posesVec;
	unsigned int destID;
	bool looped;
	std::string collisionEventName;
	DynamicBox* body;

	Program(std::vector<float> pv, DynamicBox* b, std::string cen) : posesVec(pv), body(b), destID(0), looped(true), collisionEventName(cen)
	{ }

	void move()
	{
		if (collisionEventName == "stop")
		{
			if (body->shape.getPosition().x + body->shape.getSize().x / 2 == posesVec[destID] and body->shape.getPosition().y + body->shape.getSize().y / 2 == posesVec[destID + 1])
				destID += 2;
			if (destID >= posesVec.size())
				if (looped)
					destID %= posesVec.size();
				else
					return;
			body->shift(sf::Vector2f(posesVec[destID], posesVec[destID + 1]));
			return;
		}
		if (collisionEventName == "bounceOf")
		{
			// CONTINUE HERE
		}
		if (collisionEventName == "budge")
		{
			// ...
		}
		if (collisionEventName == "activate") // by this
		{
			// ... 
		}

	}

};
/////////////////////////

int main()
{
	unsigned int windowWidth = 1280, windowHeight = 720; // coords
	unsigned int playerId = 0;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "HONTO?"/*, sf::Style::Fullscreen*/);
	sf::View view(sf::FloatRect(0.f, 0.f, windowWidth, windowHeight));
	window.setView(view);

	windowWidth = window.getSize().x;
	windowHeight = window.getSize().y;

	viewResize(view, window, windowHeight, windowWidth);

	//window.setView(view); 

	sf::RectangleShape shape(sf::Vector2f(windowWidth, windowHeight));
	shape.setFillColor(sf::Color::Yellow);

	sf::CircleShape shape2(100.f);
	shape2.setFillColor(sf::Color::Red);

	sf::RectangleShape boxShape(sf::Vector2f(50, 50));
	boxShape.setOutlineColor(sf::Color::Red);
	boxShape.setOutlineThickness(-3);

	boxShape.setFillColor(sf::Color::Magenta);
	boxShape.setPosition(-100, -100);

	std::list<DynamicBox> levelsDynamicBoxes;
	levelsDynamicBoxes.push_back(DynamicBox(boxShape, 1, &levelsDynamicBoxes));

	boxShape.setFillColor(sf::Color::Green);
	boxShape.setPosition(50, 500);
	levelsDynamicBoxes.push_back(DynamicBox(boxShape, 1, &levelsDynamicBoxes));



	std::vector<float> prog1_posesVec;

	prog1_posesVec.push_back(600);
	prog1_posesVec.push_back(300);

	prog1_posesVec.push_back(50);
	prog1_posesVec.push_back(50);

	prog1_posesVec.push_back(900);
	prog1_posesVec.push_back(50);

	Program prog1(prog1_posesVec, &levelsDynamicBoxes.back(), "stop");

	boxShape.setPosition(500, 500);
	levelsDynamicBoxes.push_back(DynamicBox(boxShape, 0.1, &levelsDynamicBoxes));

	Program prog2(prog1_posesVec, &levelsDynamicBoxes.back(), "stop");

	boxShape.setPosition(1000, 600);

	/*
	for (int i = 0; i < 500; i++)
	{
		if(i < 100)
			boxShape.setPosition(i*10, 600);
		else if(i < 200)
			boxShape.setPosition(i * 10 - 1000, 610);
		else if (i < 300)
			boxShape.setPosition(i * 10 - 2000, 620);
		else if (i < 400)
			boxShape.setPosition(i * 10 - 3000, 630);
		else if (i < 500)
			boxShape.setPosition(i * 10 - 4000, 640);
		levelsDynamicBoxes.push_back(DynamicBox(boxShape, 0.1, &levelsDynamicBoxes));
	}
	*/
		

	//delete prog1_posesVec;

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
			case sf::Event::Resized:
				viewResize(view, window, event.size.height, event.size.width);
				break;
			default:
				break;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			view.zoom(0.998f);
			window.setView(view);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			view.zoom(1.002f);
			window.setView(view);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			levelsDynamicBoxes.front().shift(sf::Vector2f(levelsDynamicBoxes.front().shape.getPosition().x + levelsDynamicBoxes.front().shape.getSize().x, levelsDynamicBoxes.front().shape.getPosition().y + levelsDynamicBoxes.front().shape.getSize().y / 2));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			levelsDynamicBoxes.front().shift(sf::Vector2f(levelsDynamicBoxes.front().shape.getPosition().x - levelsDynamicBoxes.front().shape.getSize().x, levelsDynamicBoxes.front().shape.getPosition().y + levelsDynamicBoxes.front().shape.getSize().y / 2));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			levelsDynamicBoxes.front().shift(sf::Vector2f(levelsDynamicBoxes.front().shape.getPosition().x + levelsDynamicBoxes.front().shape.getSize().x / 2, levelsDynamicBoxes.front().shape.getPosition().y - levelsDynamicBoxes.front().shape.getSize().y));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			levelsDynamicBoxes.front().shift(sf::Vector2f(levelsDynamicBoxes.front().shape.getPosition().x + levelsDynamicBoxes.front().shape.getSize().x / 2, levelsDynamicBoxes.front().shape.getPosition().y + levelsDynamicBoxes.front().shape.getSize().y));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			levelsDynamicBoxes.front().shift(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		}

		window.clear();
		window.draw(shape);
		window.draw(shape2);


		for(std::list<DynamicBox>::iterator iter = levelsDynamicBoxes.begin(); iter != levelsDynamicBoxes.end(); iter++)
			window.draw(iter -> shape);

		prog1.move();
		prog2.move();

		window.display();
	}

	return 0;
}

// obj //////////////////
inline void viewResize(sf::View& view, sf::RenderWindow& window, unsigned int& height, unsigned int& width)
{
	if (height / 9 * 16 <= width)
	{
		float resizedWidth = height / 9.f * 16;
		view.setViewport(sf::FloatRect((width - resizedWidth) / 2 / width, 0, resizedWidth / width, 1));
		window.setView(view);
	}
	else
	{
		float resizedHeight = width / 16.f * 9;
		view.setViewport(sf::FloatRect(0, (height - resizedHeight) / 2 / height, 1, resizedHeight / height));
		window.setView(view);
	}
}
/////////////////////////