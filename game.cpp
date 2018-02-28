#import <Arduboy2.h>
#import "globals.h"
#import "sprites.h"
#import "game.h"

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
	arduboy.drawBitmap(WIDTH - 16, HEIGHT - 16, A_ICON, 16, 16, WHITE);
	
	// play
	if (arduboy.justPressed(A_BUTTON)) {
		randomize();
		game_state = 1;
		score = 0;
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

void Game::randomize(bool reset = true) {
	for (size_t i = 0; i < (sizeof(button_queue) / sizeof(button_queue[0])); i++) {
		if (reset || button_queue[i].button >= 7 || button_queue[i].button == 0) {
			uint8_t random_button = random(1, 7);
			button_queue[i].button = random_button; //1-6 L,R,U,D,A,B
			button_queue[i].coords[0] = random_button * 16;
			button_queue[i].coords[1] = HEIGHT + (i * 16);
		}
		
	}
}

void Game::play() {
	arduboy.setFrameRate(FRAMERATE + score / 2);
	// print the target buttons
	arduboy.drawBitmap(16, 0, UP_PRESSED_ARROW, 16, 16, WHITE);
	arduboy.drawBitmap(32, 0, DOWN_PRESSED_ARROW, 16, 16, WHITE);
	arduboy.drawBitmap(48, 0, LEFT_PRESSED_ARROW, 16, 16, WHITE);
	arduboy.drawBitmap(64, 0, RIGHT_PRESSED_ARROW, 16, 16, WHITE);
	arduboy.drawBitmap(80, 0, A_PRESSED_ICON, 16, 16, WHITE);
	arduboy.drawBitmap(96, 0, B_PRESSED_ICON, 16, 16, WHITE);
	
	if (button_queue[(sizeof(button_queue) / sizeof(button_queue[0])) - 1].button >= 7) {
		randomize(false);
	}
	
	// check for next button press needed
	uint8_t next_index;
	for (size_t i = 0; i < (sizeof(button_queue) / sizeof(button_queue[0])); i++) {
		if (button_queue[i].button >= 1 && button_queue[i].button <= 6 && button_queue[i].coords[1] >= -16) {
			next_index = i;
			break; 
		}
	}
		
	// detect the press and check
	if (button_queue[next_index].coords[1] >= -16 && button_queue[next_index].coords[1] <= 16) {
		if (button_queue[next_index].button == 1 && arduboy.justPressed(UP_BUTTON)) {
			button_queue[next_index].button = 7;
			score++;
		} else if (button_queue[next_index].button == 2 && arduboy.justPressed(DOWN_BUTTON)) {
			button_queue[next_index].button = 8;
			score++;
		} else if (button_queue[next_index].button == 3 && arduboy.justPressed(LEFT_BUTTON)) {
			button_queue[next_index].button = 9;
			score++;
		} else if (button_queue[next_index].button == 4 && arduboy.justPressed(RIGHT_BUTTON)) {
			button_queue[next_index].button = 10;
			score++;
		} else if (button_queue[next_index].button == 5 && arduboy.justPressed(A_BUTTON)) {
			button_queue[next_index].button = 11;
			score++;
		} else if (button_queue[next_index].button == 6 && arduboy.justPressed(B_BUTTON)) {
			button_queue[next_index].button = 12;
			score++;
		} else if (anything_pressed()) {
			game_state = 3;
			return;
		}
		
	} else if (anything_pressed()) {
		game_state = 3;
		return;
	}

	// move the buttons
	for (size_t i = 0; i < (sizeof(button_queue) / sizeof(button_queue[0])); i++) {
		button_queue[i].coords[1]--;
		
		int x = button_queue[i].coords[0];
		int y = button_queue[i].coords[1];
		
		if (button_queue[i].coords[1] < -16 && button_queue[i].button <= 6) {
			game_state = 3;
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
	}	
		
	// run timer once first pressed
	arduboy.setTextSize(1);
	arduboy.setCursor(0, 0);
	arduboy.print(score);
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
	arduboy.print(score);
	arduboy.setTextSize(2);
    arduboy.println("PTS\n");
	int score_diff = score - highscore;
	if (score_diff < 0 && highscore != 0) {
		arduboy.print(score_diff / -1);
		arduboy.setTextSize(1);
		arduboy.print("PTS");
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
	EEPROM.get(HIGHSCORE_ADDR, highscore);
}

void Game::write_highscore() {
	read_highscore();
	// if faster, or if never set
	if (score > highscore || highscore == 0) {
		EEPROM.put(HIGHSCORE_ADDR, score);
	}
}
