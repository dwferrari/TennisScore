/*
	(С) Douglas Ferrari 2023
*/
#ifndef __SCORE_H__
#define __SCORE_H__

#define GAME_ZERO		0
#define GAME_FIFTEEN	1
#define GAME_THIRTY		2
#define GAME_FORTY		3
#define GAME_ADV		4

#define ZERO	0

#define SET_ONE		0
#define SET_TWO		1
#define SET_THREE	2

#define PLAYER_GREEN	0
#define PLAYER_RED		1

#define SERVE_RIGHT	0
#define SERVE_LEFT	1

#define NO	0
#define YES	1

// current data
struct score_ {
		void* 	ret_f;			//	return function address
		unsigned char player_green_game_score;
		unsigned char player_red_game_score;

		int player_green_set_score[3];
		int player_red_set_score[3];

		int current_set;
		unsigned char who_serve;
		unsigned char serve_side;
		Elf_proc_* proc;				//	process data pointer
};

void 	show_score_screen (void *return_screen);
void 	key_press_score_screen();
int 	dispatch_score_screen (void *param);
void	score_screen_job();
void    long_key_press_score_screen();

void draw_score_screen();
					
#endif