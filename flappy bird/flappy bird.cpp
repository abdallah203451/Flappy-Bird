#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

//Drawing the window
RenderWindow window(VideoMode(1000, 600), "Flappy Bird");

//Defining the mouse vector
Vector2i mousePos = Mouse::getPosition(window);


float  fy;

//defining the game states
int	waiting = 0, started = 1, gameover = 2;

//the flag of the starting buttom
//the flag of the resume menu and difficulty levels
//changing the position of the pipes
//changing the color of the bird and pipes and background
int  but = 1, res = 0, dif = 0, p, x, y, z;

//the flag of the difficulty levels
int easy = 0, med = 0, hard = 0;

//controling in the difficulty

// a is the distance between two consecutive pipes
// gab is the distance between the upperand lower pipe
// c is the speed of the pipes
int a = 1, gab = 0, c = 0;
//defining an event
Event event;

//for the array of the pipes
const int o = 500000;
Sprite pipes[o];
int d = 0;


struct Game {
	int score = 0;
	int highscore = 0;
	int frames = 0;
	int gameState = 0;
	Sprite background1[3];
	Sprite background2[3];
	Sprite gameover;
	Sprite team_memb;
	Sprite wallpaper;
	Sprite buttom;
	Text pressC;
	Text scoreText;
	Text highscoreText;
	Text made_by;
	Font font;
	Font writing;
} game;

struct Textures {
	Texture flappy1[3];
	Texture flappy2[3];
	Texture flappy3[3];
	Texture pipe1;
	Texture pipe2;
	Texture background1;
	Texture background2;
	Texture gameover;
	Texture team;
	Texture wall;
	Texture but;
} textures;

struct Sounds {
	SoundBuffer chingBuffer;
	SoundBuffer hopBuffer;
	SoundBuffer crashBuffer;
	SoundBuffer clickBuffer;
	SoundBuffer fallBuffer;
	Sound ching;
	Sound hop;
	Sound crash;
	Sound click;
	Sound fall;

} sounds;

//The Game Music
Music gameing;


struct Flappy {
	double v = 0;
	int frame = 0;
	Sprite sprite;
} flappy;


//The declaration of the functions
void load_sound();
void load_textures();
void all_definitions();
void update_score();
void Flappy();
void generate_move_pipes();
void collision();
void events();
void draw_everything();
void difficulty_level();
void makers();
void starting();
void resume_menu();
void closing();


int main() {

	//general settings
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	//Difining and loading sounds and textures
	load_sound();
	load_textures();

	//all definitions about the game
	all_definitions();


	// initial position, scale
	flappy.sprite.setPosition(250, 300);
	flappy.sprite.setScale(2, 2);


	// main loop
	while (window.isOpen()) {

		//generating and moving pipes
		generate_move_pipes();

		//collision of pipes
		collision();

		//updating the score
		update_score();

		//all about the flappy bird
		Flappy();

		// events
		events();

		window.clear();

		draw_everything();

		window.display();

		// update total frames
		game.frames++;
	}
	return 0;
}

//All the sounds
void load_sound() {

	sounds.chingBuffer.loadFromFile("audio_score.wav");
	sounds.hopBuffer.loadFromFile("audio_flap.wav");
	sounds.crashBuffer.loadFromFile("audio_crash.wav");
	sounds.fallBuffer.loadFromFile("Assets_audio_die.ogg");
	sounds.clickBuffer.loadFromFile("click-melodic-tone.wav");
	sounds.ching.setBuffer(sounds.chingBuffer);
	sounds.hop.setBuffer(sounds.hopBuffer);
	sounds.crash.setBuffer(sounds.crashBuffer);
	sounds.fall.setBuffer(sounds.fallBuffer);
	sounds.click.setBuffer(sounds.clickBuffer);
}

//All the textures
void load_textures() {
	textures.background1.loadFromFile("background.png");
	textures.background2.loadFromFile("background-night.png");
	textures.pipe1.loadFromFile("pipe.png");
	textures.pipe2.loadFromFile("pipe-red.png");
	textures.gameover.loadFromFile("gameover.png");
	textures.flappy1[0].loadFromFile("flappy1.png");
	textures.flappy1[1].loadFromFile("flappy2.png");
	textures.flappy1[2].loadFromFile("flappy3.png");
	textures.flappy2[0].loadFromFile("bluebird-downflap.png");
	textures.flappy2[1].loadFromFile("bluebird-midflap.png");
	textures.flappy2[2].loadFromFile("bluebird-upflap.png");
	textures.flappy3[0].loadFromFile("redbird-downflap.png");
	textures.flappy3[1].loadFromFile("redbird-midflap.png");
	textures.flappy3[2].loadFromFile("redbird-upflap.png");
	textures.team.loadFromFile("images (3).jpg");
	textures.wall.loadFromFile("images (6)-1.jpg");
	textures.but.loadFromFile("flappyBirdPlayButton-1.png");
}

