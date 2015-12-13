#define WIDTH 28
#define HEIGHT 28
#define NEWWIDTH 14
#define NEWHEIGHT 14

int seamCal[WIDTH][HEIGHT];
int carveMatrix[WIDTH][HEIGHT];
int newImage[NEWWIDTH*NEWHEIGHT];

void computeVerticalSeams(int a[][HEIGHT]) { //WIDTH*HEIGHT 28*28 first calculation
    for (int i = 0; i < WIDTH; i++)   //initiate first row
        seamCal[i][0] = a[i][0];
    for (int y = 1; y < HEIGHT; y++) { //compare and find the smallest value, going up to h
        for (int x = 0; x < WIDTH; x++) {
            seamCal[x][y] = a[x][y];
            if (x == 0)
                seamCal[x][y] += fmin(seamCal[x][y-1], seamCal[x+1][y-1]);
            else if (x == WIDTH-1)
                seamCal[x][y] += fmin(seamCal[x][y-1], seamCal[x-1][y-1]);
            else {
                //verticalSeamCal[x][y] += fmin(verticalSeamCal[x-1][y-1], verticalSeamCal[x][y-1], verticalSeamCal[x+1][y-1]);
                int min = fmin(seamCal[x+1][y-1], seamCal[x][y-1]);
                seamCal[x][y] += fmin(min, seamCal[x-1][y-1]);
            }
        }
    }
}

void computeHorizontalSeams(int a[][HEIGHT]) { //actual NEWWIDTH*HEIGHT 14*28 second calculation
    for (int j = 0; j < HEIGHT; j++)   //initiate first column
        seamCal[0][j] = a[0][j];
    for (int x = 1; x < NEWWIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            seamCal[x][y] = a[x][y];
            if (y == 0)
                seamCal[x][y] += fmin(seamCal[x-1][y], seamCal[x-1][y+1]);
            else if (y == HEIGHT-1)
                seamCal[x][y] += fmin(seamCal[x-1][y], seamCal[x-1][y-1]);
            else {
                //horiontalSeamCal[x][y] += fmin(horiontalSeamCal[x-1][y-1], horiontalSeamCal[x-1][y], horiontalSeamCal[x-1][y+1]);
                int min = fmin(seamCal[x-1][y+1], seamCal[x-1][y]);
                seamCal[x][y] += fmin(min, seamCal[x-1][y-1]);
            }
        }
    }
}

void removeVerticalSeams(){ //W=WIDTH, get NEWWIDTH*HEIGHT 14*28, first remove
     //first time calculate seam
    //find best column
    for (int d = 0; d < WIDTH-NEWWIDTH; d++) {
        computeVerticalSeams(carveMatrix);
//        printf("\n");
//        for (int j = HEIGHT - 1; j >= 0; j--) {
//            for (int i = 0; i < WIDTH; i++) {
//                printf("%d ", seamCal[i][j]);
//            }
//            printf("\n");
//        }
//        printf("\n");
        int bestCol = WIDTH - d - 1;
        for (int i = WIDTH - d - 1; i >= 0; i--) {
            //printf("removeVerticalSeams\n");
            if (seamCal[bestCol][HEIGHT-1] > seamCal[i][HEIGHT-1]) {
                bestCol = i;
            }
            //printf("bestCol = %d\n",bestCol);
        }
        for (int y = HEIGHT-1; y >= 0; y--) {
            //transfer value to new matrix
            for (int x = 0; x < WIDTH; x++) {
                if (x == bestCol) {
                    for (int i = bestCol; i < WIDTH; i++) {
                        carveMatrix[i][y] = carveMatrix[i+1][y];
                    }
                    carveMatrix[WIDTH - 1][y] = 1;
                    break;
                }//when reach min best column shift to left
                else carveMatrix[x][y] = carveMatrix[x][y];
            }
            // update best column for the next row
            if (y > 0) {
                int theMin = seamCal[bestCol][y-1];
                if (bestCol > 0 && seamCal[bestCol-1][y-1] < theMin)
                    bestCol = bestCol - 1;
                else if (bestCol < WIDTH-1 && seamCal[bestCol+1][y-1] < theMin)
                    bestCol = bestCol + 1;
            }
        }
//        printf("\n");
//        for (int j = HEIGHT - 1; j >= 0; j--) {
//            for (int i = 0; i < WIDTH; i++) {
//                printf("%d ", carveMatrix[i][j]);
//            }
//            printf("\n");
//        }
//        printf("\n");
    }    
}
// int (*func3())[ROW][COL] define a function returning 2D array
//(int (*)[ROW][COL])a return 2D array
// int (&a)[3][3] for parameter of function

void removeHorizontalSeams(){ //W=NEWWIDTH, get NEWWIDTH*NEWHEIGHT 14*14 second remove
     //first time calculate seam   
    //find best row
    for (int d = 0; d < HEIGHT-NEWHEIGHT; d++) {        
        computeHorizontalSeams(carveMatrix);
        int bestRow = 0;
        for (int j = 0; j < HEIGHT - d; j++) {
            if (seamCal[NEWWIDTH-1][bestRow] > seamCal[NEWWIDTH-1][j])
                bestRow = j;
        }
        for (int x = NEWWIDTH-1; x >= 0; x--) {
            //transfer value to new matrix
            for (int y = 0; y < HEIGHT; y++) {
                if (y == bestRow) {
                    for (int j = bestRow; j < HEIGHT; j++) {
                        carveMatrix[x][j] = carveMatrix[x][j+1]; //when reach min best row shift to lower
                    }
                    carveMatrix[x][HEIGHT - 1] = 1;
                    break;
                }
                else
                    carveMatrix[x][y] = carveMatrix[x][y];
            }
            // update best row for the next column
            if (x > 0) {
                int theMin = seamCal[x-1][bestRow];
                if (bestRow > 0 && seamCal[x-1][bestRow-1] < theMin)
                    bestRow = bestRow - 1;
                else if (bestRow < HEIGHT-1 && seamCal[x-1][bestRow+1] < theMin)
                    bestRow = bestRow + 1;
            }
        }
//        printf("\n");
//        for (int j = HEIGHT - 1; j >= 0; j--) {
//            for (int i = 0; i < WIDTH; i++) {
//                printf("%d ", carveMatrix[i][j]);
//            }
//            printf("\n");
//        }
//        printf("\n");
    }
//    printf("\n");
//    for (int j = HEIGHT - 1; j >= 0; j--) {
//        for (int i = 0; i < WIDTH; i++) {
//            printf("%d ", carveMatrix[i][j]);
//        }
//        printf("\n");
//    }
//    printf("\n");
}

void seamCarving(int a[]) {    
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            carveMatrix[x][y] = a[(HEIGHT - 1 - y) * WIDTH + x];
        }
    }
    removeVerticalSeams();//get 14*28 matrix
    removeHorizontalSeams();//get 14*14 matrix
    for (int y = 0; y < NEWHEIGHT; y++){
        for (int x = 0; x < NEWWIDTH; x++){
            newImage[x + NEWWIDTH * y] = carveMatrix[x][NEWHEIGHT - 1 - y];
        }
    }
}
