public class LibraryItem {
    private String title;
    private String creator;
    private int itemId;
    private boolean available = true;

    public LibraryItem() {}

    public LibraryItem(String title, String creator, int itemId) {
        this.title = title;
        this.creator = creator;
        this.itemId = itemId;
    }
    public void checkoutItem() {
        if (available) {
            available = false;
        }
    }
    public void returnItem() {
        if (!available) {
            available = true;
        }
    }
    public boolean isAvailable() {
        return available;
    }

    public int getItemId() {
        return itemId;
    }

//    public String toString() {
//        return "Title of item" + title;
//    }
}