void all_definitions() {

	game.font.loadFromFile("flappy.ttf");

	game.background1[0].setTexture(textures.background1);
	game.background1[1].setTexture(textures.background1);
	game.background1[2].setTexture(textures.background1);
	game.background1[0].setScale(1.2, 1.2);
	game.background1[1].setScale(1.2, 1.2);
	game.background1[2].setScale(1.2, 1.2);
	game.background1[1].setPosition(333, 0);
	game.background1[2].setPosition(666, 0);

	game.background2[0].setTexture(textures.background2);
	game.background2[1].setTexture(textures.background2);
	game.background2[2].setTexture(textures.background2);
	game.background2[0].setScale(1.2, 1.2);
	game.background2[1].setScale(1.2, 1.2);
	game.background2[2].setScale(1.2, 1.2);
	game.background2[1].setPosition(333, 0);
	game.background2[2].setPosition(666, 0);

	game.gameover.setTexture(textures.gameover);
	game.gameover.setOrigin(192 / 2, 42 / 2);
	game.gameover.setPosition(500, 125);
	game.gameover.setScale(2, 2);

	game.team_memb.setTexture(textures.team);
	game.team_memb.setOrigin(701 / 2, 438 / 2);
	game.team_memb.setPosition(500, 300);
	game.team_memb.setScale(1.5, 1.6);

	game.wallpaper.setTexture(textures.wall);
	game.wallpaper.setOrigin(700 / 2, 393 / 2);
	game.wallpaper.setPosition(500, 300);
	game.wallpaper.setScale(1.5, 1.6);

	game.buttom.setTexture(textures.but);
	game.buttom.setOrigin(231 / 2, 128 / 2);
	game.buttom.setPosition(500, 470);
	game.buttom.setScale(0.8, 0.8);

	game.pressC.setString("PRESS C TO CONTINUE");
	game.pressC.setFont(game.font);
	game.pressC.setFillColor(Color::White);
	game.pressC.setCharacterSize(50);
	game.pressC.setOrigin(game.pressC.getLocalBounds().width / 2, 0);
	game.pressC.setPosition(500, 250);

	game.scoreText.setFont(game.font);
	game.scoreText.setFillColor(Color::White);
	game.scoreText.setCharacterSize(40);
	game.scoreText.setOrigin(0, 0);
	game.scoreText.setPosition(20, 50);

	game.highscoreText.setFont(game.font);
	game.highscoreText.setFillColor(Color::White);
	game.highscoreText.setCharacterSize(40);
	game.highscoreText.setPosition(20, 7);
}

//Generating and moving the pipes
void generate_move_pipes() {

	// generate pipes
	if (game.gameState == started && game.frames % a == 0) {
		p = rand() % 350;

		if (y == 1) {
			// lower pipe
			Sprite pipeL;
			pipeL.setTexture(textures.pipe1);
			pipeL.setPosition(1000, p + gab);
			pipeL.setScale(2, 2);

			// upper pipe
			Sprite pipeU;
			pipeU.setTexture(textures.pipe1);
			pipeU.setPosition(1000, p);
			pipeU.setScale(2, -2);

			// push to the array
			pipes[d] = pipeL;
			d++;
			pipes[d] = pipeU;
			d++;
		}

		else if (y == 2) {
			// lower pipe
			Sprite pipeL;
			pipeL.setTexture(textures.pipe2);
			pipeL.setPosition(1000, p + gab);
			pipeL.setScale(2, 2);

			// upper pipe
			Sprite pipeU;
			pipeU.setTexture(textures.pipe2);
			pipeU.setPosition(1000, p);
			pipeU.setScale(2, -2);

			// push to the array
			pipes[d] = pipeL;
			d++;
			pipes[d] = pipeU;
			d++;
		}
	}
	// move pipes
	if (game.gameState == started) {
		for (int i = 0; i < d; i++) {
			pipes[i].move(c, 0);
		}
	}
}

// collision detection
void collision() {

	if (game.gameState == started) {

		//updating the positions of pipes
		for (int i = 0; i < d; i++) {
			if (flappy.sprite.getGlobalBounds().intersects(pipes[i].getGlobalBounds())) {
				game.gameState = gameover;
				sounds.crash.play();
			}
		}
	}
}

//Counting the score
void update_score() {

	game.scoreText.setString("Score " + to_string(game.score));
	game.highscoreText.setString("HIGH " + to_string(game.highscore));

	for (int i = 0; i < d; i += 2) {
		if (game.gameState == started && (pipes[i]).getPosition().x == 250) {
			game.score++;
			sounds.ching.play();
			if (game.score % 5 == 0) {
				x = rand() % 3 + 1;
			}

			if (game.score > game.highscore) {
				game.highscore = game.score;
			}
		}
	}
}

