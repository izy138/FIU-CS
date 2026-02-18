public class RightTriangle extends Shape implements Resizable{
    private double base;
    private double height;
    private double hypotenuse;

    public RightTriangle(String name, String color, double base, double height) {
        super(name, color);
        this.base = base;
        this.height = height;
        this.hypotenuse = calculateHypotenuse();
    }

    //calculates the hypotenuse of the triangle by base and height
    private double calculateHypotenuse() {
        return Math.sqrt(base * base + height * height);
    }
    // overrides the calculateArea method in the Shape class
    @Override
    public double calculateArea() {
        return base * height * 0.5;
    }
    // overrides the calculatePerimeter method in the Shape class
    @Override
    public double calculatePerimeter() {
        return base + height + hypotenuse;
    }
    public double getBase() {
        return base;
    }

    public double getHeight() {
        return height;
    }

    public double getHypotenuse() {
        return hypotenuse;
    }
    // overrides the resize method in the interface Resizable
    @Override
    public void resize(int percent) {
        base = base * (1 + percent / 100.0);
        height = height * (1 + percent / 100.0);
        hypotenuse = calculateHypotenuse();
    }
}
