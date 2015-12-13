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

unsigned char *char_trim(unsigned char *character, int h, int w, int *t, int *b) {
    unsigned char arr[w];
    int top = -1, bot = -1;
    bool top_mode = true;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            arr[j] = character[i * w + j];
        }
        int count = 0;
        for (int k = 0; k < w; k++) {
            if (top_mode) {
                if (arr[k] == '1') {
                    top_mode = false;
                    top = i;
                }
            }
            else {
                if (arr[k] == '0') {
                    count++;
                }
                if (count == w) {
                    top_mode = true;
                    bot = i;
                }
            }
        }
    }
    
    //unsigned char *img = (unsigned char *)malloc((bot - top) * w);
    unsigned char *img = (unsigned char *)malloc((bot - top) * w);
    for (int i = 0; i < bot - top; i++) {
        for (int j = 0; j < w; j++) {
            img[i * w + j] = character[(i + top) * w + j];
        }
    }
    
    *t = top; *b = bot;
    return img;
}

// shrink for directly scale
int shrink_row(int row, int w, int h) {
    float scale = (float)NEWHEIGHT/h > 1 ? 1.0 : (float)NEWHEIGHT/h - 0.2;
    return (int)(((row - (h - 1)/2.0) * scale + 6.5) + 0.5);
}

int shrink_col(int col, int w, int h) {
    float scale = (float)NEWHEIGHT/h > 1 ? 1.0 : (float)NEWHEIGHT/h - 0.2;
    return (int)(((col - (w - 1)/2.0) * scale + 6.5) + 0.5);
}

//int main(int argc, char *argv[])
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
    