//All about the flappy bird
void Flappy() {

	// update flappy position
	
	fy = flappy.sprite.getPosition().y;

	// move flappy and making the gravity
	if (game.gameState == started) {
		flappy.sprite.move(0, flappy.v);
		flappy.v += 0.5;
	}

	// flap the wings if playing
	if (game.gameState == waiting || game.gameState == started) {

		// change the texture once in 6 frames
		if (game.frames % 6 == 0) {
			flappy.frame += 1;
		}
		if (flappy.frame == 3) {
			flappy.frame = 0;
		}
	}
	//changing the colorr of the bird
	if (x == 1) {
		flappy.sprite.setTexture(textures.flappy1[flappy.frame]);
	}
	else if (x == 2) {
		flappy.sprite.setTexture(textures.flappy2[flappy.frame]);
	}
	else if (x == 3) {
		flappy.sprite.setTexture(textures.flappy3[flappy.frame]);
	}

	// if hits ceiling, stop ascending
	// if out of screen, game over
	if (game.gameState == started) {
		if (fy < 0) {
			flappy.sprite.setPosition(250, 0);
			flappy.v = 0;
		}
		else if (fy > 600) {
			flappy.v = 0;
			game.gameState = gameover;
			sounds.fall.play();
		}
	}
}

void events() {

	while (window.pollEvent(event)) {

		// bye bye
		if (event.type == Event::Closed) {
			window.close();
		}

		//if clicked on Escape
		if (dif == 0 && but == 0) {
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				if (game.gameState == waiting || game.gameState == started) {
					game.gameState = waiting;
					res = 1;
				}
			}
		}

		//if clicked on Space
		if (dif == 0 && but == 0) {
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				if (game.gameState == waiting) {
					game.gameState = started;
					res = 0;
				}

				//the speed of the jumb
				if (game.gameState == started) {
					flappy.v = -8;
					sounds.hop.play();
				}
			}
		}

		// restart
		if (Keyboard::isKeyPressed(Keyboard::C) && game.gameState == gameover) {
			game.gameState = waiting;
			flappy.sprite.setPosition(250, 300);
			game.score = 0;
			x = rand() % 3 + 1;
			y = rand() % 2 + 1;
			z = rand() % 2 + 1;
			for (int i = 0; i < d; i++) {
				pipes[i] = {};
			}
		}
	}
}

void draw_everything() {

	//drawing the background and the flappy

	if (z == 1) {
		window.draw(game.background1[0]);
		window.draw(game.background1[1]);
		window.draw(game.background1[2]);
	}
	if (z == 2) {
		window.draw(game.background2[0]);
		window.draw(game.background2[1]);
		window.draw(game.background2[2]);
	}

	window.draw(flappy.sprite);

	// draw pipes
	for (int i = 0; i < d; i++) {
		window.draw(pipes[i]);
	}

	// draw scores
	window.draw(game.scoreText);
	window.draw(game.highscoreText);

	// gameover. press c to continue and 
	//the time of the appearence of the game over statement
	if (game.gameState == gameover) {
		window.draw(game.gameover);

		if (game.frames % 60 < 30) {
			window.draw(game.pressC);
		}
	}

	// draw pause menu
	if (res == 1 || event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
		resume_menu();
	}

	//draw the difficulty
	if (dif == 1 || mousePos.x > 345 && mousePos.x < 670 && mousePos.y>400 && mousePos.y < 470) {
		difficulty_level();
	}

	//pressing on the buttom
	window.draw(game.wallpaper);
	window.draw(game.buttom);

	//introducing the names
	window.draw(game.team_memb);
	window.draw(game.made_by);
	makers();

	//closing the introducing names  and the starting button
	starting();

	//Closing the game from the top left door in the difficulty menu
	closing();
}

