// Arkanoid.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <SFML/Graphics.hpp>
using namespace sf;

#define MOSQUITTO_LOG_FILE "d:\\mosquitto.txt"

int width = 1024;
int height = 768;
int roadW = 2000;
int segL = 200; //segment length
float camD = 0.84; //camera depth
float playerX = 0;




void drawQuad(RenderWindow &w, Color c, int x1, int y1, int w1, int x2, int y2, int w2)
{
	ConvexShape shape(4);
	shape.setFillColor(c);
	shape.setPoint(0, Vector2f(x1 - w1, y1));
	shape.setPoint(1, Vector2f(x2 - w2, y2));
	shape.setPoint(2, Vector2f(x2 + w2, y2));
	shape.setPoint(3, Vector2f(x1 + w1, y1));
	w.draw(shape);
}

struct Line
{
	float x, y, z; //3d center of line
	float X, Y, W; //screen coord
	float curve, spriteX, clip, scale;
	Sprite sprite;

	Line()
	{
		spriteX = curve = x = y = z = 0;
	}

	void project(int camX, int camY, int camZ)
	{
		scale = camD / (z - camZ);
		X = (1 + scale * (x - camX)) * width / 2;
		Y = (1 - scale * (y - camY)) * height / 2;
		W = scale * roadW  * width / 2;
	}

	void drawSprite(RenderWindow &app)
	{
		Sprite s = sprite;
		int w = s.getTextureRect().width;
		int h = s.getTextureRect().height;

		float destX = X + scale * spriteX * width / 2;
		float destY = Y + 4;
		float destW = w * W / 266;
		float destH = h * W / 266;

		destX += destW * spriteX; //offsetX
		destY += destH * (-1);    //offsetY

		float clipH = destY + destH - clip;
		if (clipH < 0) clipH = 0;

		if (clipH >= destH) return;
		s.setTextureRect(IntRect(0, 0, w, h - h * clipH / destH));
		s.setScale(destW / w, destH / h);
		s.setPosition(destX, destY);
		app.draw(s);
	}
};



/*std::string exec(const char* cmd)
{
	FILE* pipe = _popen("C:\Program Files\mosquitto>mosquitto_sub -t \"sensornode / david / x / #\" -v >> d:\mosquito.txt", "r");
	if (!pipe) return "ERROR";
	char buffer[128];
	std::string result = "";
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	_pclose(pipe);
	return result;
}*/



#include <string>
#include <windows.h>

#include <sstream>
#include <string>
#include <fstream>


#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

std::string exec_function() {
	char buffer[2048];
	std::string result = "";
	FILE* pipe = _popen("d:\\mosquitto_sub2.bat","r");
	std::string match_to_find = "sensornode/david/x/Accelerometer/y";
	
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
			std::string line(buffer);
			
			if (line.find(match_to_find) != std::string::npos) {
				std::string strPlayerX = line.substr(match_to_find.length(), line.length());
				printf("%s\r\n", strPlayerX.c_str());
				if (atoi(strPlayerX.c_str()) < 0) {
					playerX -= 0.1;
					printf("Left\r\n");
				}
				else {
					playerX += 0.1;
					printf("Right\r\n");
				}
			}
		}
	}
	catch (...) {
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);
	return result;
}


