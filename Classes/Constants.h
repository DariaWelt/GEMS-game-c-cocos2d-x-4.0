#pragma once

#define FONT_PATH "fonts/Snap ITC.ttf"
#define SCENE_TRANSITION_TIME 0.3f // sec

#define SOUND_ON_BUTTON_REST "res/Sound_On_Button_AtRest.png"
#define SOUND_OFF_BUTTON_REST "res/Sound_Off_Button_AtRest.png"
#define SOUND_ON_BUTTON_PRESSED "res/Sound_On_Button_Pressed.png"
#define SOUND_OFF_BUTTON_PRESSED "res/Sound_Off_Button_Pressed.png"
#define GAME_BUTTON_EXIT_REST "res/Game_Button_Exit.png"
#define GAME_BUTTON_EXIT_PRESSED "res/Game_Button_Exit_Pressed.png"
#define GAME_BUTTON_INFO_REST "res/Menu_Button_Info.png"
#define GAME_BUTTON_INFO_PRESSED "res/Menu_Button_Info_Pressed.png"

#define SPLASH_SCENE_BACKGROUND_PATH "res/Splash_Background.png"
#define SPLASH_SCENE_SHOW_TIME 2.0f // sec

#define MAIN_MENU_BACKGROUND_PATH "res/Main_Menu_Background.png"
#define MAIN_MENU_START_BUTTON_REST_PATH "res/Main_Menu_Start_Button_AtRest.png"
#define MAIN_MENU_START_BUTTON_PRESSED_PATH "res/Main_Menu_Start_Button_Pressed.png"
#define MAIN_MENU_TITLE_PATH "res/Game_Title.png"
#define GAME_INFO_BACKGROUND_PATH "res/Info_Item_Background.png"
#define GAME_INFO_DESIGN_PROVIDER_PATH "res/FreePik label.png"

#define GAME_BACKGROUND_PATH "res/Game_Background.png"
#define GAME_FIELD_ROWS 9
#define GAME_FIELD_COLS 8
#define GAME_FIELD_POS_X 546.0f // pixels
#define GAME_FIELD_POS_Y 80.0f // pixels
#define GAME_FIELD_SIZE_W 1298.0f // pixels
#define GAME_FIELD_SIZE_H 1367.0f // pixels

#define GAME_BLOCKED_PROBABILITY 0.4f
#define GAME_SCORE_KOEF 1
#define GEMS_ATLAS_PATH "res/Game_Gemses.png"
#define GEMS_COLOUR_NUMBER 7
#define GEMS_BONUS_NUMBER 4
#define GEMS_EFFECTS_ATLAS_PATH "res/Game_Gemses_Effects.png"
#define GEMS_ATLAS_ELEMENT_SIZE 495.0f // pixels

#define BLOCK_FADE_IN_TIME 0.5f // sec
#define BLOCK_VELOCITY 400.0f // pixel/ sec


#define GAME_WON_BACKGROUND_PATH "res/Game_Won_Background.png"
#define GAME_WON_MAIN_COMPONENT_PATH "res/Game_Won_Field.png"
#define GAME_WON_RETRY_BUTTON_REST_PATH "res/Game_Won_Replay_Button_Rest.png"
#define GAME_WON_RETRY_BUTTON_PRESSED_PATH "res/Game_Won_Replay_Button_Pressed.png"
#define GAME_WON_TOMENU_BUTTON_REST_PATH "res/Game_Won_ToMenu_Button_Rest.png"
#define GAME_WON_TOMENU_BUTTON_PRESSED_PATH "res/Game_Won_ToMenu_Button_Pressed.png"

enum GameState {
	GAME_STATE_PLAYING = 1,
	GAME_STATE_PAUSED,
	GAME_STATE_WON,
	GAME_STATE_FALLING_BLOCK
};

enum BlockBehaviorType {
	BLOCK_TYPE_SIMPLE,
	BLOCK_TYPE_BLOCKED,
	BLOCK_TYPE_BONUS_BOMB,
	BLOCK_TYPE_BONUS_REPAINT,
	BLOCK_TYPE_BONUS_HORIZ_DEL,
	BLOCK_TYPE_BONUS_VERTIC_DEL
};