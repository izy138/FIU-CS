public class Product {

    private int productID;
    private String name;
    private double price;
    private int quantity;

    public Product() {
    }

    public Product(int productID, String name, double price, int quantity) {
        this.productID = productID;
        this.name = name;
        this.price = price;

        //if quantity is negative it throws an exception
        if (quantity < 0) {
            throw new IllegalArgumentException("Quantity cannot be negative.");

        }else {
            this.quantity = quantity;}
    }

    public int getProductID() {
        return productID;
    }

    public void setProductID(int productID) {
        this.productID = productID;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        if (quantity < 0) {
            throw new IllegalArgumentException("Quantity cannot be negative.");
        }else{
            this.quantity = quantity;
        }
    }
    // calculates total item value
    public double calculateTotalValue() {
        return price * quantity;
    }

    // if restock amount is negative it throws an exception
    // else the restock amount is added to the quantity and printed.
    public void restock(int restockAmount) {
        if (restockAmount < 0) {
            throw new IllegalArgumentException("Restock amount cannot be negative.");
        }else {
            this.quantity += restockAmount;
            System.out.println("\nRestocking " + name + " by " + restockAmount + "...\nNew Quantity: " + quantity + "\nNew Total Value: $" + String.format("%.2f", calculateTotalValue()));
        }
    }

    @Override
    public String toString() {
        return "\nProduct ID: "+ productID + "" +
                "\nName: " + name +
                "\nPrice: $" + String.format("%.2f", price) +
                "\nQuantity: "+ quantity;

    }
}