bool is_empty(std::ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

//void readFileRealTime()
//{
//	std::string line;
//
//
//	while (true)
//	{
//		printf("Abrimos\r\n");
//		printf("Abrimos\r\n");
//		printf("Abrimos\r\n");
//		printf("Abrimos\r\n");
//		printf("Abrimos\r\n");
//		printf("Abrimos\r\n");
//		printf("Abrimos\r\n");
//		std::ifstream infile(MOSQUITTO_LOG_FILE);
//		std::string match_to_find = "sensornode/david/x/Accelerometer/y";
//
//		if (is_empty(infile)) {
//			Sleep(100);
//			infile.close();
//		}
//		else
//		{
//
//			while (std::getline(infile, line))
//			{
//				if (line.find(str) != std::string::npos) {
//					std::string strPlayerX = line.substr(str.length(), line.length());
//					printf("%s\r\n", strPlayerX.c_str());
//					if (atoi(strPlayerX.c_str()) < 0){
//							playerX -= 0.1;
//							printf("Left\r\n");
//					}
//					else {
//						playerX += 0.1;
//						printf("Right\r\n");
//					}
//					Sleep(550);
//				}								
//			}
//		}
//	}
//
//}


void launchMosquitto_sub() 
{
	int error = system("d:\\mosquitto_sub.bat");
	printf("Error subscriber");
}

#include <thread>

int main()
{

	//Initialize the subscriber class and subscriber to the topic sensornode/david/x/
	pch* pch_;
	pch_ = new pch("unique", "sensornode/david/x/#", "localhost", 1883);
	int res;

	std::remove(MOSQUITTO_LOG_FILE);
	
	//std::thread threadObj1(launchMosquitto_sub);
	//std::thread threadObj2(readFileRealTime);
	std::thread threadObj1(exec_function);


	RenderWindow app(VideoMode(width, height), "Outrun Racing!");
	app.setFramerateLimit(60);


	Texture t[50];
	Sprite object[50];
	for (int i = 1; i <= 7; i++)
	{
		bool xx = t[i].loadFromFile("C:\\Users\\ASUS_PORTATIL\\source\\repos\\Arkanoid\\images\\" + std::to_string(i) + ".png");
		t[i].setSmooth(true);
		object[i].setTexture(t[i]);
	}
	  
	Texture bg;
	bool xxx = bg.loadFromFile("C:\\Users\\ASUS_PORTATIL\\source\\repos\\Arkanoid\\images\\bg.png");
	
	bg.setRepeated(true);
	Sprite sBackground(bg);
	sBackground.setTextureRect(IntRect(0, 0, 5000, 411));
	sBackground.setPosition(-2000, 0);

	std::vector<Line> lines;

	for (int i = 0; i < 1600; i++)
	{
		Line line;
		line.z = i * segL;

		if (i > 300 && i < 700) line.curve = 0.5;
		if (i > 1100) line.curve = -0.7;

		if (i < 300 && i % 20 == 0) { line.spriteX = -2.5; line.sprite = object[5]; }
		if (i % 17 == 0) { line.spriteX = 2.0; line.sprite = object[6]; }
		if (i > 300 && i % 20 == 0) { line.spriteX = -0.7; line.sprite = object[4]; }
		if (i > 800 && i % 20 == 0) { line.spriteX = -1.2; line.sprite = object[1]; }
		if (i == 400) { line.spriteX = -1.2; line.sprite = object[7]; }

		if (i > 750) line.y = sin(i / 30.0) * 1500;

		lines.push_back(line);
	}

	int N = lines.size();
	
	int pos = 0;
	int H = 1500;


	while (app.isOpen())
	{

		//Connect to the MQTT broker
		res = pch_->loop(); // Keep MQTT connection
		if (res)
			pch_->reconnect();

		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		int speed = 0;

		if (Keyboard::isKeyPressed(Keyboard::Right)) playerX += 0.0005;
		if (Keyboard::isKeyPressed(Keyboard::Left)) playerX -= 0.0005;
		/*if (Keyboard::isKeyPressed(Keyboard::Up)) */speed = 200;
		if (Keyboard::isKeyPressed(Keyboard::Down)) speed = -200;
		if (Keyboard::isKeyPressed(Keyboard::Tab)) speed *= 3;
		if (Keyboard::isKeyPressed(Keyboard::W)) H += 100;
		if (Keyboard::isKeyPressed(Keyboard::S)) H -= 100;

		pos += speed;
		while (pos >= N * segL) pos -= N * segL;
		while (pos < 0) pos += N * segL;

		app.clear(Color(105, 205, 4));
		app.draw(sBackground);
		int startPos = pos / segL;
		int camH = lines[startPos].y + H;
		if (speed > 0) sBackground.move(-lines[startPos].curve * 2, 0);
		if (speed < 0) sBackground.move(lines[startPos].curve * 2, 0);

		int maxy = height;
		float x = 0, dx = 0;

		///////draw road////////
		for (int n = startPos; n < startPos + 300; n++)
		{
			Line &l = lines[n%N];
			l.project(playerX*roadW - x, camH, startPos*segL - (n >= N ? N * segL : 0));
			x += dx;
			dx += l.curve;

			l.clip = maxy;
			if (l.Y >= maxy) continue;
			maxy = l.Y;

			Color grass = (n / 3) % 2 ? Color(16, 200, 16) : Color(0, 154, 0);
			Color rumble = (n / 3) % 2 ? Color(255, 255, 255) : Color(0, 0, 0);
			Color road = (n / 3) % 2 ? Color(107, 107, 107) : Color(105, 105, 105);

			Line p = lines[(n - 1) % N]; //previous line

			drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
			drawQuad(app, rumble, p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
			drawQuad(app, road, p.X, p.Y, p.W, l.X, l.Y, l.W);
		}

		////////draw objects////////
		for (int n = startPos + 300; n > startPos; n--)
			lines[n%N].drawSprite(app);

		app.display();
	}

	return 0;
}

