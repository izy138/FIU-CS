#include <stdio.h>

#define STD 5 // number of students
#define EXM 4 // number of exams

void input(float (*p)[EXM]);
void calculate_stdavg(float (*p)[EXM], float *stdavg);
void calculate_exmavg(float (*p)[EXM], float *exmavg);
void display(float (*p)[EXM], float *stdavg, float *exmavg);

void main(){
    float grades[STD][EXM];
    float stdavg[STD], exmavg[EXM];

    //stdavg = { std1_avg, std2_avg, std3_avg, std4_avg, std5_avg }
    //exmavg = { exam1_avg, exam2_avg, exam3_avg, exam4_avg }

    input(grades);
    calculate_stdavg(grades,stdavg);
    calculate_exmavg(grades,exmavg);
    display(grades,stdavg,exmavg);
}

void input(float (*p)[EXM]) {
    for (int i = 0; i < STD; i++) {
        for (int j = 0; j < EXM; j++) {
            printf("Enter grades for Exam %d of Student %d: ",j+1, i+1); //
            scanf("%f", *(p+i)+j); //inputs the score in the 2d array with *(p+i)+j
        }
        printf("\n");
    }
    printf("\n");
}

void calculate_stdavg(float (*p)[EXM], float *stdavg) {
    for (int i = 0; i < STD; i++) {
        float sum = 0.0; // resets the sum for every student
        for (int j = 0; j < EXM; j++) {
            sum += *(*(p+i)+j); // this will get the value of each grade in the 2d array
            // each grade will be added to the sum of grades
        }
        *(stdavg + i) = sum / EXM; // stdavg[i] = sum / number of exams(4)
    }
}

void calculate_exmavg(float (*p)[EXM], float *exmavg) {
    for (int j = 0; j < EXM; j++) {
        float sum = 0.0; // resets the sum for every exam
        for (int i = 0; i < STD; i++) {
            sum += *(*(p+i)+j); // add each exam score for exam[i]
        }
        *(exmavg + j) = sum / STD; // exmavg[j] = sum / total students(5)
    }
}

void display(float (*p)[EXM], float *stdavg, float *exmavg) {
    for (int i = 0; i < STD; i++) {
        for (int j = 0; j < EXM; j++) {
            printf("Student: %d, grades for Exam %d is: %.2f\n", i+1, j+1, *(*(p+i)+j));
        }
        printf("Student %d Average grades are: %.2f\n\n", i+1, *(stdavg+i));
    }
    for (int i = 0; i < EXM; i++) {
        printf("Average of Exam %d is %.2f\n", i+1, *(exmavg+i));
    }
}