// pause and resume menu
void resume_menu() {

	//defining pause menu
	Texture resmenu;
	resmenu.loadFromFile("res. menu1-1.jpg");
	Sprite res_menu(resmenu);
	res_menu.setOrigin(216, 198);
	res_menu.setPosition(500, 300);
	res_menu.setScale(1.3, 1.3);

	window.draw(res_menu);

	//when we click on the mouse
	if (Mouse::isButtonPressed(Mouse::Left)) {
		Vector2i mousePos = Mouse::getPosition(window);

		//Pressing on resumme
		if (mousePos.x > 345 && mousePos.x < 670 && mousePos.y>210 && mousePos.y < 285) {
			sounds.click.play();
			res = 0;
			res_menu.setScale(0, 0);
			game.gameState = started;
		}

		//Pressing on Restart
		if (mousePos.x > 345 && mousePos.x < 670 && mousePos.y>300 && mousePos.y < 380) {
			x = rand() % 3 + 1;
			y = rand() % 2 + 1;
			z = rand() % 2 + 1;
			sounds.click.play();
			res = 0;
			res_menu.setScale(0, 0);
			game.gameState = waiting;
			flappy.sprite.setPosition(250, 300);
			game.score = 0;
			for (int i = 0; i < d; i++) {
				pipes[i] = {};
			}
		}

		//Pressing Exit
		if (mousePos.x > 345 && mousePos.x < 670 && mousePos.y>400 && mousePos.y < 470) {
			sounds.click.play();
			res = 0;
			dif = 1;
			res_menu.setScale(0, 0);
			game.gameState = waiting;
			flappy.sprite.setPosition(250, 300);
			game.score = 0;
			for (int i = 0; i < d; i++) {
				pipes[i] = {};
			}
			easy = 0;
			med = 0;
			hard = 0;
		}
	}
}

void difficulty_level() {

	//Difining diificulty image
	Texture diff;
	diff.loadFromFile("Screenshot_20220408_160236-1.jpg");
	Sprite difficulty(diff);
	difficulty.setOrigin(1080 / 2, 1426 / 2);
	difficulty.setPosition(500, 300);
	difficulty.setScale(0.95, 0.423);

	//Drawing the image
	if (dif == 1) {
		window.draw(difficulty);
	}

	//click on the mouse
	if (Mouse::isButtonPressed(Mouse::Left)) {
		Vector2i mousePos = Mouse::getPosition(window);

		//Choosing easy difficulty level
		if (mousePos.x > 320 && mousePos.x < 670 && mousePos.y>132 && mousePos.y < 200) {
			x = rand() % 3 + 1;
			y = rand() % 2 + 1;
			z = rand() % 2 + 1;
			sounds.click.play();
			dif = 0;
			easy = 1;
		}

		//Choosing medium difficulty level
		if (mousePos.x > 320 && mousePos.x < 670 && mousePos.y>220 && mousePos.y < 285) {
			x = rand() % 3 + 1;
			y = rand() % 2 + 1;
			z = rand() % 2 + 1;
			sounds.click.play();
			dif = 0;
			med = 1;
		}

		//Choosing hard difficulty level
		if (mousePos.x > 320 && mousePos.x < 670 && mousePos.y>320 && mousePos.y < 385) {
			x = rand() % 3 + 1;
			y = rand() % 2 + 1;
			z = rand() % 2 + 1;
			sounds.click.play();
			dif = 0;
			hard = 1;
		}
	}
	if (easy == 1) {
		a = 120, gab = 250, c = -3.5;
	}
	if (med == 1) {
		a = 90, gab = 190, c = -5.5;
	}
	if (hard == 1) {
		a = 60, gab = 170, c = -6.1;
	}
}

//Team Members
void makers() {

	game.writing.loadFromFile("Brief River.ttf");
	game.made_by.setFont(game.writing);
	game.made_by.setFillColor(Color::Green);
	game.made_by.setCharacterSize(36);
	game.made_by.setPosition(5, 10);
	game.made_by.setString("Mohamed Ahmed Mohamed Sayed\n\n\t\t\t\t\t\t\t\t\t\t\t\t\tAbdallah Ashraf Ahmed Sadek\n\nMohamed Ayman Mohamed Salah Eldin\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tOmar Abdo Hafez Abdo\n\nMalak Ahmed Azab\n\n\t\t\t\t\t\t\t\t\t\t\t\t\tAbd Elrahman Emad Bayoumi\n\nAbd Elrahman Emad Shamikh");
}

void starting() {

	//when we click on the mouse
	if (Mouse::isButtonPressed(Mouse::Left)) {
		Vector2i mousePos = Mouse::getPosition(window);

		//TO close the introducing image
		if (mousePos.x > 0 && mousePos.x < 1000 && mousePos.y>0 && mousePos.y < 600) {
			gameing.openFromFile("bg-music.wav");
			gameing.play();
			gameing.setLoop(true);
			game.team_memb.setScale(0, 0);
			game.made_by.setScale(0, 0);
		}

		//when we click on the start button

		if (mousePos.x > 400 && mousePos.x < 590 && mousePos.y>415 && mousePos.y < 515) {
			sounds.click.play();
			game.buttom.setScale(0, 0);
			game.wallpaper.setScale(0, 0);
			dif = 1;
			but = 0;
		}
	}
}

void closing() {

	if (Mouse::isButtonPressed(Mouse::Left)) {
		Vector2i mousePos = Mouse::getPosition(window);

		//the closing button
		if (mousePos.x > 0 && mousePos.x < 113 && mousePos.y>0 && mousePos.y < 53) {
			window.close();
		}
	}
}