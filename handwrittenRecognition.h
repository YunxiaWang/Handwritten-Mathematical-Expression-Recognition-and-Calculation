#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "math.h"

#define WIDTH 28
#define HEIGHT 28
#define NEWWIDTH 14
#define NEWHEIGHT 14


//int verticalSeamCal[WIDTH][HEIGHT];
//int horiontalSeamCal[NEWWIDTH][HEIGHT];
//int newVerticalMatrix[NEWWIDTH][HEIGHT];
//int newMatrix[NEWWIDTH][NEWHEIGHT];


//int originMatrix[WIDTH][HEIGHT];
int seamCal[WIDTH][HEIGHT];
int carveMatrix[WIDTH][HEIGHT];
int newImage[NEWWIDTH*NEWHEIGHT];
