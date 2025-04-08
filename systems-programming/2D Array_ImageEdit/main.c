#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ROW 5
#define COL 5

void fillMatrix(int (*p)[COL]);
void printMatrix(int (*p)[COL]);
void brightness(int k, int (*pA)[COL], int (*pB)[COL]);
void invertColor(int (*pA)[COL], int (*pB)[COL]);
void transpose(int (*pA)[COL], int (*pB)[COL]);
// void loadImage(int (*pA)[COL], int (*pB)[COL]);

int main() {
    int A[COL][ROW]; //5x5
    int B[COL][ROW];
    fillMatrix(A);
    printf("\nOriginal Matrix:\n");
    printMatrix(A);

    int running = 1;
    do {
        printf("\nChoose an option:\n");
        printf("1. Increase Brightness\n");
        printf("2. Invert Color\n");
        printf("3. Transpose Image\n");
        int option;
        scanf("%d", &option);

        if(option == 1){
            int k;
            printf("\nIncrease brightness by k = ");
            scanf("%d", &k);
            brightness(k,A,B);
            printf("\nProcessed Image:\n");
            printMatrix(B);
            // running = 0;
        }
        else if(option == 2){
            invertColor(A,B);
            printf("\nProcessed Image:\n");
            printMatrix(B);
        }
        else if(option == 3){
            transpose(A,B);
            printf("\nProcessed Image:\n");
            printMatrix(B);
        }
        else if(option == 4) {
            running = 0;
        }
        else {
            printf("\nInvalid Option. Try again.\n");
        }
    }while(running);
    return 0;
}

void brightness(int k, int (*pA)[COL], int (*pB)[COL]){
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++){
            int newValue = *(*(pA+i)+j) + k;
            // Cap the brightness at 255
            *(*(pB+i)+j) = (newValue > 255) ? 255 : newValue;
        }
    }
}

void invertColor(int (*pA)[COL], int (*pB)[COL]){
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++)
            *(*(pB+i)+j) = 255 - *(*(pA+i)+j);
    }
}

void transpose(int (*pA)[COL], int (*pB)[COL]){
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++){
            *(*(pB+j)+i) = *(*(pA+i)+j);
        }
    }
}


void fillMatrix(int (*p)[COL]){
    srand(time(0));
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++)
            *(*(p+i)+j) = rand() % 255 + 1; // number between 0 and 255
    }
}

void printMatrix(int (*p)[COL]) {
    for(int i = 0; i < ROW; i++) {
        for(int j = 0; j < COL; j++)
            printf("%4d\t", *(*(p+i)+j)); // print addresses
        printf("\n");
    }
}