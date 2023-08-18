/*
	(С) Douglas Ferrari 2023
	Tennis Score for  Amazfit Bip v0.1

*/

#include <libbip.h>
#include "score.h"

//	score screen menu structure
struct regmenu_ menu_score_screen = {
	55,
	1,
	0,
	dispatch_score_screen,
	key_press_score_screen,
	score_screen_job,
	0,
	show_score_screen,
	0,
	0
};

int main(int param0, char **argv) { //	variable argv is not defined
	show_score_screen((void *)param0);
}

void show_score_screen(void *param0) {
	struct score_ **score_p = get_ptr_temp_buf_2(); //	pointer to pointer to screen data
	struct score_ *score;							 //	pointer to screen data

	if ((param0 == *score_p) && get_var_menu_overlay()) { // return from overlay screen (incoming call, notification, alarm, target, etc.)

		score = *score_p; //	the pointer to the data must be saved to prevent the release of 
						  //	memory by the reg_menu function, 
		*score_p = NULL;  //	reset the pointer to be passed to the reg_menu function

		// we create a new screen, while the temp_buf_1 and temp_buf_2 pointers were equal to 0 and the memory was not freed
		reg_menu(&menu_score_screen, 0); // 	menu_overlay=0

		*score_p = score; //	restore the data pointer after the reg_menu function
	} else { // if the function was launched from the menu,

		// create a screen
		reg_menu(&menu_score_screen, 0);

		// allocate the necessary memory and place data in it
		*score_p = (struct score_ *)pvPortMalloc(sizeof(struct score_));
		score = *score_p; //	data pointer

		// clear memory for data
		_memclr(score, sizeof(struct score_));

		score->proc = param0;

		// remember the address of the pointer to the function to which you want to return after the completion of this screen
		if (param0 && score->proc->elf_finish) //	if a return pointer is passed, then return to it
			score->ret_f = score->proc->elf_finish;
		else //	if not, then on the dial
			score->ret_f = show_watchface;
		

		//initialize score data
		score->player_green_game_score = GAME_ZERO;
		score->player_red_game_score = GAME_ZERO;

		score->player_green_set_score = SET_ZERO;
		score->player_red_set_score = SET_ZERO;

		score->player_green_set_two_score = SET_ZERO;
		score->player_red_set_two_score = SET_ZERO;
		score->player_green_set_three_score = SET_ZERO;
		score->player_red_set_three_score = SET_ZERO;

		score->current_set = SET_ONE;
		score->who_serve = PLAYER_RED;
		score->serve_side = SERVE_RIGHT;
		
	}

	set_score_status();
}

void draw_game_score(unsigned char points, int pos_x, unsigned int pos_y){
	switch (points){
		case GAME_ZERO:
			text_out_font(9, "0", pos_x, pos_y, 1);	
			break;
		case GAME_FIFTEEN:
			text_out_font(9, "15", pos_x, pos_y, 1);	
			break;
		case GAME_THIRTY:
			text_out_font(9, "30", pos_x, pos_y, 1);	
			break;
		case GAME_FORTY:
			text_out_font(9, "40", pos_x, pos_y, 1);	
			break;
		case GAME_ADV:
			text_out_font(6, "ADV", pos_x, pos_y, 1);	
			break;
		default:
			break;
	}
}

void draw_set_score(char text, int pos_x, unsigned int pos_y){
	char text2[1];
	_sprintf(text2, "%d", text);
	text_out_font(8, text2, pos_x, pos_y, 1);
}

void set_score_status() {
	struct score_ **score_p = get_ptr_temp_buf_2(); //	pointer to pointer to screen data
	struct score_ *score = *score_p;				 //	pointer to screen data

		set_bg_color(COLOR_BLACK);// fill the screen black
		fill_screen_bg();

		set_bg_color(COLOR_RED);
		draw_filled_rect_bg(4, 4, 172,88);//draw a red rectangle on top half screen
		
		set_bg_color(COLOR_GREEN);
		draw_filled_rect_bg(4, 92, 172,172);//draw a gree rectangle on top half screen
		
		//draw a vertical white line on the serve side
		set_bg_color(COLOR_WHITE);
		if(score->serve_side == SERVE_LEFT){
			draw_filled_rect_bg(0, 0, 9,176);
		}else{
			draw_filled_rect_bg(167, 0, 176,176);
		}

		//no ideia why it is needed
		GUI_UC_SetEncodeUTF8();
		load_font();

		draw_game_score(score->player_red_game_score, 75, 25);			//draw game score for player red
		draw_game_score(score->player_green_game_score, 75, 120);		//draw game score for player green



		draw_set_score(score->player_red_set_score, 121, 62);
		draw_set_score(score->player_green_set_score, 121, 92);

		draw_set_score(score->player_red_set_two_score, 137, 62);
		draw_set_score(score->player_green_set_two_score, 137, 92);

		draw_set_score(score->player_red_set_three_score, 153, 62);
		draw_set_score(score->player_green_set_three_score, 153, 92);

		switch (score->who_serve){
			case PLAYER_RED:
				text_out_font(6, "@", 30, 30, 1);
				break;
			case PLAYER_GREEN:
				text_out_font(6, "@", 30, 120, 1);
				break;
			default:
				break;
		}

		// do not exit when idle
		set_display_state_value(8, 1);
		set_display_state_value(4, 1);
		set_display_state_value(2, 1);
				
		// set backlight to on with 60 percent bright
		set_backlight_percent(60);
		set_backlight_state(1);
		
		//	turn off idle timer
		set_update_period(0, 0);
}

