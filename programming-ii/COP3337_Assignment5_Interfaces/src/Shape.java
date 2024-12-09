// abstract parent class for shapes.
// includes name and color and a constructor that initializes them.
abstract class Shape {
    protected String name;
    protected String color;

    public Shape(String name, String color) {
        this.name = name;
        this.color = color;
    }
    public String getName() {
        return name;
    }
    public String getColor() {
        return color;
    }

    // abstract methods for calculating area and perimeter.
    // varies by shape so it is abstract.
    public abstract double calculateArea();
    public abstract double calculatePerimeter();


}
