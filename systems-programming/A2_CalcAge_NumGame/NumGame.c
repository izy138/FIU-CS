#include<stdio.h>
#include<stdlib.h>
void find_unique(int arr[], int n);
void find_frequent(int arr[], int n);
void left_shift(int arr[], int n);
int maxSize = 100;

int main() {
    int n, arr[maxSize]; // creates an array of maximum size 100.
    //prompt user for number of elements and will continue to prompt is input is not between 0 and 100.
    do {
        printf("Enter number of elements: ");
        scanf("%d", &n);
        if(n < 1 || n > 100) {
            printf("Invalid input. Must be between 1 and 100. Try again.\n");
        }
    }while (n < 1 || n > 100);

    // user inputs number n elements into array.
    printf("Enter %d elements: ", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    find_unique(arr, n);
    find_frequent(arr, n);
    left_shift(arr, n);
    return 0;
}

void find_unique(int arr[], int n) {
    printf("Unique numbers: ");
    int isUnique = 0;// {1,1,2,1}
    //goes through each element in the array twice and compares the numbers
    //if a number is found more than once it will increment count++
    for(int i=0;i<n;i++) {
        int count=0;
        for(int j=0;j<n;j++) {
            if(arr[i]==arr[j]) {
                count++;
            }
        }
        //if the number only appears once the count will be 1 and count as a unique number.
        if (count == 1) {
            printf("%d ",arr[i]);
            isUnique = 1;
        }
    }
    //if no unique numbers are found unique = 0
    if (!isUnique) {
        printf("No unique numbers ");
    }
    printf("\n");
}
void find_frequent(int arr[], int n) {
    int maxFrequent= 0;
    int mostFrequent[maxSize]; // stores the most frequent numbers
    int count = 0; // counter for most frequent numbers
    //goes through each element in the array and checks if there are multiples of the same number
    for(int i=0;i<n;i++) {
        int freq=0;
        for(int j=0;j<n;j++) {
            if(arr[i]==arr[j]) {
                freq++;
            }
        }
        // if the count is greater than the current maxFrequent count that means it is currently
        // the most frequent number
        if (freq > maxFrequent) {
            maxFrequent = freq; // updates the maximum frequency
            count = 0;
            mostFrequent[count++] = arr[i]; //adds the number to the array of most frequent.
        }else if (freq == maxFrequent) {
            int duplicate = 0;
            for(int j=0;j<n;j++) {
                if (mostFrequent[j] == arr[i]) {
                    duplicate= 1;
                    break;
                }
            }
            if (!duplicate) {
                mostFrequent[count++] = arr[i];
            }
        }
    }
    printf("Frequent element(s): ");
    for(int i=0;i<count;i++) {
        printf("%d ", mostFrequent[i]);
    }
    printf("\n");
}
void left_shift(int arr[], int n) {
    int firstElement = arr[0];
    for(int i=0;i<n-1;i++) {
        //shifts elements to the left
        arr[i] = arr[i+1];
    }
    //
    arr[n-1] = firstElement;
    printf("Left shift elements: ");

    for(int i=0;i<n;i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