void key_press_score_screen(){
	struct score_ **score_p = get_ptr_temp_buf_2(); //	pointer to pointer to screen data
	struct score_ *score = *score_p;				 //	pointer to screen data

	set_score_status();
	repaint_screen_lines(0, 176);
	show_menu_animate(score->ret_f, (int)show_score_screen, ANIMATE_RIGHT);
};

void score_screen_job(){

	struct score_ **score_p = get_ptr_temp_buf_2(); //	pointer to pointer to screen data
	struct score_ *score = *score_p;				 //	pointer to screen data

	set_score_status();
	repaint_screen_lines(0, 176);
	show_menu_animate(score->ret_f, (int)show_score_screen, ANIMATE_RIGHT);
}

//check game state and score
char handle_game_score(unsigned char *scorer, unsigned char *opponent) {
	char game_ended = NO;
	if(*scorer == GAME_FORTY && *opponent < GAME_FORTY ){// scorer com 40 e opponent abaixo de 40 = scorer ganha
		game_ended = YES;
	}else if(*scorer == GAME_FORTY && *opponent == GAME_FORTY ){//ambos com 40 = scorer com vantagem
		*scorer = GAME_ADV;
		game_ended = NO;
	}else if(*scorer == GAME_ADV && *opponent == GAME_FORTY ){//scorer com vantagem e opponent com 40 = scorer ganha
		game_ended = YES;
	}else if(*scorer == GAME_FORTY && *opponent == GAME_ADV){// opponent com vantagem e scorer com 40 = opponent volta para 40
		game_ended = NO;
		*opponent = GAME_FORTY;
	}else if(*scorer < GAME_FORTY && *opponent <= GAME_FORTY){// scorer com menos de 40 e opponent com 40 ou menos = +1 ponto para scorer
		game_ended = NO;
		*scorer += 1;
	}
	return game_ended;
}

int dispatch_score_screen(void *param) {
	struct score_ **score_p = get_ptr_temp_buf_2(); // pointer to pointer to screen data
	struct score_ *score = *score_p; 				// pointer to screen data

	struct gesture_ *gest = param;
	int result = 0;

	switch (gest->gesture) {
		case GESTURE_CLICK: {
			//Vibrate on touch
			vibrate(1, 40, 0);

			char game_ended = NO;
			char who_won;

			if(gest->touch_pos_y > VIDEO_Y/2 ){//if the touch is on top half of the screen then player green score, if game ended player green wins the game
				game_ended = handle_game_score(&score->player_green_game_score, &score->player_red_game_score);
				who_won = PLAYER_GREEN;
			}else{//or then player red score, if game ended player red wins the game
				game_ended = handle_game_score(&score->player_red_game_score, &score->player_green_game_score);
				who_won = PLAYER_RED;
			}

			score->serve_side = score->serve_side ? SERVE_RIGHT : SERVE_LEFT;
			if(game_ended == YES){
				switch (score->current_set){
					case SET_ONE:
						if(who_won == PLAYER_GREEN){
							score->player_green_set_score+=1;
							if(score->player_green_set_score >= 6){
								score->current_set +=1;
							}
						}else{
							score->player_red_set_score+=1;
							if(score->player_red_set_score >= 6){
								score->current_set +=1;
							}
						}
						break;
					case SET_TWO:
						if(who_won == PLAYER_GREEN){
							score->player_green_set_two_score+=1;
							if(score->player_green_set_two_score >= 6){
								score->current_set +=1;
							}
						}else{
							score->player_red_set_two_score+=1;
							if(score->player_red_set_two_score >= 6){
								score->current_set +=1;
							}
						}
						break;
					case SET_THREE:
						if(who_won == PLAYER_GREEN){
							score->player_green_set_three_score+=1;
							if(score->player_green_set_three_score >= 6){
								score->current_set +=1;
							}
						}else{
							score->player_red_set_three_score+=1;
							if(score->player_red_set_three_score >= 6){
								score->current_set +=1;
							}
						}
						break;
					default:
						if(who_won == PLAYER_GREEN){
							score->player_green_set_three_score+=1;
						}else{
							score->player_red_set_three_score+=1;
						}
						break;
				}
				
				score->player_green_game_score = GAME_ZERO;
				score->player_red_game_score = GAME_ZERO;
				score->who_serve = score->who_serve ? PLAYER_GREEN : PLAYER_RED;
				score->serve_side = SERVE_RIGHT;
			}

			set_score_status();
			repaint_screen_lines(0, 176);
			break;
		};
		default: { // Something went wrong...
			break;
		};
	}

	return result;
};