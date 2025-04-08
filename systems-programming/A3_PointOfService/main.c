#include<stdio.h>

void input(char*,float*,int*,int);
void calculate_bill(float*,int*, float*,int);
void display(char*, float*,int*,int,float*);

int main(){
    //arrays hold the information for the items
    char products[] = {'K','M','P','U','D','C'};
    float price[] = {12.50, 8.25, 35.00, 7.99, 47.25, 18.75};
    //size of the array
    int size = sizeof(products)/sizeof(products[0]);
    //6 total items array that holds the quantities inputted by the user
    int quantity[6] = {0};
    float total;

    input(products,price,quantity,size);
    calculate_bill(price,quantity,&total,size);
    display(products,price,quantity,size,&total);

    return 0;
}

void input(char *products,float *price,int *quantity,int size){
    for(int i = 0; i<size; i++){
        //user inputs
        printf("Enter quantity for product %c, the price is $%.2f: ", products[i], price[i]);
        scanf("%d", &quantity[i]);
    }
    printf("\n");
}
void calculate_bill(float *price,int *quantity, float *amount, int size){
    *amount = 0;
    for(int i = 0; i<size; i++){
        //price for the item * its quantity
        *amount += price[i] * quantity[i];
    }
}
void display(char *products, float *prices, int *quantity, int size, float *amount) {
    float tax= 0.07 * (*amount); //total tax is 0.07 * the total bill amount
    float total = *amount + tax; // total payable is the amount + the total tax
    //subtotal display
    for(int i = 0; i<size; i++){
        printf("%c product has %.2f price and %d quantity is selected, sub-total: $%.2f\n",products[i], prices[i], quantity[i], prices[i] * quantity[i]);
    }
    printf("\nThe total bill amount is: $%.2f\n", *amount);
    printf("\nSales tax: $%.2f\n", tax);
    printf("\nTotal payable: $%.2f", total);
}