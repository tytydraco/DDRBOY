#ifndef GAME_H
#define GAME_H

class Game {
	Arduboy2 &arduboy;
	unsigned int reset_highscore_hold = 0;
	bool anything_pressed();
	void randomize();
	struct Key {
		uint8_t button;
		int coords[2];
	};
	public:
		Game(Arduboy2 &n_arduboy) : arduboy(n_arduboy) {}
		uint8_t game_state = 0; // 0: menu; 1: get ready; 2: play
		bool timer_active = false; // run the timer when true
		unsigned int score = 0; // duration in seconds timer has been running
		unsigned int highscore = 0; // the last score
		Key button_queue[4]; // use 1-6 as UP,LEFT,DOWN,RIGHT instead of 0-5 because an uninitialized int is always 0
		
		void menu();
		void play();
		void game_over();
		void win();
		void read_highscore();
		void write_highscore();
};

#endif
