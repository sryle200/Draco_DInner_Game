#ifndef _GAMECONSTANTS_H
#define _GAMECONSTANTS_H

// Windows & SDL 
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <SDL.h>
// Image Texture loading library
#include <SDL_image.h>
// Font loading library
#include <SDL_ttf.h>
// Audio loading library
#include <SDL_mixer.h>
// Maths functions
#include <math.h>
#include <cmath>
#define PI 3.14159265
// Using Files
#include <fstream>

// STL Container & Algorithms
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
using namespace std::chrono;

using namespace std;

//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "Draco Dinner- The Game"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// This header file contains all the constants & enumarated types for the game
typedef struct
{
	float  X;
	float  Y;
}FPoint;

enum textType {SOLID, BLENDED, SHADED};
enum soundType {SFX, MUSIC};
enum gameState {QUIT, MENU, INSTRUCTIONS, PLAYING, HIGHSCORE, LOSE, SAVESCORE,RESTARTING };
enum btnTypes {EXIT, END, PLAY, SAVE, SETTINGS };

#endif