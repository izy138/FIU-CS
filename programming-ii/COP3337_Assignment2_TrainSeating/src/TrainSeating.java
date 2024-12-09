import java.util.Scanner;

public class TrainSeating {
    public static void main(String[] args) {
        // seats array has 5 containers that fit 4 values in each
        // 5 rows 4 columns

        // Initialization
        // row index                 0                     1                     2                     3                    4
        String[][] seats = {{"A", "B", "C", "D"}, {"A", "B", "C", "D"}, {"A", "B", "C", "D"}, {"A", "B", "C", "D"}, {"A", "B", "C", "D"}};
        // column index       0    1    2    3

        // int totalSeatsAvailable = 20; assigns total seats to 20 since there are 5 rows x 4 seats in each row
        // it is okay to call only the first index in this case because we know the seats # are the same for every row
        int totalSeatsAvailable = seats.length * seats[0].length;

        System.out.println();
        System.out.println("Initial Seat Pattern:");
        display(seats);     // called display method and displays available seats

        Scanner scn = new Scanner(System.in); // creates a new scanner

        // while loop that continues while total seats available is greater than 0
        while (totalSeatsAvailable > 0) {
            // prompts user to input seat row and column
            System.out.print("Enter seat row (1-5) and column (A-D) or 'Q' to quit: ");
            // scans user input and converts into upper case
            String input = scn.nextLine().toUpperCase();

            // if user inputs "Q" the while loop will end
            if (input.equals("Q")) {
                break;
            }
            // if user input has a length of 2
            if (input.length() == 2) {

                /* example input is : '1A' charAt(0) = "1" charAt(1) = "A"
                Using ASCII table, the charAt(0) = "1" is equal to the number 49, row1 is at index[0]
                if I want the input string "1" to correspond to row = 0 as an int not a String, I need to subtract by the string "1" because it corresponds to int 49.
                "1"(49) - "1"(49) = 0
                for column: input string "A" I also want that to correspond to column1 at index[0].
                In ASCII "A" = 65. So in order to have column = 0 (int) I subtract "A"(65) - 65 = 0
                this will work for the other possible values example "B" is the 2nd row at index[1] "B"=66 , so "B"(66) - 65 = 1 */
                int row = input.charAt(0) - '1';
                int column = input.charAt(1) - 65;

                // if row is between 0 and 5 AND column is between 0 and 4
                if (row >= 0 && row < seats.length && column >= 0 && column < seats[row].length) {
                    // if 'X' is placed in one of the seats, the seat is noted as occupied and user is prompted to input another seat
                    if (seats[row][column].equals("X")) {
                        System.out.print("Seat is occupied.\n" + "Please try again. ");

                    } else {
                        /* else if no 'X' is found, prints the seat that has been inputted: 'Seat 1A assigned'
                        example '1A' corresponds to seats[0][0]. row index 0 and column index 0 will equal 'X'
                        one of the seats has been taken so the total seats available is incremented -- by 1 less seat and then the display is updated*/
                        System.out.println("Seat " + input + " assigned.");
                        seats[row][column] = "X";
                        totalSeatsAvailable--;
                        display(seats);
                    }
                } else {
                    // if the rows and columns are not in the appropriate range of row(1-5) and column (A-D), the selection is invalid and user is prompted to try again
                    System.out.print("Invalid seat selection.\n" + "Please try again. ");
                }
            } else {
                // if input does not have a length of 2, the seat is invalid and user is prompted to input again.
                System.out.print("Invalid seat selection.\n" + "Please try again. ");
            }
        }
        // when all seats are filled total seats will be = 0
        if (totalSeatsAvailable == 0) {
            System.out.println("All seats are filled.");
        }
        // exits program when 'Q' is entered and or when all seats are filled.
        System.out.println("Exiting Program.");

    }
    // this loop makes an invalid seat selection on values that do not have a length of 2, in the case the input is incorrect for example only '1' or '1AA' is entered, it prompts the user to try again
    // lower case letters can also be used.

    // seats length is 5 so i= 0,1,2,3,4 seats[].length is 4 so i= 0,1,2,3
    public static void display(String[][] seats) {
        for (int i = 0; i < seats.length; i++) {
            // prints out the row number 1-5
            System.out.print(i+1 + " ");
            // prints out the column letter A-D and shows 'X' when seats are updated.
            for (int j = 0; j < seats[i].length; j++) {
                System.out.print(seats[i][j] + " ");
            }
            System.out.println();
        }


    }
}