public class Rectangle extends Shape implements Resizable{
    private double length;
    private double width;

    public Rectangle(String name, String color, double length, double width) {
        super(name, color);
        this.length = length;
        this.width = width;
    }
    // overrides the calculateArea method in the Shape class
    @Override
    public double calculateArea() {
        return length * width;
    }
    // overrides the calculatePerimeter method in the Shape class

    @Override
    public double calculatePerimeter() {
        return 2 * (length + width);
    }

    public double getLength() {
        return length;
    }

    public double getWidth() {
        return width;
    }

    //overrides the resize method in the interface Resizable
    @Override
    public void resize(int percent) {
        length *= (1.0 + (percent / 100.0));
        width *= (1.0 + (percent / 100.0));

    }
}