//    for(int j = 0 ; j < h ; j++)
//    {
//        for(int i = 0 ; i < w ; i++)
//            printf("%c ", img[j * w + i] ? '0' : '1' );
//        
//        printf("\n");
//    }
    
    unsigned char *character_after_seg0 = (unsigned char *)malloc((tail[0] - head[0]) * HEIGHT);
    unsigned char *character_after_seg1 = (unsigned char *)malloc((tail[1] - head[1]) * HEIGHT);
    unsigned char *character_after_seg2 = (unsigned char *)malloc((tail[2] - head[2]) * HEIGHT);
    unsigned char *character_after_seg3 = (unsigned char *)malloc((tail[3] - head[3]) * HEIGHT);
    unsigned char *character_after_seg4 = (unsigned char *)malloc((tail[4] - head[4]) * HEIGHT);
    unsigned char *character_after_seg5 = (unsigned char *)malloc((tail[5] - head[5]) * HEIGHT);
    unsigned char *character_after_seg6 = (unsigned char *)malloc((tail[6] - head[6]) * HEIGHT);
    
    int auto_shrinked0[NEWHEIGHT*NEWWIDTH] = {0};
    int auto_shrinked1[NEWHEIGHT*NEWWIDTH] = {0};
    int auto_shrinked2[NEWHEIGHT*NEWWIDTH] = {0};
    int auto_shrinked3[NEWHEIGHT*NEWWIDTH] = {0};
    int auto_shrinked4[NEWHEIGHT*NEWWIDTH] = {0};
    int auto_shrinked5[NEWHEIGHT*NEWWIDTH] = {0};
    int auto_shrinked6[NEWHEIGHT*NEWWIDTH] = {0};
    
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
    
    unsigned char *char_after_trim0 = char_trim(character_after_seg0, HEIGHT, tail[0] - head[0], &t, &b);
    top[count_number] = t; bot[count_number] = b; count_number++;
    
    unsigned char *char_after_trim1 = char_trim(character_after_seg1, HEIGHT, tail[1] - head[1], &t, &b);
    top[count_number] = t; bot[count_number] = b; count_number++;
    
    unsigned char *char_after_trim2 = char_trim(character_after_seg2, HEIGHT, tail[2] - head[2], &t, &b);
    top[count_number] = t; bot[count_number] = b; count_number++;
    
    unsigned char *char_after_trim3 = char_trim(character_after_seg3, HEIGHT, tail[3] - head[3], &t, &b);
    top[count_number] = t; bot[count_number] = b; count_number++;
    
    unsigned char *char_after_trim4 = char_trim(character_after_seg4, HEIGHT, tail[4] - head[4], &t, &b);
    top[count_number] = t; bot[count_number] = b; count_number++;
    
    unsigned char *char_after_trim5 = char_trim(character_after_seg5, HEIGHT, tail[5] - head[5], &t, &b);
    top[count_number] = t; bot[count_number] = b; count_number++;
    
    unsigned char *char_after_trim6 = char_trim(character_after_seg6, HEIGHT, tail[6] - head[6], &t, &b);
    top[count_number] = t; bot[count_number] = b; count_number++;
    
    //
    for (int i = 0; i < bot[0] - top[0]; i++) {
        for (int j = 0; j < tail[0] - head[0]; j++) {
            if (char_after_trim0[i * (tail[0] - head[0]) + j] == '1') {
                auto_shrinked0[shrink_row(i,tail[0] - head[0],bot[0] - top[0]) * NEWWIDTH + shrink_col(j,tail[0] - head[0],bot[0] - top[0])] = 1;
            }
        }
    }
    for (int i = 0; i < bot[1] - top[1]; i++) {
        for (int j = 0; j < tail[1] - head[1]; j++) {
            if (char_after_trim1[i * (tail[1] - head[1]) + j] == '1') {
                auto_shrinked1[shrink_row(i,tail[1] - head[1],bot[1] - top[1]) * NEWWIDTH + shrink_col(j,tail[1] - head[1],bot[1] - top[1])] = 1;
            }
        }
    }
    for (int i = 0; i < bot[2] - top[2]; i++) {
        for (int j = 0; j < tail[2] - head[2]; j++) {
            if (char_after_trim2[i * (tail[2] - head[2]) + j] == '1') {
                auto_shrinked2[shrink_row(i,tail[2] - head[2],bot[2] - top[2]) * NEWWIDTH + shrink_col(j,tail[2] - head[2],bot[2] - top[2])] = 1;
            }
        }
    }
    for (int i = 0; i < bot[3] - top[3]; i++) {
        for (int j = 0; j < tail[3] - head[3]; j++) {
            if (char_after_trim3[i * (tail[3] - head[3]) + j] == '1') {
                auto_shrinked3[shrink_row(i,tail[3] - head[3],bot[3] - top[3]) * NEWWIDTH + shrink_col(j,tail[3] - head[3],bot[3] - top[3])] = 1;
            }
        }
    }
    for (int i = 0; i < bot[4] - top[4]; i++) {
        for (int j = 0; j < tail[4] - head[4]; j++) {
            if (char_after_trim4[i * (tail[4] - head[4]) + j] == '1') {
                auto_shrinked4[shrink_row(i,tail[4] - head[4],bot[4] - top[4]) * NEWWIDTH + shrink_col(j,tail[4] - head[4],bot[4] - top[4])] = 1;
            }
        }
    }
    for (int i = 0; i < bot[5] - top[5]; i++) {
        for (int j = 0; j < tail[5] - head[5]; j++) {
            if (char_after_trim5[i * (tail[5] - head[5]) + j] == '1') {
                auto_shrinked5[shrink_row(i,tail[5] - head[5],bot[5] - top[5]) * NEWWIDTH + shrink_col(j,tail[5] - head[5],bot[5] - top[5])] = 1;
            }
        }
    }
    for (int i = 0; i < bot[6] - top[6]; i++) {
        for (int j = 0; j < tail[6] - head[6]; j++) {
            if (char_after_trim6[i * (tail[6] - head[6]) + j] == '1') {
                auto_shrinked6[shrink_row(i,tail[6] - head[6],bot[6] - top[6]) * NEWWIDTH + shrink_col(j,tail[6] - head[6],bot[6] - top[6])] = 1;
            }
        }
    }
    
    printf("\nprint out the auto shrinked matrix\n\n");
    
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", auto_shrinked0[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", auto_shrinked1[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", auto_shrinked2[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", auto_shrinked3[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", auto_shrinked4[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", auto_shrinked5[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < NEWHEIGHT; i++) {
        for (int j = 0; j < NEWWIDTH; j++) {
            printf("%d ", auto_shrinked6[i * NEWWIDTH + j]);
        }
        printf("\n");
    }
    
    std::ofstream outfile;
    outfile.open("testing_set.dat");
    
    //printf("\nprint out the auto shrinked bit vector\n\n");
    if (tail[0] - head[0] > 0) {
        for (int i = 0; i < NEWHEIGHT; i++) {
            for (int j = 0; j < NEWWIDTH; j++) {
                //printf("%d", auto_shrinked0[i * NEWWIDTH + j]);
                outfile << auto_shrinked0[i * NEWWIDTH + j];
            }
            //printf("\n");
        }
        //printf("\n");
        outfile << std::endl;
    }
    if (tail[1] - head[1] > 0) {
        for (int i = 0; i < NEWHEIGHT; i++) {
            for (int j = 0; j < NEWWIDTH; j++) {
                //printf("%d", auto_shrinked1[i * NEWWIDTH + j]);
                outfile << auto_shrinked1[i * NEWWIDTH + j];
            }
            //printf("\n");
        }
        //printf("\n");
        outfile << std::endl;
    }
    if (tail[2] - head[2] > 0) {
        for (int i = 0; i < NEWHEIGHT; i++) {
            for (int j = 0; j < NEWWIDTH; j++) {
               // printf("%d", auto_shrinked2[i * NEWWIDTH + j]);
                outfile << auto_shrinked2[i * NEWWIDTH + j];
            }
            //printf("\n");
        }
       // printf("\n");
        outfile << std::endl;
    }
    if (tail[3] - head[3] > 0) {
        for (int i = 0; i < NEWHEIGHT; i++) {
            for (int j = 0; j < NEWWIDTH; j++) {
               // printf("%d", auto_shrinked3[i * NEWWIDTH + j]);
                outfile << auto_shrinked3[i * NEWWIDTH + j];
            }
            //printf("\n");
        }
       // printf("\n");
        outfile << std::endl;
    }
    if (tail[4] - head[4] > 0) {
        for (int i = 0; i < NEWHEIGHT; i++) {
            for (int j = 0; j < NEWWIDTH; j++) {
              //  printf("%d", auto_shrinked4[i * NEWWIDTH + j]);
                outfile << auto_shrinked4[i * NEWWIDTH + j];
            }
            //printf("\n");
        }
       // printf("\n");
        outfile << std::endl;
    }
    if (tail[5] - head[5] > 0) {
        for (int i = 0; i < NEWHEIGHT; i++) {
            for (int j = 0; j < NEWWIDTH; j++) {
            //    printf("%d", auto_shrinked5[i * NEWWIDTH + j]);
                outfile << auto_shrinked5[i * NEWWIDTH + j];
            }
            //printf("\n");
        }
      //  printf("\n");
        outfile << std::endl;
    }
    if (tail[6] - head[6] > 0) {
        for (int i = 0; i < NEWHEIGHT; i++) {
            for (int j = 0; j < NEWWIDTH; j++) {
             //   printf("%d", auto_shrinked6[i * NEWWIDTH + j]);
                outfile << auto_shrinked6[i * NEWWIDTH + j];
            }
            //printf("\n");
        }
      //  printf("\n");
        outfile << std::endl;
    }
    
    outfile.close();
    
    free(character_after_seg0);
    free(character_after_seg1);
    free(character_after_seg2);
    free(character_after_seg3);
    free(character_after_seg4);
    free(character_after_seg5);
    free(character_after_seg6);
    free(char_after_trim0);
    free(char_after_trim1);
    free(char_after_trim2);
    free(char_after_trim3);
    free(char_after_trim4);
    free(char_after_trim5);
    free(char_after_trim6);
    free(img);
    return 0;
}
