import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        Library library = new Library(); // creates a new library

        String option;
        // do-while loop so the main menu is repeated until the option "6" is input.
        do {
            System.out.println("\n1. Add item\n" + "2. Add member\n" + "3. Borrow item\n" + "4. Return item\n" + "5. Display library\n" + "6. Exit");

            System.out.print("Enter your choice: ");
            option = scanner.nextLine();

            // switch case to check the option for "1" through "6"
            switch (option) {

                case "1": //adds a new item
                    // try catch to catch exception e when an input is incorrect
                    // for example: when asking for the item ID: it should only be an int, if a String is input it will catch the error
                    try {
                        System.out.print("Enter title: ");
                        String title = scanner.nextLine();
                        System.out.print("Enter author: ");
                        String creator = scanner.nextLine();
                        System.out.print("Enter item ID: ");
                        int itemID = Integer.parseInt(scanner.nextLine());

                        // if the user inputs an ID thats already taken
                        if(library.checkItemId(itemID)){
                            System.out.println("Item ID: " + itemID +" is taken. Try again.");
                            break;
                        }

                        System.out.print("Is it a book or a DVD (B/D): ");
                        String bookOrDvd = scanner.nextLine().toUpperCase();
                        if (bookOrDvd.equals("B")) {
                            System.out.print("Enter genre: ");
                            String genre = scanner.nextLine();
                            System.out.print("Enter number of pages: ");
                            int pages = Integer.parseInt(scanner.nextLine()); // if this input is a String it will catch this error
                            Book book = new Book(title, creator, itemID, genre, pages);
                            library.addItem(book);
                        } else if (bookOrDvd.equals("D")) {
                            System.out.print("Enter director: ");
                            String director = scanner.nextLine();
                            System.out.print("Enter duration: ");
                            int duration = Integer.parseInt(scanner.nextLine()); // if this input is a String it will catch this error

                            DVD dvd = new DVD(title, creator, itemID, director, duration);
                            library.addItem(dvd);
                        } else {
                            System.out.println("Invalid option, Try again.");
                        }
                        break;
                    } catch (Exception e) {
                        System.out.println("Invalid input. Try again.");
                        break;
                    }


                case "2": // adds new member
                    try {
                        System.out.print("Enter name: ");
                        String name = scanner.nextLine();
                        System.out.print("Enter address: ");
                        String address = scanner.nextLine();
                        System.out.print("Enter contact: ");
                        String contact = scanner.nextLine();
                        System.out.print("Enter Member ID: ");
                        int newMemberID = Integer.parseInt(scanner.nextLine()); // if this input is a String it will catch this error

                        // checks if member ID is already taken
                        if(library.checkMemberID(newMemberID)) {
                            System.out.println("Member ID: " + newMemberID + " is taken. Try again.");
                            // if member ID is available it will add the member to the Library's LibraryMember array
                        } else {
                            LibraryMember member = new LibraryMember(name, address, contact, newMemberID);
                            library.addMember(member);
                        }
                        break;

                    } catch (Exception e) {
                        System.out.println("Invalid input. Try again.");
                        break;
                    }


                case "3":

                    try {
                        System.out.print("Enter member ID: ");
                        int borrowMemberID = Integer.parseInt(scanner.nextLine());
                        // checks if the member ID matches to a member in the LibraryMembers array
                        // if it's not a match it will return to main menu
                        boolean memberExists = library.checkMemberID(borrowMemberID);
                        if (!memberExists) {
                            System.out.println("Member does not exist. Try again.");
                            break;
                        }

                        System.out.print("Enter item ID to borrow: ");
                        int borrowItemID = Integer.parseInt(scanner.nextLine());
                        // checks if the item ID matches to an item in the LibraryItems array
                        // if it's not a match it will return to main menu
                        boolean itemExists = library.checkItemId(borrowItemID);
                        // if the item is not available for checkout
                        if (!itemExists) {
                            System.out.println("Item does not exist. Try again.");
                            break;
                        }

                        // if both the memberID and itemID match, the item can be borrowed if its available in the library
                        LibraryItem borrowedItem = library.getItem(borrowItemID);
                        if (borrowedItem.isAvailable()) {
                            library.getMember(borrowMemberID).borrowItem(borrowedItem);
                        }else {
                            System.out.println("Item has been checked out.");
                        }

                        break;
                    } catch (Exception e) {
                        System.out.println("Invalid input. Try again.");
                        break;
                    }


                case "4":
                    try {
                        System.out.print("Enter member ID: ");
                        int returnMemberID = Integer.parseInt(scanner.nextLine());
                        // checks if the member ID matches to a member in the LibraryMembers array
                        // if it's not a match it will return to main menu
                        boolean memberExists = library.checkMemberID(returnMemberID);
                        if (!memberExists) {
                            System.out.println("Member does not exist. Try again.");
                            break;
                        }
                        System.out.print("Enter item ID to return: ");
                        int returnItemID = Integer.parseInt(scanner.nextLine());
                        // checks if the item ID matches to an item in the LibraryItems array
                        // if it's not a match it will return to main menu
                        boolean itemExists = library.checkItemId(returnItemID);
                        if (!itemExists) {
                            System.out.println("Item does not exist. Try again.");
                            break;
                        }
                        // if both the memberID and itemID match, the item can be returned if it is not in the library
                        LibraryItem returningItem = library.getItem(returnItemID);
                        if (!returningItem.isAvailable()) {
                            library.getMember(returnMemberID).returnItem(returningItem);
                        }else {
                            System.out.println("Item has not been checked out.");
                        }

                        break;

                    } catch (Exception e) {
                        System.out.println("Invalid input. Try again.");
                        break;
                    }
                case "5":
                    System.out.println(library);
                    break;
                case "6":
                    System.out.println("Exiting...");
                    break;

                // any input to "Enter your choice:" that is not 1-6, will jump to the default case and print invalid option
                default:
                    System.out.println("Invalid option. Try again.");
                    break;
            }

        } while (!option.equals("6")); // stops the loop once the user input "6"

    }
}