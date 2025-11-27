#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000 //max length for a single line in the file
#define MAX_VALUE_LENGTH 100 //max length for a field's value
#define MAX_STUDENTS 100 // max number of students in file

typedef struct {
    char name[MAX_VALUE_LENGTH];
    char pid[MAX_VALUE_LENGTH];
    char email[MAX_VALUE_LENGTH];
    char phone[MAX_VALUE_LENGTH];
    char department[MAX_VALUE_LENGTH];
    char year[MAX_VALUE_LENGTH];
} Student;

// remove whitespace (spaces, tabs, new lines) from the beginning and end of a string
char* trimWhitespace(char* str) {
    char* end;
    // move the str pointer forward which skips any space that is not a char
    while(isspace((unsigned char)*str)) //dereference str and casts to an unsigned char
      str++;

    if(*str == 0) //if the entire string is white space it returns empty str
      return str;

    // the end of the string starts at the last char
    // pointer moves backwards while space is empty to trim space at the end
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // appends a null terminator after the last char
    end[1] = '\0';

    return str;
}

// function to check if a word is a valid name
int validNameCheck(char* word) {
    //    printf("DEBUG: Checking name '%s'\n", word);

    if (!word || strlen(word) == 0) return 0;

    // name should be alphabetic only
    for (int i = 0; word[i]; i++) {
        if (!isalpha(word[i])) return 0;
    }

    // should be between 2-20 characters
    int len = strlen(word);
    if (len < 2 || len > 20) return 0;

    // should have at least one vowel
    int hasVowel = 0;
    for (int i = 0; word[i]; i++) {
        char c = tolower(word[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            hasVowel = 1;
            break;
        }
    }
    return hasVowel;
}

// function to check if the name is exactly 2 words
int validNameCount(char* str) { //ex: takes the string "John Abraham"
  // checks if the string is null or empty
    if (!str || strlen(str) == 0) return 0;

    //creates a temp copy of the string
    char temp[MAX_VALUE_LENGTH];
    strcpy(temp, str);
    // splits the string at a space or tab ex:  "John", "Abraham"
    char* word = strtok(temp, " \t");

    int wordCount = 0;
    //counts the number of words in the temp array
    while (word != NULL) {
        wordCount++;
        if (wordCount > 2) return 0; // more than 2 words = exit
        word = strtok(NULL, " \t");
    }
    // first and last name = 2 words
    return wordCount == 2;
}

// function to check if a string is a valid PID: FIU followed by 5 digits
int validPIDCheck(char* str) {
  // check for null pointer and if length of string is 8 (FIU12345)
    if (!str || strlen(str) != 8) return 0;
    //checks the first three char to make sure they are "FIU
    if (strncmp(str, "FIU", 3) != 0) return 0;
    // checks the five digits are actually numbers and not chars.
    for (int i = 3; i < 8; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

// function to check if a string is a valid email
int validEmailCheck(char* str) {
  //check for null or empty string
    if (!str || strlen(str) == 0) return 0;
    //returns pointer to @ char or null if not found.
    return strchr(str, '@') != NULL;
}

// function to check if a string is a valid phone number should be exactly 10 digits
int validPhoneCheck(char* str) {
    if (!str || strlen(str) != 10) return 0; // null/empty check and 10 digit length check

    //itereates through the string to verify theyre all numbers 
    for (int i = 0; i < 10; i++) {
        if (!isdigit(str[i])) return 0;
    }

    return 1;
}

// Function to check if a word is a valid department component
int validDepartmentCheck(char* word) {
//    printf("DEBUG: Checking department word '%s'\n", word);

    if (!word || strlen(word) == 0) return 0;

    int len = strlen(word); // takes the length of the word and checks between 3-25 chars
    if (len < 3 || len > 25) return 0;

    // count alphabetic chars
    int alphaCount = 0;
    int spaceCount = 0;

    for (int i = 0; word[i]; i++) {
        //check is alphbetical
        if (isalpha(word[i])) {
            alphaCount++;
        //checks for spaces
        } else if (isspace(word[i])) {
            spaceCount++;
            //no special characters
        } else if (!isspace(word[i]) && !isalpha(word[i])) {
            return 0;
        }
    }

    // At least 80% should be alphabetical vs spaces
    return (alphaCount * 100 / len) >= 80;
}

// function to check if a string is a valid year
int validYearCheck(char* str) {
    if (!str || strlen(str) == 0) return 0; // null/empty check

    // check if string matches any of the valid values for year
    return (strcmp(str, "Freshman") == 0 ||
            strcmp(str, "Sophomore") == 0 ||
            strcmp(str, "Junior") == 0 ||
            strcmp(str, "Senior") == 0 ||
            strcmp(str, "Graduate") == 0);
}

// Noise filters using different patterns
int noiseWordFilter(char* word) {
//    printf("DEBUG: Rejecting '%s' as noise (reason: [pattern type])\n", word);

    if (!word || strlen(word) == 0) return 1;

    int len = strlen(word);

    //filter out known words
    if (strcmp(word, "xaxsaxa") == 0 ||
        strcmp(word, "garbage") == 0 ||
        strcmp(word, "qwerty") == 0) {
        return 1;
    }

    // Check if word is all numbers (but not valid phone or PID)
    if (strspn(word, "0123456789") == (size_t)len) {
        if (len < 8 || (len != 8 && len != 10)) {
//            printf("DEBUG: Rejecting '%s' as noise (reason: all-numbers)\n", word);
            return 1;  // Numbers that aren't PID (8) or phone (10)
        }
    }

    // Check for any repeating characters
    if (len >= 4) {
        // ex : "aaaa"
        int repeatChar = 1;
        for (int i = 1; i < len; i++) {
            if (word[i] != word[0]) {
                repeatChar = 0;
                break;
            }
        }
        if (repeatChar){
//            printf("DEBUG: Rejecting '%s' as noise (reason: repeating chars)\n", word);
            return 1;
        }

        // Check for alternating patterns (like "xaxaxa")
        if (len >= 6 && len % 2 == 0) {
            int half = len / 2;
            if (strncmp(word, word + half, half) == 0) {
//                printf("DEBUG: Rejecting '%s' as noise (reason: repeating pattern)\n", word);
                return 1;
            }
        }
    }

    // Check for any random consonants
    if (len >= 4) {
        int consonants = 0, vowels = 0;
        for (int i = 0; i < len; i++) {
            char c = tolower(word[i]);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowels++;
            } else if (isalpha(c)) {
                consonants++;
            }
        }

        // If no vowels and many consonants
        if (vowels == 0 && consonants >= 4) {
//            printf("DEBUG: Rejecting '%s' as noise (reason: no-vowels)\n", word);
            return 1;
        }

        // more consonants than vowels
        if (vowels > 0 && (consonants / vowels) > 8) {
//            printf("DEBUG: Rejecting '%s' as noise (reason: consonant-ratio too high)\n", word);
            return 1;
        }
    }

    // mixed alphabet and digits in too short of a string
    if (len <= 10) { // string less than 10 chars
        int alpha = 0, digit = 0;
        for (int i = 0; i < len; i++) {
            if (isalpha(word[i])) alpha = 1;
            if (isdigit(word[i])) digit = 1;
        }
        //if theres both and length is less than 6
        if (alpha && digit && len <= 6) {
//            printf("DEBUG: Rejecting '%s' as noise (reason: mixed-char pattern)\n", word);
            return 1;
        }
    }

    return 0;
}

// Extract all words found after a label
void extractFieldValue(char* line, char* label, char* output, int maxLen) {
    //finds the label within the line
    char* pos = strstr(line, label);
    if (!pos) return; //if no label is found in the line, exit

    pos += strlen(label); // moves the pointer after the labels position

    // Skip whitespace after label
    while (*pos && isspace(*pos)) pos++;

    // adds all words after it until we hit another label
    char temp[MAX_LINE_LENGTH];
    strcpy(temp, pos);

    //tokens are split on spaces, tabs, and carriages
    char* token = strtok(temp, " \t\n\r");
    char result[MAX_VALUE_LENGTH] = "";

    // label fields
    int nameField = (strstr(label, "Name:") != NULL);
    int departmentField = (strstr(label, "Department:") != NULL);
    int PIDField = (strstr(label, "PID:") != NULL);
    int emailField = (strstr(label, "Email:") != NULL);
    int phoneField = (strstr(label, "Phone:") != NULL);
    int yearField = (strstr(label, "Year:") != NULL);

    //go through each token we extracted
    while (token != NULL) {
        // stops if we hit another field label
        if (strstr(token, "PID:") || strstr(token, "Email:") ||
            strstr(token, "Phone:") || strstr(token, "Department:") ||
            strstr(token, "Year:") || strstr(token, "Name:")) {
            break;
        }

        //skips the noise word tokens for all fields
        if (noiseWordFilter(token)) {
            token = strtok(NULL, " \t\n\r");
            continue;
        }
        // checks each token throuugh its respective field method check
        int validToken = 0;
        if (nameField && !noiseWordFilter(token)) {
            validToken = validNameCheck(token);
        } else if (departmentField) {
            validToken = validDepartmentCheck(token);
        } else if (PIDField) {
            validToken = validPIDCheck(token);
        } else if (emailField) {
            validToken = validEmailCheck(token);
        } else if (phoneField) {
            validToken = validPhoneCheck(token);
        } else if (yearField) {
            validToken = validYearCheck(token);
        } else {
            validToken = !noiseWordFilter(token);
        }

        // adds valid tokens to result
        if (validToken) {
            if (strlen(result) > 0) {
                //adds a space if there is content inside
                strcat(result, " ");
            }
            strncat(result, token, MAX_VALUE_LENGTH - strlen(result) - 1);
        }
        //gets to next token
        token = strtok(NULL, " \t\n\r");
    }
    //trims any whitespace in the result string
    trimWhitespace(result);
    //copies the result to output if there is text
    if (strlen(result) > 0 && strlen(result) < (size_t)maxLen) {
        strcpy(output, result);
    }
}

// first function to extract students from file content
int extractStudents(FILE* file, Student* students) {
    char line[MAX_LINE_LENGTH]; //sets each line to a max length of 1000
    int studentCount = 0;
    Student currentStudent = {0};
    char collectedText[MAX_LINE_LENGTH * 5] = ""; // Buffer for multi-line records

    //reads the file line by line until the end
    while (fgets(line, sizeof(line), file)) {
        // trims any white space in the line
        trimWhitespace(line);

        // skip any empty lines and separator lines
        if (strlen(line) == 0 || strstr(line, "====")) {
            // processes the text found for a new student
            if (strlen(collectedText) > 0) {
                // extract all fields from the collected text
                extractFieldValue(collectedText, "Name:", currentStudent.name, MAX_VALUE_LENGTH);
                extractFieldValue(collectedText, "PID:", currentStudent.pid, MAX_VALUE_LENGTH);
                extractFieldValue(collectedText, "Email:", currentStudent.email, MAX_VALUE_LENGTH);
                extractFieldValue(collectedText, "Phone:", currentStudent.phone, MAX_VALUE_LENGTH);
                extractFieldValue(collectedText, "Department:", currentStudent.department, MAX_VALUE_LENGTH);
                extractFieldValue(collectedText, "Year:", currentStudent.year, MAX_VALUE_LENGTH);

                // check extracted field values with their method checks
                if (!validNameCount(currentStudent.name)) {
                    strcpy(currentStudent.name, "");
                }
                if (!validPIDCheck(currentStudent.pid)) {
                    strcpy(currentStudent.pid, "");
                }
                if (!validEmailCheck(currentStudent.email)) {
                    strcpy(currentStudent.email, "");
                }
                if (!validPhoneCheck(currentStudent.phone)) {
                    strcpy(currentStudent.phone, "");
                }
                if (!validYearCheck(currentStudent.year)) {
                    strcpy(currentStudent.year, "");
                }

                // checks that all fiedls are filled, and saves the student to the array
                if (strlen(currentStudent.name) > 0 &&
                    strlen(currentStudent.pid) > 0 &&
                    strlen(currentStudent.email) > 0 &&
                    strlen(currentStudent.phone) > 0 &&
                    strlen(currentStudent.department) > 0 &&
                    strlen(currentStudent.year) > 0) {

                    students[studentCount] = currentStudent;
                    studentCount++;
                }

                // resets for next student
                memset(&currentStudent, 0, sizeof(Student));
                strcpy(collectedText, "");
            }
            continue;
        }

        // Collects text for current student record
        if (strlen(collectedText) > 0) {
            strcat(collectedText, " ");
        }
        strcat(collectedText, line);
    }

    // Process any remaining text
    if (strlen(collectedText) > 0) {
        extractFieldValue(collectedText, "Name:", currentStudent.name, MAX_VALUE_LENGTH);
        extractFieldValue(collectedText, "PID:", currentStudent.pid, MAX_VALUE_LENGTH);
        extractFieldValue(collectedText, "Email:", currentStudent.email, MAX_VALUE_LENGTH);
        extractFieldValue(collectedText, "Phone:", currentStudent.phone, MAX_VALUE_LENGTH);
        extractFieldValue(collectedText, "Department:", currentStudent.department, MAX_VALUE_LENGTH);
        extractFieldValue(collectedText, "Year:", currentStudent.year, MAX_VALUE_LENGTH);

        // validates the students extracted data
        if (!validNameCount(currentStudent.name)) {
            strcpy(currentStudent.name, "");
        }
        if (!validPIDCheck(currentStudent.pid)) {
            strcpy(currentStudent.pid, "");
        }
        if (!validEmailCheck(currentStudent.email)) {
            strcpy(currentStudent.email, "");
        }
        if (!validPhoneCheck(currentStudent.phone)) {
            strcpy(currentStudent.phone, "");
        }
        if (!validYearCheck(currentStudent.year)) {
            strcpy(currentStudent.year, "");
        }

        // if we have all required fields, save the student
        if (strlen(currentStudent.name) > 0 &&
            strlen(currentStudent.pid) > 0 &&
            strlen(currentStudent.email) > 0 &&
            strlen(currentStudent.phone) > 0 &&
            strlen(currentStudent.department) > 0 &&
            strlen(currentStudent.year) > 0) {

            students[studentCount] = currentStudent;
            studentCount++;
        }
    }

    return studentCount;
}

int main() {
  //open the input file
    FILE* file = fopen("Testing/Testcases/input1.txt", "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open input file\n");
        return 1;
    }

    Student students[MAX_STUDENTS];
    int studentList = extractStudents(file, students);
    fclose(file);

    // header format
    printf("Student Name,PID,Email,Phone Number,Department,Year\n");
    printf("====================================================\n");

    // goes through each student and prints their data
    for (int i = 0; i < studentList; i++) {
        printf("%s,%s,%s,%s,%s,%s\n",
               students[i].name,
               students[i].pid,
               students[i].email,
               students[i].phone,
               students[i].department,
               students[i].year);
    }

    return 0;
}