#include<stdio.h>
int main (){
    char code;
    double basicSalary = 0.0;
    double housing, medical, mileage, utilities;
    double grossSalary, netSalary, incomeTax;

    do{
        printf("\nEnter a code (C, D, M, S, T, R, B): ");
        scanf(" %c",&code);

        switch(code){
            case 'C': basicSalary = 5000;
            break;
            case 'D': basicSalary = 4800;
            break;
            case 'M': basicSalary = 3950;
            break;
            case 'S': basicSalary = 3100;
            break;
            case 'T': basicSalary = 2800;
            break;
            case 'R': basicSalary = 1450;
            break;
            case 'B': basicSalary = 1770;
            break;
            default: printf("Invalid code. Try again. \n");
        }
    }while(basicSalary == 0.0);


    housing = basicSalary * 0.25;
    medical = basicSalary * 0.085;
    mileage= basicSalary * 0.037;
    utilities = basicSalary * 0.072;

    grossSalary = basicSalary + housing + medical + mileage + utilities;

    if (grossSalary > 5500) {
        incomeTax = grossSalary * 0.14;
    } else if (grossSalary > 5000) {
        incomeTax = grossSalary * 0.12;
    } else if (grossSalary > 4500) {
        incomeTax = grossSalary * 0.10;
    } else if (grossSalary > 4000) {
        incomeTax = grossSalary * 0.08;
    } else {
        incomeTax = grossSalary * 0.06;
    }

    netSalary = grossSalary - incomeTax;

    printf("SALARY INFORMATION \n");
    printf("=================== \n");
    printf("Basic Salary: \t\t\t %.0f\n", basicSalary);
    printf("Housing Rent Allowance: \t %.2f\n", housing);
    printf("Medical Allowance: \t\t %.2f\n", medical);
    printf("Monthly Mileage Allowance: \t %.2f\n", mileage);
    printf("Utilities Allowance: \t\t %.2f\n", utilities);
    printf("Gross Salary: \t\t\t %.2f\n", grossSalary);
    printf("Income Tax: \t\t\t %.2f\n", incomeTax);
    printf("Net Payable Salary: \t\t %.2f\n", netSalary);

    return 0;
}
