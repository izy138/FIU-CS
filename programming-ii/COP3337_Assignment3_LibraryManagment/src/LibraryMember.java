public class LibraryMember {
    private String name;
    private String address;
    private String contact;
    private int memberId;
    private LibraryItem[] borrowedItems;
    private double fines;

    public LibraryMember(String name, String address, String contact, int memberId) {
        this.name = name;
        this.address = address;
        this.contact = contact;
        this.memberId = memberId;
        this.borrowedItems = new LibraryItem[5];
    }
    public void borrowItem(LibraryItem item) {
        for (int i = 0; i < borrowedItems.length; i++) {
            if (borrowedItems[i] == null) {
                borrowedItems[i] = item;
                item.checkoutItem();
                System.out.println("Item borrowed successfully");
                break;
            }
            if (i == borrowedItems.length - 1) {
                System.out.println("Cannot borrow any more items.");

            }
        }
    }

    // each member has their own array with items that they have borrowed
    // if that array has the item in it, then the item is removed from the members array
    public void returnItem(LibraryItem item) {
        for (int i = 0; i < borrowedItems.length; i++) {
            if (borrowedItems[i] == item) {
                borrowedItems[i] = null;
                item.returnItem();
                System.out.println("Item returned successfully");
                return;
            }
        }
        // if the member doesn't have the item in their borrowed items list
        System.out.println("Item is not borrowed by this member: " + memberId);
    }
    public int getMemberId() {
        return memberId;
    }

//    public String toString(){
//        return name + " " + address + " " + contact + " " + memberId + " " + borrowedItems;
//    }
}