public class Square extends Shape implements Resizable{
    private double length;

    public Square(String name, String color, double length) {
        super(name, color);
        this.length = length;
    }
    // overrides the calculateArea method in the Shape class
    @Override
    public double calculateArea() {
        return length * length;
    }
    // overrides the calculatePerimeter method in the Shape class
    @Override
    public double calculatePerimeter() {
        return 4 *  length;
    }

    public double getLength() {
        return length;
    }

    //overrides the resize method in the interface Resizable
    @Override
    public void resize(int percent) {
        length *= (1.0 + (percent / 100.0));
    }
}
