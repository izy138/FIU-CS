public class Furniture extends Product {
    private String materialType;

    public Furniture(int productID,String name, double price, int quantity, String material) {
        super(productID, name, price, quantity);
        this.materialType = material;

    }
    public String getMaterialType() {
        return materialType;
    }
    public void setMaterialType(String materialType) {
        this.materialType = materialType;
    }

    // inherits the calculatetotalvalue method from the parent Product, and increases the value by 5%
    @Override
    public double calculateTotalValue() {
        return super.calculateTotalValue() * 1.05;
    }
    // inherits the toString method from the parent Product, and adds details specific to Furniture and prints total value.
    public String toString() {
        return super.toString()+ "\nMaterial Type: " + materialType + "\nTotal Value: $" + String.format("%.2f", calculateTotalValue());
    }

}



























//}
//
// public class Shape {
//public double calculateArea() {
//    return 0.0;
//}
//public String display(){
//    return "Area: " + calculateArea();
//}
//    }public class Circle extends Shape {
//    private double radius;
//    public Circle(double radius) {
//        this.radius = radius;
//    }
//    public double calculateArea() {
//        return Math.PI * radius * radius;
//    }
//
//}public class Rectangle extends Shape {
//    private double length;
//    private double width;
//    public Rectangle(double length, double width) {
//        this.length = length;
//        this.width = width;
//    }
//    public double calculateArea() {
//        return length * width;
//    }
//}
//}
//        {
//}
