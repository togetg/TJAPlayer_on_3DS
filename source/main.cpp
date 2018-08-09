#include "header.h"
#include "time.h"
#include "notes.h"
#include "tja.h"
#include "audio.h"
#include "playback.h"

#define TOP_WIDTH  400
#define TOP_HEIGHT 240
#define BOTTOM_WIDTH  320
#define BOTTOM_HEIGHT 240

C2D_Sprite sprites[2];			//ただの画像用
C2D_Sprite notes_sprites[12];	//ノーツとか画像用
static C2D_SpriteSheet spriteSheet;
C2D_TextBuf g_dynamicBuf;


static void debug() {
	// Clear the dynamic text buffer
	C2D_TextBufClear(g_dynamicBuf);

	// Generate and draw dynamic text
	char buf[160];
	C2D_Text dynText;
	snprintf(buf, sizeof(buf), "太鼓の達人!");
	C2D_TextParse(&dynText, g_dynamicBuf, buf);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_WithColor, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f,C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f));
}

void main_init() {
	// Initialize the libs
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	init_main_music();
}

void main_exit() {
	// Deinitialize the scene
	C2D_TextBufDelete(g_dynamicBuf);

	// Deinitialize the libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	music_exit();
}

int main() {

	main_init();

	touchPosition tp;	//下画面タッチした座標

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	g_dynamicBuf = C2D_TextBufNew(4096);

	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	C2D_SpriteFromSheet(&sprites[0], spriteSheet, 0);
	C2D_SpriteSetCenter(&sprites[0], 0.5f, 0.5f);
	C2D_SpriteSetPos(&sprites[0], TOP_WIDTH / 2, TOP_HEIGHT / 2);
	C2D_SpriteFromSheet(&sprites[1], spriteSheet, 1);
	C2D_SpriteSetCenter(&sprites[1], 0.5f, 0.5f);
	C2D_SpriteSetPos(&sprites[1], BOTTOM_WIDTH / 2, BOTTOM_HEIGHT / 2);

	int cnt = 0, notes_cnt = 0;
	bool isNotesStart = false, isMusicStart = false;
	double FirstSecTime = 9999.0, offset, bpm, time; int measure = 4; double *p_offset = &offset, *p_bpm = &bpm; int *p_measure = &measure;

	/*tja_head_load(p_offset, p_bpm, p_measure);
	tja_notes_load();
	music_load();
	init_main_music();*/


	while (aptMainLoop()) {
		hidScanInput();
		hidTouchRead(&tp);

		unsigned int key = hidKeysDown();
		if (key & KEY_START) break;
		
		/*time = time_now(1);
		bool isPlayMain;
		bool* p_isPlayMain = &isPlayMain;
		
		if (cnt == 0) {

			FirstSecTime = (60.0 / bpm * measure)*(307.0 / 400.0) + 60.0 / bpm;
			play_main_music(p_isPlayMain);

		}

		if (offset >= 0 && (isNotesStart == false || isMusicStart == false)) {
			if (cnt == 0 && isNotesStart == false) //isNotesStart = true;
				if (cnt == (int)(offset * 60)) {
					//music_play(2);
					isMusicStart = true;
				}
		}
		else if (offset < 0 && (isNotesStart == false || isMusicStart == false)) {
			//if (cnt == 0) first_time = time;
			if (time >= FirstSecTime) {
				//music_play(2);
				isPlayMain = true;
				isMusicStart = true;
			}

			if (time >= (-1.0) * offset && isNotesStart == false) {
				isNotesStart = true;
			}
		}
		*/
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		C2D_TargetClear(top, C2D_Color32(0x00, 0x00, 0x00, 0xFF));	//上画面
		C2D_SceneBegin(top);
		C2D_DrawSprite(&sprites[0]);
		/*
		bool isDon = false, isKa = false;

		if ((((tp.px - 160)*(tp.px - 160) + (tp.py - 145)*(tp.py - 145)) <= 105 * 105 && key & KEY_TOUCH) ||
			key & KEY_B ||
			key & KEY_Y ||
			key & KEY_RIGHT ||
			key & KEY_DOWN ||
			key & KEY_CSTICK_LEFT ||
			key & KEY_CSTICK_DOWN) {//ドン

			isDon = true;
		}
		else if (key & KEY_TOUCH ||
			key & KEY_A ||
			key & KEY_X ||
			key & KEY_LEFT ||
			key & KEY_UP ||
			key & KEY_CSTICK_RIGHT ||
			key & KEY_CSTICK_UP ||
			key & KEY_R ||
			key & KEY_L ||
			key & KEY_ZR ||
			key & KEY_ZL) {//カツ

			isKa = true;
		}
		
		if (isNotesStart == true) {

			tja_to_notes(isDon, isKa, notes_cnt,spriteSheet);
			notes_cnt++;
		}
		*/
		debug();

		C2D_TargetClear(bottom, C2D_Color32(0x00, 0x00, 0x00, 0xFF));	//下画面
		C2D_SceneBegin(bottom);
		C2D_DrawSprite(&sprites[1]);
		/*
		if (isDon == true) {	//ドン

								//sftd_draw_text(font, 10, 10, RGBA8(255, 0, 0, 255), 20, "Don!");
			music_play(0);
		}
		if (isKa == true) {		//カツ

								//sftd_draw_text(font, 10, 10, RGBA8(0, 0, 255, 255), 20, "Ka!");
			music_play(1);
		}
		*/
		C3D_FrameEnd(0);
		cnt++;
	}

	main_exit();
	return 0;
}