public class Library {
    public LibraryItem[] items;
    public LibraryMember[] members;

    public Library() {
        this.items = new LibraryItem[10];
        this.members = new LibraryMember[10];
    }

    // adding the item to array
    public void addItem(LibraryItem item) {
        // starting from index 0 it should add the item to an open spot
        for (int i=0 ; i<items.length ; i++) {
            if (items[i] == null) {
                items[i] = item;
                break;
            }
            // once it reaches the last index the library is full
            if(i == items.length-1){
                System.out.println("Library is full, Cannot add more items.");
            }
        }
    }
    // adding a member to array
    public void addMember(LibraryMember member) {
        // starting from index 0 it should add the member to an open spot
        for (int i=0 ; i<members.length ; i++) {
            if(members[i] == null){
                members[i] = member;
                break;
            }
            // once it reaches the last index the members list is full
            if(i == members.length-1){
                System.out.println("Members list is full, Cannot add more members.");
            }
        }
    }

    public boolean checkMemberID(int memberID) {
// checks the memberid that is inputted by the user by going through each index in the array
        //when it find a match with the memberID is returns true.
        for (int i = 0; i <= members.length - 1; i++) {
            if (members[i] != null) {
                if (members[i].getMemberId() == memberID) {
                    return true;

                }
            }
        }
        return false; // if no match is found it returns false
    }

    // checks the itemid that is inputted by the user by going through each index in the array
    // when it find a match with the itemID is returns true.
    public boolean checkItemId(int itemID) {
        for (int i = 0; i <= items.length - 1; i++) {
            if (items[i] != null) {
                if (items[i].getItemId() == itemID) {
                    return true;
                }
            }
        }
        return false;// if no match is found it returns false
    }
    // gets the item that matches to the itemID the user inputs by going through each index in the array
    // if the itemid is a match for one of the items in the array, it returns true.
    public LibraryItem getItem(int itemID){
        for (int i = 0; i <= items.length - 1; i++) {
            if (items[i] != null) {
                if (items[i].getItemId() == itemID) {
                    return items[i];
                }
            }
            if (i == items.length - 1) { // once it reaches the end of the array, and no match has been found.
                System.out.println("Item not found.");

            }
        }
        return null;
    }
    // gets the member that matches to the memberID the user inputs by going through each index in the array
    // if the memberid is a match for one of the members in the array, it returns true.
    public LibraryMember getMember(int memberID){
        for (int i = 0; i <= members.length - 1; i++) {
            if (members[i] != null) {
                if (members[i].getMemberId() == memberID) {
                    return members[i];
                }
            }
            if (i == members.length - 1) { // once it reaches the end of the array, and no match has been found.
                System.out.println("Member not found.");

            }
        }
        return null;
    }

    // displays the total number of available items and the total number of members in the library system.
    public String toString() {
        int itemCounter = 0;
        for (int i=0 ; i<items.length ; i++) {
            if(items[i] != null && items[i].isAvailable()){  // checks if the item is available in the library
                itemCounter ++;
            }
        }
        int memberCounter = 0;
        for (int i=0 ; i<members.length ; i++) {
            if(members[i] != null){
                memberCounter ++;
            }
        }
        return "Library: Items - " + itemCounter + ", Members - " + memberCounter;
    }

}
