#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "globals.h"
#include "sprites.h"
#include "game.h"
#include "sounds.h"

void Game::menu() {
	arduboy.setFrameRate(FRAMERATE);
	// draw title and highscore
	arduboy.setTextSize(2);
	arduboy.setCursor(0, 0);
	arduboy.println("DDRBOY");
	arduboy.setTextSize(1);
	arduboy.println("HIGHSCORE");
	arduboy.println(highscore);
	
	// center START text
	arduboy.setTextSize(2);
	arduboy.setCursor(0, HEIGHT - 15);
	arduboy.println("START");
	
	// handle sound
	if (sound_on) {
		arduboy.drawBitmap(WIDTH - 16, HEIGHT - 32, MUSIC_ON, 16, 16, WHITE);
	} else {
		arduboy.drawBitmap(WIDTH - 16, HEIGHT - 32, MUSIC_OFF, 16, 16, WHITE);
	}
	
	// toggle sound
	if (arduboy.justPressed(B_BUTTON)) {
		if (sound_on) {
			arduboy.audio.off();
		} else {
			arduboy.audio.on();
			sound.tones(ok);
		}
		sound_on = !sound_on;
	}
	
	arduboy.drawBitmap(WIDTH - 16, HEIGHT - 16, A_ICON, 16, 16, WHITE);
	
	// difficulty select
	if (arduboy.justPressed(A_BUTTON)) {
		game_state = 3;
		sound.tones(ok);
	}
	
	// reset highscore
	if (arduboy.everyXFrames(FRAMERATE) && arduboy.pressed(B_BUTTON)) {
		reset_highscore_hold++;
		if (reset_highscore_hold == 3) {
			EEPROM.put(HIGHSCORE_ADDR, 0);
			read_highscore();
			reset_highscore_hold = 0;
		}
	}
}

void Game::select_difficulty() {
	// change difficulty with buttons
	if (arduboy.justPressed(LEFT_BUTTON)) {
		if (difficulty > MIN_DIFFICULTY) {
			difficulty--;
		}
	} else if (arduboy.justPressed(RIGHT_BUTTON)) {
		if (difficulty < MAX_DIFFICULTY) {
			difficulty++;
		}
	}
	
	// print difficulty
	arduboy.setTextSize(3);
	switch (difficulty) {
		case 1:
			arduboy.setCursor(30, 22);
			arduboy.println(DIFFICULTY_1_NAME);
			break;
		case 2:
			arduboy.setCursor(2, 22);
			arduboy.println(DIFFICULTY_2_NAME);
			break;
		case 3:
			arduboy.setCursor(30, 22);
			arduboy.println(DIFFICULTY_3_NAME);
			break;
		case 4:
			arduboy.setCursor(10, 22);
			arduboy.println(DIFFICULTY_4_NAME);
			break;
		default:
			break;
	}
	
	// play
	if (arduboy.justPressed(A_BUTTON)) {
		switch (difficulty) {
			case 1:
				arduboy.setFrameRate(DIFFICULTY_1_FPS);
				break;
			case 2:
				arduboy.setFrameRate(DIFFICULTY_2_FPS);
				break;
			case 3:
				arduboy.setFrameRate(DIFFICULTY_3_FPS);
				break;
			case 4:
				arduboy.setFrameRate(DIFFICULTY_4_FPS);
				break;
			default:
				break;
		}
		randomize();
		game_state = 1;
		score = 0;
		sound.tones(start);
	} else if (arduboy.justPressed(B_BUTTON)) {
		game_state = 0;
	}
}

void Game::randomize() {
	for (size_t i = 0; i < (sizeof(button_queue) / sizeof(button_queue[0])); i++) {
		uint8_t random_button = random(MIN_RANDOMIZE, MAX_RANDOMIZE);
		button_queue[i].button = random_button; //1-6 L,R,U,D,A,B
		button_queue[i].coords[0] = random_button * 16;
		button_queue[i].coords[1] = HEIGHT + (i * 16);
	}
}

