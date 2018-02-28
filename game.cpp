#import <Arduboy2.h>
#import "globals.h"
#import "sprites.h"
#import "game.h"

void Game::menu() {
	// draw title and highscore
	arduboy.setTextSize(2);
	arduboy.setCursor(0, 0);
	arduboy.println("DDRBOY");
	arduboy.setTextSize(1);
	arduboy.println("HIGHSCORE");
	arduboy.println(highscore_seconds);
	
	// center START text
	arduboy.setTextSize(2);
	arduboy.setCursor(0, HEIGHT - 15);
	arduboy.println("START");
	arduboy.drawBitmap(WIDTH - 16, HEIGHT - 16, A_ICON, 16, 16, WHITE);
	
	// play
	if (arduboy.justPressed(A_BUTTON)) {
		for (size_t i = 0; i < sizeof(button_queue); i++) {
			button_queue[i] = random(1, 7); //1-6 L,R,U,D,A,B
		}
		game_state = 1;
		timer_seconds = 0;
	}
	
	if (arduboy.everyXFrames(FRAMERATE) && arduboy.pressed(B_BUTTON)) {
		reset_highscore_hold++;
		if (reset_highscore_hold == 3) {
			EEPROM.put(HIGHSCORE_ADDR, 0);
			read_highscore();
			reset_highscore_hold = 0;
		}
		
	}
}

void Game::play() {
	// see if everything is finished
	bool all_clear = true;
	for (size_t i = 0; i < sizeof(button_queue); i++) {
		if (button_queue[i] >= 1 && button_queue[i] <= 6) {
			all_clear = false;
			break;
		}
	}
	// if it is, end the game
	if (all_clear) {
		game_state = 3;
	}

	// check for next button press needed
	uint8_t next_index;
	for (size_t i = 0; i < sizeof(button_queue); i++) {
		if (button_queue[i] >= 1 && button_queue[i] <= 6) {
			next_index = i;
			break; 
		}
	}
	
	// detect the press and check
	if (button_queue[next_index] == 1 && arduboy.justPressed(UP_BUTTON)) {
		button_queue[next_index] = 7;
	} else if (button_queue[next_index] == 2 && arduboy.justPressed(DOWN_BUTTON)) {
		button_queue[next_index] = 8;
	} else if (button_queue[next_index] == 3 && arduboy.justPressed(LEFT_BUTTON)) {
		button_queue[next_index] = 9;
	} else if (button_queue[next_index] == 4 && arduboy.justPressed(RIGHT_BUTTON)) {
		button_queue[next_index] = 10;
	} else if (button_queue[next_index] == 5 && arduboy.justPressed(A_BUTTON)) {
		button_queue[next_index] = 11;
	} else if (button_queue[next_index] == 6 && arduboy.justPressed(B_BUTTON)) {
		button_queue[next_index] = 12;
	} else if (button_queue[next_index] != 0 && anything_pressed()) {
		game_state = 2;
	}

	// draw the buttons
	for (size_t i = 0; i < sizeof(button_queue); i++) {
		// too lazy to automate but it works
		uint8_t x = i * 16;
		uint8_t y = 0;
		if (i >= 8) {
			x = i * 16 - 128;
			y = 16;
		}
		
		if (i >= 16) {
			x = i * 16 - 256;
			y = 32;
		}
		
		if (i >= 24) {
			x = i * 16 - 256 + 128;
			y = 48;
	}

	// now draw the buttons
	switch (button_queue[i]) {
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
		case 7:
			arduboy.drawBitmap(x, y, UP_PRESSED_ARROW, 16, 16, WHITE);
			break;
		case 8:
			arduboy.drawBitmap(x, y, DOWN_PRESSED_ARROW, 16, 16, WHITE);
			break;
		case 9:
			arduboy.drawBitmap(x, y, LEFT_PRESSED_ARROW, 16, 16, WHITE);
			break;
		case 10:
			arduboy.drawBitmap(x, y, RIGHT_PRESSED_ARROW, 16, 16, WHITE);
			break;
		case 11:
			arduboy.drawBitmap(x, y, A_PRESSED_ICON, 16, 16, WHITE);
			break;
		case 12:
			arduboy.drawBitmap(x, y, B_PRESSED_ICON, 16, 16, WHITE);
			break;
		default:
			break;
		}
	}
	
	// run timer once first pressed
	if (button_queue[0] >= 7) {
		timer_active = true;
		arduboy.setTextSize(1);
		arduboy.setCursor(0, 0);
		arduboy.print(timer_seconds);
	}
	
}

void Game::game_over() {
	timer_active = false;
	arduboy.setTextSize(3);
	arduboy.setCursor(30, 10);
	arduboy.println("GAME");
	arduboy.setCursor(30, 35);
	arduboy.println("OVER");
	arduboy.setTextSize(1);
	if (arduboy.justPressed(A_BUTTON)) {
		game_state = 0;
		read_highscore();
	}
}

void Game::win() {
	timer_active = false;
	arduboy.setTextSize(3);
	arduboy.print(timer_seconds);
	arduboy.setTextSize(2);
    arduboy.println("MS\n");
	int score_diff = timer_seconds - highscore_seconds;
	if (score_diff > 0 && highscore_seconds != 0) {
		arduboy.print(score_diff);
		arduboy.setTextSize(1);
		arduboy.print("MS");
		arduboy.setTextSize(2);
		arduboy.print("\nSHORT!");
	} else {
		arduboy.print("HIGHSCORE!");
	}
	
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
	EEPROM.get(HIGHSCORE_ADDR, highscore_seconds);
}

void Game::write_highscore() {
	read_highscore();
	// if faster, or if never set
	if (timer_seconds < highscore_seconds || highscore_seconds == 0) {
		EEPROM.put(HIGHSCORE_ADDR, timer_seconds);
	}
}
