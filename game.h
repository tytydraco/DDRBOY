#ifndef GAME_H
#define GAME_H

class Game {
	Arduboy2 &arduboy;
	ArduboyTones &sound;
	unsigned int reset_highscore_hold = 0;
	bool anything_pressed();
	void randomize();
	struct Key {
		uint8_t button;
		int coords[2];
	};
	public:
		Game(Arduboy2 &n_arduboy, ArduboyTones &n_sound) : arduboy(n_arduboy), sound(n_sound) {}
		uint8_t game_state = 0; // 0: menu; 1: get ready; 2: play
		bool timer_active = false; // run the timer when true
		bool sound_on = false; // run the timer when true
		unsigned int score = 0; // duration in seconds timer has been running
		unsigned int highscore = 0; // the last score
		unsigned int difficulty = MIN_DIFFICULTY; // 1-4
		Key button_queue[5]; // use 1-6 as UP,LEFT,DOWN,RIGHT instead of 0-5 because an uninitialized int is always 0
		
		void menu();
		void play();
		void win();
		void select_difficulty();
		void read_highscore();
		void write_highscore();
};

#endif
