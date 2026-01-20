#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#define SDL_TICKS_PASSED(A, B)  ((Sint32)((B) - (A)) <= 0)


// ================= CONFIG =================
const int IPF = 11;
int scale = 15;

// colors
const Uint32 cBG = 0x1e1e2e;
const Uint32 cPixel = 0xcdd6f4;

// ================= CHIP-8 CORE =================

// memory and index
unsigned char *mem;
unsigned short I = 0x0;

// index register and program counter
unsigned short PC = 0x200;

// registers and stack pointer
unsigned char V[16];
unsigned int Vx, Vy;
unsigned int oldVx, oldVy;
unsigned char SP = 0;

// stack
unsigned short stack[16] = {0};

// delay and sound timer
unsigned int DT = 0;
unsigned int ST = 0;

// VF macro
#define VF V[15]

// keypad
unsigned char key = 0x10;
unsigned char keypad[16] = {0};
int inst;


// ================= VIDEO HARDWARE =================

// resolution and render scale
const int width = 64;
const int height = 32;

// display array
unsigned char display[64][32] = {0};

// for screen clear
unsigned int fill = 0;


// ================= OPCODE =================

unsigned char opcode_low, opcode_high;
unsigned short opcode;

unsigned char n1, n2, n3, n4;

// ================= OPCODE TEMPORARIES =================

// sprite X, Y and height
int sX, sY;
int N;

// current row and column of rendered sprite
int row, col;

// coordinate on the display array
int cX, cY;

// sprite bit logic
unsigned char bits, curr_bit, curr_pixel;

// opcode decode helpers
int h, t, o;
int i;

// ================= SDL FRONTEND =================

SDL_Rect pixel = (SDL_Rect) {1, 1, 15, 15};



#include "font.h"
#include "draw.c"
#include "cpu.c"
#include "input.c"


int main (int argc, char *argv[]){
	
	int running = 1;
	int clock;

	//sanity check
	if (argc < 2){
		printf("Usage: ./chip8 rom.ch8 scale\n");
		return 1;
	}
	else if (argc == 3) {
		scale = atoi(argv[2]);
		pixel.w = scale;
		pixel.h = scale;
	}

	
	//allocate 4096 bytes of memory
	mem = malloc(0x1000);
	//copy fontset into virtual memory
	memcpy(mem, fontset, 0x80);
	
	//file pointer
	FILE* fptr;

	//open file, paste it to program start in the virtual memory, close file 
	fptr = fopen(argv[1], "rb");
	fread(mem + 0x200, 1, 0x1000 - 0x201, fptr);
	fclose(fptr);
	
	//create window, surface, pixel struct, event, fill bg color
	SDL_Window *pwindow = SDL_CreateWindow("Chip 8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, 0);
	SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);
	SDL_Event event;
	SDL_FillRect(psurface, NULL, cBG);
	
	//60Hz clock (1000/60)
	const Uint32 frame_interval = 1000 / 60;
	Uint32 next_frame = SDL_GetTicks() + frame_interval;

	Uint32 curr_tick = SDL_GetTicks();


	while (running) {
                //exit with close button
                while(SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) {running = 0;}
                        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {running = 0;}

                }

		SDL_PumpEvents();
                update_keys();
		
		curr_tick = SDL_GetTicks();

		if (SDL_TICKS_PASSED(curr_tick, next_frame)){
		if (DT != 0) {
                	DT--;
                }
                if (ST != 0) {
                	ST--;
                	printf("\a");
                	fflush(stdout);
		}
		Execute(IPF);
		Render(psurface);
		SDL_UpdateWindowSurface(pwindow);
		next_frame += frame_interval;
		}	

	}

	return 0;
}

