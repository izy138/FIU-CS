# Student Information
- Full Name: Isabella Correa
- PID: 6043518
- Section: U01
- FIU Email: icorr014@fiu.edu

# Homework 2:

Implemented the sum of numbers using the provided formula: 
sum = n * (n + 1) / 2

Wrote a iterative method for the factorial sum starting at i=2 
because 1x 2 = 2 , so the next would be 2x3 = 6 and so on

Made a recursive factorial method to call in order to print 
the fibonacci numbers in order.


# Terminal Commands:

//compile source file into object file 
gcc -c submission_HW2.c -o submission_HW2.o

//create static library from object file 
ar rcs libmathutils.a submission_HW2.o

//build the executable using Makefile 
make 

//run the program and save output to txt
./run > student_output.txt 

//compare my output with expected output
diff student_output.txt Testing/Expected_Output/output1.txt

//make sure the autograder script is executable 
chmod +x autograder_HW2.sh 

//run autograding
./autograder_HW2.sh

