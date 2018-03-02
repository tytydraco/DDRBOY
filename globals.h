#ifndef GLOBALS_H
#define GLOBALS_H

#define FRAMERATE 20

#define EEP_OFFSET 100
#define HIGHSCORE_ADDR EEPROM_STORAGE_SPACE_START + EEP_OFFSET
#define EEP_CHECK_ADDR EEPROM_STORAGE_SPACE_START + EEP_OFFSET - 1
#define EEP_CHECK_VALUE 1

#define MIN_RANDOMIZE 1
#define MAX_RANDOMIZE 7

#define MIN_DIFFICULTY 1
#define MAX_DIFFICULTY 4
#define DIFFICULTY_1_NAME "EASY"
#define DIFFICULTY_2_NAME "REGULAR"
#define DIFFICULTY_3_NAME "HARD"
#define DIFFICULTY_4_NAME "INSANE"
#define DIFFICULTY_1_FPS 20
#define DIFFICULTY_2_FPS 40
#define DIFFICULTY_3_FPS 60
#define DIFFICULTY_4_FPS 80

#endif
