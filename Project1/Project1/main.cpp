#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <deque>  
#include <iterator>  
#include <sstream>
#include "NNPlayer.h"
#pragma warning(disable : 4996)


class Bird
{

	float fVerticalSpeed;
	float fYCoord;
	float fXCoord;

	float fGravity;

	sf::Image BirdImage;
	sf::Texture BirdTexture;


public:

	sf::Sprite BirdSprite;

	Bird()
	{

		fVerticalSpeed = 0;
		fGravity = 1;
		fYCoord = 150;
		fXCoord = 100;

		BirdImage.loadFromFile("bird.png");
		BirdTexture.loadFromImage(BirdImage);
		BirdSprite.setTexture(BirdTexture);
		BirdSprite.setPosition(fXCoord, fYCoord);
	}

	void SetPositionDefault()
	{
		fYCoord = 150;
		fXCoord = 100;
	}

	void Move(float time)
	{
		fYCoord -= fVerticalSpeed * time;
		fVerticalSpeed = fVerticalSpeed - fGravity;
		if (fYCoord < 0)
		{
			fYCoord = 0;
			fVerticalSpeed = 0;
		}
		BirdSprite.setPosition(fXCoord, fYCoord);
		
	}
	
	void Jump()
	{
		fVerticalSpeed = 20;
		//fYCoord = 0;
	}

	void Update(float time)
	{
		Move(time);
	}

	float GetXCoord()
	{
		return fXCoord;
	}

	float GetYCoord()
	{
		return fYCoord;
	}

	sf::Sprite GetSprite()
	{
		return BirdSprite;
	}
};

class Tube
{
	float fHeight;
	float fXCoord;
	float fSpeed;

	sf::Image TubeImage;
	sf::Texture TubeTexture;

	float Delta = 90;
	


public:

	sf::Sprite TubeUpSprite;
	sf::Sprite TubeDownSprite;
	bool Checked = false;

	Tube()
	{
		TubeImage.loadFromFile("tube.png");

		TubeTexture.loadFromImage(TubeImage);
		TubeDownSprite.setTexture(TubeTexture);

		TubeUpSprite.setTexture(TubeTexture);

		TubeUpSprite.rotate(180);

		fXCoord = 640;
		fSpeed = 5;
		
		fHeight = (float)(150 + (rand() % 250));

	}

	void Move(float time)
	{
		fXCoord = fXCoord - fSpeed*time;
		TubeUpSprite.setPosition(fXCoord+25, fHeight-Delta);
		TubeDownSprite.setPosition(fXCoord, fHeight);

	}

	void Update(float time)
	{
		Move(time);
	}

	float GetXCoord()
	{
		return fXCoord;
	}

	sf::Sprite GetUpSprite()
	{
		return TubeUpSprite;
	}

	sf::Sprite GetDownSprite()
	{
		return TubeDownSprite;
	}
};

//struct returnvalue
//{
//	float height = 0;
//	float distance = 0;
//};

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "FlappyBird");
	sf::Image BG;
	sf::Texture BGTexture;
	sf::Sprite BGSprite;

	Bird Bird;
	

	int ScorePoint = 0;
	bool playing = false;
	float deltaTime = 4000;

	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text score("0", font, 40);
	score.setColor(sf::Color::White);
	score.setOutlineThickness(3);
	score.setOutlineColor(sf::Color::Black);
	std::ostringstream PlayerScore;
	score.setPosition(320, 40);

	BG.loadFromFile("bg.png");
	BGTexture.loadFromImage(BG);
	BGSprite.setTexture(BGTexture);

	//Bird.createMaskFromColor(sf::Color(255,255,255));//create mask for image(RGB color)

	sf::Clock clock; 
	srand(time(NULL));

	Tube *temp;
	std::deque<Tube*> Tubes;
	std::deque<Tube*> ::iterator it = Tubes.begin();

	ReturnValue returnValue;

	while (window.isOpen())
	{
		window.setFramerateLimit(60);
		float time = clock.getElapsedTime().asMicroseconds();
		time = time / 100000; //параметр управления скоростью игры
		deltaTime += clock.getElapsedTime().asMilliseconds();
		clock.restart();


		// Нейросеть
		NNPlayer AIplayer;
		

		//std::cout << deltaTime << "\n";
		//std::cout << "--------" << Tubes.size() << std::endl;
		std::cout  << returnValue.Distance<<" "<<returnValue.Height << std::endl;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (/*sf::Keyboard::isKeyPressed(sf::Keyboard::Space)&&*/playing == false) //проверка нажатия клавиши прыжка
		{
			Tubes.clear();
			playing = true;
			deltaTime = 4000;
			ScorePoint = 0;
			PlayerScore.str("0");
			score.setString(PlayerScore.str());
			Bird.SetPositionDefault();
		}


		int counter = 0;
		if (playing) //проверка начала игры
		{
			AIplayer.showWeights();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //проверка нажатия клавиши прыжка
			{
				Bird.Jump();
			}

			counter++;
			if (AIplayer.needToJump(returnValue) && (counter % 10) == 0) // запрос у нейросети
			{
				Bird.Jump();
				counter = 0;
			}

			Bird.Update(time);

			if (deltaTime > 3000) //создание трубы
			{
				Tube *tube = new Tube();
				Tubes.push_back(tube);
				deltaTime = 0;

			}

			for (it = Tubes.begin(); it < Tubes.end(); it++) //обновление движения труб
			{
				Tube *temp = *it;
				temp->Update(time);

			}

			it = Tubes.begin();
			temp = *it;
			if (temp->GetXCoord()+25 < Bird.GetXCoord() && temp->Checked == false)  //подсчет очков
			{
				ScorePoint++;
				PlayerScore.str("");
				PlayerScore << ScorePoint;
				score.setString(PlayerScore.str());
				temp->Checked = true;
			}

			if (Bird.GetYCoord() > 480) //проверка падения
			{
				playing = false;
			}

			it = Tubes.begin();
			temp = *it;        //проверка на столкновение с трубой 
			if (temp->GetDownSprite().getGlobalBounds().intersects(Bird.GetSprite().getGlobalBounds())|| 
				temp->GetUpSprite().getGlobalBounds().intersects(Bird.GetSprite().getGlobalBounds()))
			{
				playing = false;
				
				// изменение нейросети
				AIplayer.learn();
			}

			it = Tubes.begin();										//
			temp = *it;												//
			if (temp->GetXCoord() < -25 )							//
			{														//  удаление труб вышедших за экран
				Tubes.pop_front();									//
			}														//
		}

		for (it = Tubes.begin(); it < Tubes.end(); it++) //возврат расстояния до ближайшей трубы и её высоты
		{
			Tube *temp = *it;
			if (temp->Checked == false)
			{
				returnValue.Distance = temp->GetXCoord() - Bird.GetXCoord()+25;
				returnValue.Height = temp->TubeDownSprite.getPosition().y+45;
				break;
			}

		}
		
		

		window.clear();
		window.draw(BGSprite);
		window.draw(Bird.BirdSprite);
		for(it = Tubes.begin(); it < Tubes.end(); it++) //отрисовка труб
		{ 
			Tube *temp = *it;
			window.draw(temp->TubeDownSprite);
			window.draw(temp->TubeUpSprite);
		}
		window.draw(score);
		window.display();
		
	}

	return 0;
}
