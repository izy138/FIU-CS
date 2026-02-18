public class Main {
    public static void main(String[] args) {

        // Creating items to add to the Product inventory array
        Product chair = new Furniture(101, "Chair", 50.00, 1, "Wood");
        Product table = new Furniture(102, "Table", 200.00, 1, "Glass");
        Product laptop = new Electronics(103, "Laptop", 1000.00, 1, 24);
//        Product cellPhone = new Electronics(104,"Cell Phone",700.00, 1,24);

        // initialize the objects to the array
        Product[] inventory = {chair, table, laptop};


        // enhanced for loop to display using inheritance from the Products toString method.
        System.out.println("\nProduct Inventory:");
        for (Product p : inventory) {
            System.out.println(p);
        }

        // restocks all items by 20
        chair.restock(20);
        table.restock(20);
        laptop.restock(20);

        // shows the inventory again with updated quantities and value.
        System.out.println("\nUpdated Inventory: ");
        for (Product p : inventory) {
            System.out.println(p);
        }

        // enhanced for loop to go through each item in the inventory array, and add the total value of each together to get the total inventory value
        double totalInventory = 0;
        for (Product p : inventory) {
            double total = p.calculateTotalValue();
            totalInventory += total;
        }
        System.out.println("\nTotal Inventory Value: $" + String.format("%.2f", totalInventory));
        System.out.println();


        // When quantity is set to a negative number, it will throw an exception
        Product laptop1 = new Electronics(103, "Laptop", 1000.00, -2, 24);
        System.out.println(laptop1);

        // when restock is set to negative number, it will throw an exception
        chair.restock(-1);
        System.out.println(laptop1);

    }
}