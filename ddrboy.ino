#import <Arduboy2.h>
#import "globals.h"
#import "game.h"

Arduboy2 arduboy;
Game game(arduboy);

void setup() {
	arduboy.begin();
	arduboy.setFrameRate(FRAMERATE);
	arduboy.setTextWrap(true);
	arduboy.initRandomSeed();
	game.read_highscore();
}

void loop() {
	if (!(arduboy.nextFrame())) return;
	arduboy.pollButtons();
	arduboy.clear();

	switch (game.game_state) {
		case 0:
			game.menu();
			break;
		
		case 1:
			game.play();
			break;

		case 2:
			game.game_over();
			break;

		case 3:
			game.win();
			break;

		default:
			break;
	}

	// increase the timer if it is active (1000 because ms)
	if (game.timer_active) game.timer_seconds += 1000 / FRAMERATE;
	arduboy.display();
}
