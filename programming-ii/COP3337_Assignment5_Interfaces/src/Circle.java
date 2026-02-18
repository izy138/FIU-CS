public class Circle extends Shape implements Resizable{
    //circle class is a child of the Shape parent class and implements the interface Resizable
    private double radius;

    public Circle(String name, String color, double radius) {
        super(name, color);
        this.radius = radius;
    }
    // overrides the calculateArea method in the Shape class
    @Override
    public double calculateArea() {
        return Math.PI * radius * radius;
    }
    // overrides the calculatePerimeter method in the Shape class
    @Override
    public double calculatePerimeter() {
        return 2 * Math.PI * radius;
    }

    public double getRadius() {
        return radius;
    }

    //overrides the resize method in the interface Resizable
    @Override
    public void resize(int percent) {
        radius = radius * (1.0 + (percent / 100.0));

    }
}
