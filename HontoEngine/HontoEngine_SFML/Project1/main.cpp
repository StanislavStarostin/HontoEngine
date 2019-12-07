#include <SFML/Graphics.hpp>

// header ///////////////
inline void viewResize(sf::View& view, sf::RenderWindow& window, unsigned int& height, unsigned int& width);
/////////////////////////

int main()
{
	unsigned int windowWidth = 1280, windowHeight = 720;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!"/*, sf::Style::Fullscreen*/);
	sf::View view(sf::FloatRect(0.f, 0.f, windowWidth, windowHeight));
	window.setView(view);

	windowWidth = window.getSize().x;
	windowHeight = window.getSize().y;

	viewResize(view, window, windowHeight, windowWidth);
	
	//window.setView(view); 
	
	sf::RectangleShape shape(sf::Vector2f(1600, 900));
	shape.setFillColor(sf::Color::Green);
	sf::CircleShape shape2(100.f);
	shape2.setFillColor(sf::Color::Red); 

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

		window.clear();
		window.draw(shape);
		window.draw(shape2);
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