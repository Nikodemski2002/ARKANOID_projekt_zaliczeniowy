#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle
{
public:
	const float SPEED = 12.f;
	sf::Texture texture;
	sf::Sprite sprite;

	Paddle()
	{
		texture.loadFromFile("paddle2.png");
		sprite.setTexture(texture);
		sprite.setPosition(sf::Vector2f(800, 1400));
		sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
		sprite.setScale(sf::Vector2f(0.7, 0.7));
	}

	void moveLeft()
	{
		sprite.move(-SPEED, 0);
	}

	void moveRight()
	{
		sprite.move(SPEED, 0);
	}
};

class Ball
{
public:
	const float SPEED = 7.f;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f velocity;

	Ball()
	{
		texture.loadFromFile("ball.png");
		sprite.setTexture(texture);
		sprite.setPosition(sf::Vector2f(600, 1000));
		sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
		sprite.setScale(sf::Vector2f(0.7, 0.7));
		velocity = sf::Vector2f(SPEED, -SPEED);
	}

	void update()
	{
		sprite.move(velocity);
	}
};

class Block
{
public:
	sf::Texture texture;
	sf::Sprite sprite;
	bool destroyed = false;

	Block(int x, int y)
	{
		texture.loadFromFile("block.png");
		sprite.setTexture(texture);
		sprite.setPosition(sf::Vector2f(x, y));
		sprite.setScale(sf::Vector2f(0.4, 0.4));
	}
};

class Game
{
public:
	int windoWidth = 1600;
	int windowHeight = 1600;
	bool paused = false;
	bool gameOver = false;
	bool gameStarted = false;
	bool helpMenu = false;
	bool saveGame = false;
	int score = 0;
	int remainingBlocks = 0;
	sf::Font font;
	sf::Text pauseText;
	sf::Text helpText;
	sf::Text gameOverText;
	sf::Text scoreText;
	sf::Text gameStartText;
	sf::Text saveText;
	sf::RenderWindow window;
	Paddle paddle;
	Ball ball;
	std::vector<Block> blocks;

	Game()
	{
		window.create(sf::VideoMode(windoWidth, windowHeight), L"ARKANOID - Gra zaliczeniowa");
		sf::Image icon;
		icon.loadFromFile("icon.png");
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
		window.setFramerateLimit(60);

		font.loadFromFile("fonts/Bangers-Regular.ttf");

		pauseText.setFont(font);
		pauseText.setCharacterSize(60);
		pauseText.setFillColor(sf::Color::White);
		pauseText.setOutlineColor(sf::Color::Black);
		pauseText.setOutlineThickness(10);
		pauseText.setPosition(sf::Vector2f(500, 500));
		pauseText.setString(L"Gra wstrzymana\nNaciœnij Esc aby wznowiæ rozgrywkê");

		helpText.setFont(font);
		helpText.setCharacterSize(60);
		helpText.setFillColor(sf::Color::White);
		helpText.setOutlineColor(sf::Color::Black);
		helpText.setOutlineThickness(10);
		helpText.setPosition(sf::Vector2f(500, 500));
		helpText.setString(L"ARKANOID - GRA ZALICZENIOWA\n\n			  MENU POMOCY\n\n			  Pauza - Esc\n\n		    Opuœæ grê - X");

		gameOverText.setFont(font);
		gameOverText.setCharacterSize(60);
		gameOverText.setFillColor(sf::Color::White);
		gameOverText.setOutlineColor(sf::Color::Black);
		gameOverText.setOutlineThickness(10);
		gameOverText.setPosition(sf::Vector2f(400, 500));
		gameOverText.setString(L"						  KONIEC GRY\n\n			Naciœnij X aby opuœciæ grê");

		scoreText.setFont(font);
		scoreText.setCharacterSize(60);
		scoreText.setFillColor(sf::Color::White);
		scoreText.setOutlineColor(sf::Color::Black);
		scoreText.setOutlineThickness(10);
		scoreText.setPosition(sf::Vector2f(10, 10));

		gameStartText.setFont(font);
		gameStartText.setCharacterSize(60);
		gameStartText.setFillColor(sf::Color::White);
		gameStartText.setOutlineColor(sf::Color::Black);
		gameStartText.setOutlineThickness(10);
		gameStartText.setPosition(sf::Vector2f(430, 500));
		gameStartText.setString(L"						ARKANOID\n\n\nNaciœnij N aby rozpocz¹æ now¹ grê\n\n		Naciœnij X aby opuœciæ grê\n\n				F1 - Menu pomocy");

		for (int i = 0; i < 24; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				blocks.emplace_back(Block(200 + i * 50, 200 + j * 50));
				remainingBlocks++;
			}
		}
	}

	void input()
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X))
			{
				window.close();
			}

			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::N)
			{
				gameStarted = true;
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				if (paused)
					paused = false;
				else
					paused = true;
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1)
				if (helpMenu)
					helpMenu = false;
				else
					helpMenu = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			paddle.moveRight();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			paddle.moveLeft();
	}

	void update()
	{
		if (!paused && gameStarted)
		{
			ball.update();

			if (ball.sprite.getGlobalBounds().intersects(paddle.sprite.getGlobalBounds()))
			{
				float diff = ball.sprite.getPosition().x - paddle.sprite.getPosition().x;
				ball.velocity.x = (diff / (paddle.texture.getSize().x / 2)) * ball.SPEED;
				ball.velocity.y = -ball.SPEED;
			}

			for (auto& block : blocks)
			{
				if (!block.destroyed && ball.sprite.getGlobalBounds().intersects(block.sprite.getGlobalBounds()))
				{
					block.destroyed = true;
					remainingBlocks--;
					score++;

					if (ball.velocity.x > 0)
						ball.velocity.x = ball.SPEED;
					else
						ball.velocity.x = -ball.SPEED;

					ball.velocity.y = -ball.velocity.y;
				}
			}

			if (ball.sprite.getPosition().x <= 0 || ball.sprite.getPosition().x >= window.getSize().x)
				ball.velocity.x = -ball.velocity.x;

			if (ball.sprite.getPosition().y <= 0)
				ball.velocity.y = -ball.velocity.y;

			if (ball.sprite.getPosition().y >= window.getSize().y)
				gameOver = true;
		}
	}

	void render()
	{
		sf::RectangleShape background;
		background.setSize(sf::Vector2f(1600.f, 1600.f));
		sf::Texture MainTexture;
		MainTexture.loadFromFile("ja.jpg");
		background.setTexture(&MainTexture);
		window.draw(background);

		//window.clear(sf::Color::Black);

		if (helpMenu)
			window.draw(helpText);
		else if (paused)
			window.draw(pauseText);
		else if (gameOver)
			window.draw(gameOverText);
		else if (!gameStarted)
			window.draw(gameStartText);
		else
		{
			window.draw(paddle.sprite);
			window.draw(ball.sprite);

			for (auto& block : blocks)
			{
				if (!block.destroyed)
					window.draw(block.sprite);
			}

			scoreText.setString("Wynik: " + std::to_string(score));
			window.draw(scoreText);
		}

		window.display();
	}

	void run()
	{
		while (window.isOpen())
		{
			input();
			update();
			render();
		}
	}
};

int main()
{
	Game game;
	game.run();

	return 0;
}