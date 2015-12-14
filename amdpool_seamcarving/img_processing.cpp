#include "img_processing.h"

unsigned char *read_bmp(char *fname,int* _w, int* _h)
{
    unsigned char head[54];
    FILE *f = fopen(fname,"rb");
    
    // BMP header is 54 bytes
    fread(head, 1, 54, f);
    
    int w = head[18] + ( ((int)head[19]) << 8) + ( ((int)head[20]) << 16) + ( ((int)head[21]) << 24);
    int h = head[22] + ( ((int)head[23]) << 8) + ( ((int)head[24]) << 16) + ( ((int)head[25]) << 24);
    
    // lines are aligned on 4-byte boundary
    int lineSize = (w / 32) * 4;
    if (w % 32) lineSize += 4;
    int fileSize = lineSize * h;
    
    unsigned char *img = (unsigned char *)malloc(w * h), *data = (unsigned char *)malloc(fileSize);
    
    // skip the header
    fseek(f,54,SEEK_SET);
    
    // skip palette - two rgb quads, 8 bytes
    fseek(f, 8, SEEK_CUR);
    
    // read data
    fread(data,1,fileSize,f);
    
    // decode bits
    int i, j, k, rev_j;
    for(j = 0, rev_j = h - 1; j < h ; j++, rev_j--) {
        for(i = 0 ; i <= w / 8; i++) {
            int fpos = j * lineSize + i, pos = rev_j * w + i * 8;
            for(k = 0 ; k < 8 ; k++)
                if( i < w / 8  ||  k >= 8 - (w % 8) )
                    img[pos + (7 - k)] = (data[fpos] >> k ) & 1;
        }
    }
    
    free(data);
    *_w = w; *_h = h;
    return img;
}

void img_seg(unsigned char* img_data, int w, int h, int head[], int tail[])
{
    unsigned char arr[h];
    unsigned char count_number = 0;
    bool head_mode = true;
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            arr[j] = img_data[j * w + i];
        }
        int count = 0;
        for (int k = 0; k < h; k++) {
            if (head_mode) {
                if (arr[k] == 0) {// '0' for '1'
                    head_mode = false;
                    head[count_number] = i;
                }
            }
            else {
                if (arr[k] == 1) {// '1' for '0'
                    count ++;
                }
                if (count == h) {
                    head_mode = true;
                    tail[count_number] = i;
                    count_number++;
                }
            }
        }
    }
}

// scale for seam carving matrix
int scale(int position) {
    return (int)(((position - 6.5) * SCALE + 6.5) + 0.5);
}

