# Student Information
- Full Name: Isabella Correa
- PID: 6043518
- Section: U01
- FIU Email: icorr014@fiu.edu

# Assignment: Student Data Processor

main() opens the file and passes the student info through extractStudents:
- read line by line using fgets()
- each line goes through method trimWhitespace
- lines are added until seperator or empty line is reached. (=====)

Then the lines go through method extractFieldValue:
- called for each field : Name, PID, Email, etc...
- finds "Name:" label, skips the whitespace after it and splits everything after the label into tokens
- stops tokenizing once a label is reached ex "PID:"

Next the words go through the noiseWordFilter to remove any irrelevant words.
Them the fields are validated through their own methods:
- the name is validated making sure it is all alphabet characters, between 2-20 chars, and contains at least 1 vowel
- the other fields are validated, department field must be between 3-35 char, and more than 80% of the char should be alphabetic
- the Panther ID is checked, the first 3 chars should be 'FIU' followed by 5 digits
- the Email is checked for an @ symbol
- the Phone is checked for exactly 10 digits
- the year is checked by matching from the available options: Freshman, Sophomore, etc..

After the fields are extracted, the Name is checked again to make sure it is 2 words length.
Then the extracted fields are validated again through their methods to make sure its correct.

The extractStudents method will only save a student when all the 6 fields return a valid entry
Anything incomplete will be discarded, and then students are added to the array.
main() goes through each student in the student array and prints them to output.