void Game::play() {
	// print the target buttons
	if (arduboy.pressed(UP_BUTTON)) {
		arduboy.drawBitmap(16, 0, UP_ARROW, 16, 16, WHITE);
	} else {
		arduboy.drawBitmap(16, 0, UP_PRESSED_ARROW, 16, 16, WHITE);
	}
	
	if (arduboy.pressed(DOWN_BUTTON)) {
		arduboy.drawBitmap(32, 0, DOWN_ARROW, 16, 16, WHITE);
	} else {
		arduboy.drawBitmap(32, 0, DOWN_PRESSED_ARROW, 16, 16, WHITE);
	}
	
	if (arduboy.pressed(LEFT_BUTTON)) {
		arduboy.drawBitmap(48, 0, LEFT_ARROW, 16, 16, WHITE);
	} else {
		arduboy.drawBitmap(48, 0, LEFT_PRESSED_ARROW, 16, 16, WHITE);
	}
	
	if (arduboy.pressed(RIGHT_BUTTON)) {
		arduboy.drawBitmap(64, 0, RIGHT_ARROW, 16, 16, WHITE);
	} else {
		arduboy.drawBitmap(64, 0, RIGHT_PRESSED_ARROW, 16, 16, WHITE);
	}
	
	if (arduboy.pressed(A_BUTTON)) {
		arduboy.drawBitmap(80, 0, A_ICON, 16, 16, WHITE);
	} else {
		arduboy.drawBitmap(80, 0, A_PRESSED_ICON, 16, 16, WHITE);
	}
	
	if (arduboy.pressed(B_BUTTON)) {
		arduboy.drawBitmap(96, 0, B_ICON, 16, 16, WHITE);
	} else {
		arduboy.drawBitmap(96, 0, B_PRESSED_ICON, 16, 16, WHITE);
	}
	
	//if (anything_pressed()) sound.tones(key);
	
	// check for next button press needed
	uint8_t next_index = 0;
	for (size_t i = 0; i < (sizeof(button_queue) / sizeof(button_queue[0])); i++) {
		if (button_queue[i].button != 0 && button_queue[i].coords[1] <= 16 && button_queue[i].coords[1] >= -16) {
			next_index = i;
			break; 
		}
	}
		
	// detect the press and check
	if (button_queue[next_index].coords[1] <= 16 && button_queue[next_index].coords[1] >= -16) {
		if (button_queue[next_index].button == 1 && arduboy.justPressed(UP_BUTTON)) {
			button_queue[next_index].button = 0;
			score++;
			sound.tone(260, 100);
		} else if (button_queue[next_index].button == 2 && arduboy.justPressed(DOWN_BUTTON)) {
			button_queue[next_index].button = 0;
			score++;
			sound.tone(294, 100);
		} else if (button_queue[next_index].button == 3 && arduboy.justPressed(LEFT_BUTTON)) {
			button_queue[next_index].button = 0;
			score++;
			sound.tone(330, 100);
		} else if (button_queue[next_index].button == 4 && arduboy.justPressed(RIGHT_BUTTON)) {
			button_queue[next_index].button = 0;
			score++;
			sound.tone(350, 100);
		} else if (button_queue[next_index].button == 5 && arduboy.justPressed(A_BUTTON)) {
			button_queue[next_index].button = 0;
			score++;
			sound.tone(392, 100);
		} else if (button_queue[next_index].button == 6 && arduboy.justPressed(B_BUTTON)) {
			button_queue[next_index].button = 0;
			score++;
			sound.tone(440, 100);
		} else if (anything_pressed()) {
			game_state = 2;
			sound.tones(game_over);
			return;
		}
	} else if (anything_pressed()) {
		game_state = 2;
		sound.tones(game_over);
		return;
	}
	
	
	// recreate after we know button it is clicked properly
	// this avoids regenerating even when the user didnt click the button yet
	if (anything_pressed()) {
		// find which y coordinate is the farthest down (highest in y)
		int lowest_button_y = 0;
		for (size_t i = 0; i < (sizeof(button_queue) / sizeof(button_queue[0])); i++) {
			if (button_queue[i].coords[1] > lowest_button_y) lowest_button_y = button_queue[i].coords[1];
		}
		
		// place this button just below the lowest button
		// this solves the error with misaligned buttons
		uint8_t random_button = random(MIN_RANDOMIZE, MAX_RANDOMIZE);
		button_queue[next_index].button = random_button; //1-6 L,R,U,D,A,B
		button_queue[next_index].coords[0] = random_button * 16;
		button_queue[next_index].coords[1] = lowest_button_y + 16;
	}
	

	// move the buttons
	for (size_t i = 0; i < (sizeof(button_queue) / sizeof(button_queue[0])); i++) {
		button_queue[i].coords[1]--;
		
		int x = button_queue[i].coords[0];
		int y = button_queue[i].coords[1];
		
		if (button_queue[i].coords[1] < -16) {
			game_state = 2;
			sound.tones(game_over);
			return;
		}

		// now draw the buttons
		if (button_queue[i].coords[1] >= -16) {
			switch (button_queue[i].button) {
				case 0:
					break;
				case 1:
					arduboy.drawBitmap(x, y, UP_ARROW, 16, 16, WHITE);
					break;
				case 2:
					arduboy.drawBitmap(x, y, DOWN_ARROW, 16, 16, WHITE);
					break;
				case 3:
					arduboy.drawBitmap(x, y, LEFT_ARROW, 16, 16, WHITE);
					break;
				case 4:
					arduboy.drawBitmap(x, y, RIGHT_ARROW, 16, 16, WHITE);
					break;
				case 5:
					arduboy.drawBitmap(x, y, A_ICON, 16, 16, WHITE);
					break;
				case 6:
					arduboy.drawBitmap(x, y, B_ICON, 16, 16, WHITE);
					break;
				default:
					break;
			}
		}
	}	
		
	// run timer once first pressed
	arduboy.setTextSize(1);
	arduboy.setCursor(0, HEIGHT - 7);
	arduboy.print(score);
}

void Game::win() {
	arduboy.setTextSize(1);
	arduboy.print(score);
    arduboy.print("PTS");
	timer_active = false;
	arduboy.setTextSize(3);
	arduboy.setCursor(30, 10);
	arduboy.println("GAME");
	arduboy.setCursor(30, 35);
	arduboy.println("OVER");
	
    if (arduboy.justPressed(A_BUTTON)) {
		write_highscore();
		read_highscore();
		game_state = 0;
    }
}

bool Game::anything_pressed() {
	if (arduboy.justPressed(UP_BUTTON) ||
		arduboy.justPressed(DOWN_BUTTON) ||
		arduboy.justPressed(LEFT_BUTTON) ||
		arduboy.justPressed(RIGHT_BUTTON) ||
		arduboy.justPressed(A_BUTTON) ||
		arduboy.justPressed(B_BUTTON)) {
		return true;
	}
	return false;
}

void Game::read_highscore() {
	// if our special addr matches, get the highscore
	if (EEPROM.read(EEP_CHECK_ADDR) != EEP_CHECK_VALUE) {
		EEPROM.update(EEP_CHECK_ADDR, EEP_CHECK_VALUE);
		EEPROM.put(HIGHSCORE_ADDR, 0);	
	}
	EEPROM.get(HIGHSCORE_ADDR, highscore);
}

void Game::write_highscore() {
	read_highscore();
	// if faster, or if never set
	if (score > highscore || highscore == 0) {
		EEPROM.put(HIGHSCORE_ADDR, score);
	}
}
