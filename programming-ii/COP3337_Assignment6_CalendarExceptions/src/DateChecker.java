import java.util.Scanner;

//custom exception class for Month/Day/Year inputs that are incorrect
//MonthException extends the Exception class
class MonthException extends Exception {
    public MonthException(String message) {
        //calls the constructor of the parent class (Exception) and passes the custom error message
        super(message);
    }
}
//DayException extends the Exception class
class DayException extends Exception {
    public DayException(String message) {
        super(message);
    }
}
//YearException extends the Exception class
class YearException extends Exception {
    public YearException(String message) {
        super(message);
    }
}

public class DateChecker {
    public static void main(String[] args) {
        Scanner scn = new Scanner(System.in);
        boolean running = true;
        String date;
        while (running) {
            try {
                System.out.println();
                System.out.print("Enter date in MM/DD/YYYY format: ");
                date = scn.nextLine();

                //splits the date string by regex / and makes an array of size 3
                String[] array = date.split("/", 3);

                if (array.length != 3) {
                    throw new ArrayIndexOutOfBoundsException();
                }

                //turns the integer inside the array and assigns it to month/day/year
                int month = Integer.parseInt(array[0]);
                int day = Integer.parseInt(array[1]);
                int year = Integer.parseInt(array[2]);
//            System.out.println(month + " " + day + " " + year);
//            for(int i=0 ; i<array.length ; i++) {
//                System.out.println(array[i]);
//            }
                validate(day, month, year);
                System.out.println("Formatted Date: " + getMonthName(month) + " " + day + ", " + year);
                //while loop will end once of the date is formatted so assign running as false after.
                running = false;

//            getDaysInMonth(month, year);
//            System.out.println(checkLeapYear(year));
//            System.out.println(getMonthName(month));

                //The month is between 1 and 12.
                //The day is within the appropriate range for the given month and year (e.g., 31 days for January, March, May, July, August, October, and December; 30 days for April, June, September, and November; and 28 or 29 days for February depending on leap years).
                //The year is between 1000 and 3000.

            } catch (NumberFormatException e) {
                System.out.println("Please input numbers only in MM/DD/YYYY format");

            } catch (ArrayIndexOutOfBoundsException e) {
                System.out.println("Date must be in the format MM/DD/YYYY");

            } catch (MonthException | DayException | YearException e) {
                System.out.println(e.getMessage());
            }
        }
        scn.close();
    }
    //checks if input is valid
    public static void validate(int day, int month, int year) throws MonthException, DayException, YearException {
        //checks if month is between 1 and 12
        if(month < 1 || month > 12) {
            throw new MonthException("Invalid month. Please enter a month between 1 and 12.");
        }
        //checks if year is within range 1000-3000
        if(year < 1000 || year > 3000) {
            throw new YearException("Invalid year. Please enter a day between 1000 and 3000.");
        }

        //checks the number of days possible for a given month and if it is a leap year.
        int monthRange = getDaysInMonth(month, year); // Example: checks month 2 and year 2020. it is a leap year, so it has 29 days, returns 29

        //if the day entered is not within the possible range for a given month it is invalid.
        if(day < 1 || day > monthRange){
            if (monthRange == 29){
                throw new DayException("Invalid day. " + year+ " is a leap year. " + getMonthName(month)+ " only has days between 1 and " + monthRange + ".");

            }
            throw new DayException("Invalid day. " + getMonthName(month)+ " only has days between 1 and " + monthRange +".");
        }
    }

    public static boolean checkLeapYear(int year) {
        //checks if the year is a leap year
        // if the year is divisible by 4 with no remainder
        // and the year cannot be divisible by 100, unless it divisible by 400 then it is a leap year.
        if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
            return true;
        }
        return false;
    }

    public static int getDaysInMonth(int month, int year) {
        switch(month){
            // jan,march,may,june,august,oct,dec will return 31 days.
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                return 31;
            // april,june,sept, nov will return 30 days.
            case 4: case 6: case 9: case 11:
                return 30;
            // for february you must check if it a leap year to assign how many days
            case 2:
                // isleapYear is true = when checkleapYear is true
                // boolean is true if checkLeapYear method returns as true
                boolean isLeapYear = checkLeapYear(year);
                // when boolean is true it is a leap year so there are 29 days
                if (isLeapYear) {
                    return 29; }
                // when boolean is false it is not a leap year so there are 28 days
                else {
                    return 28; }
        }
        return 0;
    }

    //assigns the numbers 1-12 to the correct month name.
    public static String getMonthName(int month) {
        switch(month) {
            case 1: return "January";
            case 2: return "February";
            case 3: return "March";
            case 4: return "April";
            case 5: return "May";
            case 6: return "June";
            case 7: return "July";
            case 8: return "August";
            case 9: return "September";
            case 10: return "October";
            case 11: return "November";
            case 12: return "December";
            default: return "Invalid month";
        }
    }
}