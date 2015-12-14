#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "math.h"

#define WIDTH 28
#define HEIGHT 28
#define NEWWIDTH 14
#define NEWHEIGHT 14

extern int seamCal[WIDTH][HEIGHT];
extern int carveMatrix[WIDTH][HEIGHT];
extern int newImage[NEWWIDTH*NEWHEIGHT];

void seamCarving(int a[]);
