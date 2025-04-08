#include <stdio.h>
#include <stdlib.h>
int is_leap_year(int year);
void calculate_age(int birth_month, int birth_day, int birth_year,
    int current_month, int current_day, int current_year);

int main() {
    int birth_month, birth_day, birth_year;
    int current_month, current_day, current_year;

    // asks the user to input current date
    printf("Enter the current date (MM DD YYYY): ");
    scanf("%d %d %d", &current_month, &current_day, &current_year);

    // asks user to input birth date
    printf("Enter your birth date (MM DD YYYY): ");
    scanf("%d %d %d", &birth_month, &birth_day, &birth_year);

    // calls function to calculate the age.
    calculate_age(birth_month, birth_day, birth_year, current_month, current_day, current_year);

    return 0;
}

// checks if the year is a leap year.
int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
void calculate_age(int birth_month, int birth_day, int birth_year,
                   int current_month, int current_day, int current_year) {
    // array to hold list of days in each month
    int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // adjusts the index of the days array to 29 if it is a leap year.
    if (is_leap_year(current_year)) {
        month_days[1] = 29;
    }
    // calculates the current day, month and year if the birth day / month is greater than current day / month.
    // current month is substracted since it is not counted.
    // example for April, that is the 4 month located at index 3 of the array.
    // so month_day[birth_month -1] finds the accurate date.
    if (birth_day > current_day) {
        current_month = current_month - 1;
        current_day = current_day + month_days[birth_month -1];
    }
    if (birth_month > current_month) {
        current_year = current_year - 1;
        current_month = current_month + 12;
    }

    if (birth_year > current_year) {
        printf("Invalid year. Birth year must be before current year.");
        exit(0);
    }

    // substracts the current day/month/yeah calculated by the birth day to get the actual age.
    int day = current_day-birth_day;
    int month = current_month - birth_month;
    int year = current_year - birth_year;
    printf("Your age is: %d years %d months %d days!\n", year, month, day);
}