int img_processing(char *bmp_file)
{
    int w, h, t, b;
    int count_number = 0;
    int top[7] = {-1,-1,-1,-1,-1,-1,-1};
    int bot[7] = {-1,-1,-1,-1,-1,-1,-1};
    int head[7] = {-1,-1,-1,-1,-1,-1,-1};
    int tail[7] = {-1,-1,-1,-1,-1,-1,-1};
    unsigned char* img = read_bmp(bmp_file, &w, &h);
    
    img_seg(img, w, h, head, tail);
    
    for(int j = 0 ; j < h ; j++)
    {
        for(int i = 0 ; i < w ; i++)
            printf("%c ", img[j * w + i] ? '0' : '1' );
        
        printf("\n");
    }
    
    unsigned char *character_after_seg0 = (unsigned char *)malloc((tail[0] - head[0]) * HEIGHT);
    unsigned char *character_after_seg1 = (unsigned char *)malloc((tail[1] - head[1]) * HEIGHT);
    unsigned char *character_after_seg2 = (unsigned char *)malloc((tail[2] - head[2]) * HEIGHT);
    unsigned char *character_after_seg3 = (unsigned char *)malloc((tail[3] - head[3]) * HEIGHT);
    unsigned char *character_after_seg4 = (unsigned char *)malloc((tail[4] - head[4]) * HEIGHT);
    unsigned char *character_after_seg5 = (unsigned char *)malloc((tail[5] - head[5]) * HEIGHT);
    unsigned char *character_after_seg6 = (unsigned char *)malloc((tail[6] - head[6]) * HEIGHT);
    
    int char_bf_sc0[HEIGHT*WIDTH] = {0};
    int char_bf_sc1[HEIGHT*WIDTH] = {0};
    int char_bf_sc2[HEIGHT*WIDTH] = {0};
    int char_bf_sc3[HEIGHT*WIDTH] = {0};
    int char_bf_sc4[HEIGHT*WIDTH] = {0};
    int char_bf_sc5[HEIGHT*WIDTH] = {0};
    int char_bf_sc6[HEIGHT*WIDTH] = {0};
    
    int scaled0[NEWHEIGHT*NEWWIDTH] = {0};
    int scaled1[NEWHEIGHT*NEWWIDTH] = {0};
    int scaled2[NEWHEIGHT*NEWWIDTH] = {0};
    int scaled3[NEWHEIGHT*NEWWIDTH] = {0};
    int scaled4[NEWHEIGHT*NEWWIDTH] = {0};
    int scaled5[NEWHEIGHT*NEWWIDTH] = {0};
    int scaled6[NEWHEIGHT*NEWWIDTH] = {0};

    for (int i = 0; i < HEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < tail[0] - head[0]; j++) { // count for fixed columns
            character_after_seg0[i * (tail[0] - head[0]) + j] = img[i * w + j + head[0]] ? '0' : '1';
        }
    }
    for (int i = 0; i < HEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < tail[1] - head[1]; j++) { // count for fixed columns
            character_after_seg1[i * (tail[1] - head[1]) + j] = img[i * w + j + head[1]] ? '0' : '1';
        }
    }
    for (int i = 0; i < HEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < tail[2] - head[2]; j++) { // count for fixed columns
            character_after_seg2[i * (tail[2] - head[2]) + j] = img[i * w + j + head[2]] ? '0' : '1';
        }
    }
    for (int i = 0; i < HEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < tail[3] - head[3]; j++) { // count for fixed columns
            character_after_seg3[i * (tail[3] - head[3]) + j] = img[i * w + j + head[3]] ? '0' : '1';
        }
    }
    for (int i = 0; i < HEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < tail[4] - head[4]; j++) { // count for fixed columns
            character_after_seg4[i * (tail[4] - head[4]) + j] = img[i * w + j + head[4]] ? '0' : '1';
        }
    }
    for (int i = 0; i < HEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < tail[5] - head[5]; j++) { // count for fixed columns
            character_after_seg5[i * (tail[5] - head[5]) + j] = img[i * w + j + head[5]] ? '0' : '1';
        }
    }
    for (int i = 0; i < HEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < tail[6] - head[6]; j++) { // count for fixed columns
            character_after_seg6[i * (tail[6] - head[6]) + j] = img[i * w + j + head[6]] ? '0' : '1';
        }
    }
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[0] - head[0]; j++) {
            printf("%c ", character_after_seg0[i * (tail[0] - head[0]) + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[1] - head[1]; j++) {
            printf("%c ", character_after_seg1[i * (tail[1] - head[1]) + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[2] - head[2]; j++) {
            printf("%c ", character_after_seg2[i * (tail[2] - head[2]) + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[3] - head[3]; j++) {
            printf("%c ", character_after_seg3[i * (tail[3] - head[3]) + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[4] - head[4]; j++) {
            printf("%c ", character_after_seg4[i * (tail[4] - head[4]) + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[5] - head[5]; j++) {
            printf("%c ", character_after_seg5[i * (tail[5] - head[5]) + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[6] - head[6]; j++) {
            printf("%c ", character_after_seg6[i * (tail[6] - head[6]) + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    //
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[0] - head[0]; j++) {
            if (character_after_seg0[i * (tail[0] - head[0]) + j] == '1') {
                char_bf_sc0[i * WIDTH +j] = 1;
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[1] - head[1]; j++) {
            if (character_after_seg1[i * (tail[1] - head[1]) + j] == '1') {
                char_bf_sc1[i * WIDTH +j] = 1;
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[2] - head[2]; j++) {
            if (character_after_seg2[i * (tail[2] - head[2]) + j] == '1') {
                char_bf_sc2[i * WIDTH +j] = 1;
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[3] - head[3]; j++) {
            if (character_after_seg3[i * (tail[3] - head[3]) + j] == '1') {
                char_bf_sc3[i * WIDTH +j] = 1;
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[4] - head[4]; j++) {
            if (character_after_seg4[i * (tail[4] - head[4]) + j] == '1') {
                char_bf_sc4[i * WIDTH +j] = 1;
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[5] - head[5]; j++) {
            if (character_after_seg5[i * (tail[5] - head[5]) + j] == '1') {
                char_bf_sc5[i * WIDTH +j] = 1;
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < tail[6] - head[6]; j++) {
            if (character_after_seg6[i * (tail[6] - head[6]) + j] == '1') {
                char_bf_sc6[i * WIDTH +j] = 1;
            }
        }
    }
    
    printf("\nprint out the matrx after seam carving process\n\n");
    
    seamCarving(char_bf_sc0);
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", newImage[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            if (newImage[i * NEWWIDTH + j] == 1) {
                scaled0[scale(i) * NEWWIDTH + scale(j)] = 1;
            }
        }
    }
    
    seamCarving(char_bf_sc1);
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", newImage[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < NEWWIDTH; j++) { // count for fixed columns
            if (newImage[i * NEWWIDTH + j] == 1) {
                scaled1[scale(i) * NEWWIDTH + scale(j)] = 1;
            }
        }
    }
    
    seamCarving(char_bf_sc2);
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", newImage[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < NEWWIDTH; j++) { // count for fixed columns
            if (newImage[i * NEWWIDTH + j] == 1) {
                scaled2[scale(i) * NEWWIDTH + scale(j)] = 1;
            }
        }
    }
    
    seamCarving(char_bf_sc3);
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", newImage[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < NEWWIDTH; j++) { // count for fixed columns
            if (newImage[i * NEWWIDTH + j] == 1) {
                scaled3[scale(i) * NEWWIDTH + scale(j)] = 1;
            }
        }
    }
    
    seamCarving(char_bf_sc4);
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", newImage[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < NEWWIDTH; j++) { // count for fixed columns
            if (newImage[i * NEWWIDTH + j] == 1) {
                scaled4[scale(i) * NEWWIDTH + scale(j)] = 1;
            }
        }
    }
    
    seamCarving(char_bf_sc5);
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", newImage[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < NEWWIDTH; j++) { // count for fixed columns
            if (newImage[i * NEWWIDTH + j] == 1) {
                scaled5[scale(i) * NEWWIDTH + scale(j)] = 1;
            }
        }
    }
    
    seamCarving(char_bf_sc6);
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", newImage[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) { // count for HEIGHT rows
        for (int j = 0; j < NEWWIDTH; j++) { // count for fixed columns
            if (newImage[i * NEWWIDTH + j] == 1) {
                scaled6[scale(i) * NEWWIDTH + scale(j)] = 1;
            }
        }
    }
    
    printf("\n");
    printf("\nprint out the scaled matrix\n\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", scaled0[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", scaled1[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", scaled2[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", scaled3[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", scaled4[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", scaled5[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", scaled6[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    
    printf("\n");
    printf("\nprint out the bit vector of scaled matrix\n\n");
    
    std::ofstream outfile;
    outfile.open("testing_set.dat");

    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d", scaled0[i * NEWWIDTH + j]);
            outfile << scaled0[i * NEWWIDTH +j];
        }
        //printf("\n");
    }
    printf("\n");
    outfile << std::endl;
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d", scaled1[i * NEWWIDTH + j]);
            outfile << scaled1[i * NEWWIDTH +j];
        }
        //printf("\n");
    }
    printf("\n");
    outfile << std::endl;
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d", scaled2[i * NEWWIDTH + j]);
            outfile << scaled2[i * NEWWIDTH +j];
        }
        //printf("\n");
    }
    printf("\n");
    outfile << std::endl;
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d", scaled3[i * NEWWIDTH + j]);
            outfile << scaled3[i * NEWWIDTH +j];
        }
        //printf("\n");
    }
    printf("\n");
    outfile << std::endl;
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d", scaled4[i * NEWWIDTH + j]);
            outfile << scaled4[i * NEWWIDTH +j];
        }
        //printf("\n");
    }
    printf("\n");
    outfile << std::endl;
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d", scaled5[i * NEWWIDTH + j]);
            outfile << scaled5[i * NEWWIDTH +j];
        }
        //printf("\n");
    }
    printf("\n");
    outfile << std::endl;
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d", scaled6[i * NEWWIDTH + j]);
            outfile << scaled6[i * NEWWIDTH +j];
        }
        //printf("\n");
    }
    printf("\n");
    outfile << std::endl;
    
    outfile.close();
    
    free(character_after_seg0);
    free(character_after_seg1);
    free(character_after_seg2);
    free(character_after_seg3);
    free(character_after_seg4);
    free(character_after_seg5);
    free(character_after_seg6);
    
    free(img);
    return 0;
}
