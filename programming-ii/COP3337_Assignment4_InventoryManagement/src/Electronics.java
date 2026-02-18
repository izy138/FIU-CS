public class Electronics extends Product {
    private int warrantyPeriod;

    public Electronics(int productID,String name, double price, int quantity, int warrantyPeriod) {
        super(productID, name, price, quantity);
        this.warrantyPeriod = warrantyPeriod;
    }

    // inherits the calculatetotalvalue method from the parent Product, and increases the value by 10%
    @Override
    public double calculateTotalValue() {
        return super.calculateTotalValue() * 1.10;
        //increases by 10%
    }
    // inherits the toString method from the parent Product, and adds details specific to Electronics and prints total value.
    @Override
    public String toString() {
        return super.toString()+ "\nWarranty Period: " + warrantyPeriod + " months" + "\nTotal Value: $" + String.format("%.2f", calculateTotalValue());

    }
}