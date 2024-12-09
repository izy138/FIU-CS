public class ShapeTest {
    public static void main(String[] args) {
        //create the 4 shapes
        Circle circle = new Circle("Circle", "Red", 5);
        Rectangle rectangle = new Rectangle("Rectangle", "Blue", 4.0, 6.0);
        Square square = new Square("Square", "Green", 5.123);
        RightTriangle rightTriangle = new RightTriangle("Right Triangle", "Yellow", 4.12, 6.0);

        System.out.println("\nCircle:");
        System.out.println("Name: " + circle.getName());
        System.out.println("Color: " + circle.getColor());
        System.out.println("Radius: " + formatNum(circle.getRadius()));//String.format("%.1f", circle.getRadius()));
        System.out.println("Area: " + formatNum(circle.calculateArea()));// String.format("%.2f", circle.calculateArea()));
        System.out.println("Perimeter: " + formatNum(circle.calculatePerimeter()));//String.format("%.2f", circle.calculatePerimeter()));
        circle.resize(50);
        System.out.println("Resized Circle - Radius: " + circle.getRadius());//String.format("%.1f", circle.getRadius()));

        System.out.println("\nRectangle:");
        System.out.println("Name: " + rectangle.getName());
        System.out.println("Color: " + rectangle.getColor());
        System.out.println("Length: " + formatNum(rectangle.getLength()));//String.format("%.2f", rectangle.getLength()));
        System.out.println("Width: " + formatNum(rectangle.getWidth()));
        System.out.println("Area: " + formatNum(rectangle.calculateArea()));
        System.out.println("Perimeter: " + formatNum(rectangle.calculatePerimeter()));
        rectangle.resize(50);
        System.out.println("Resized Rectangle - Length: " + formatNum(rectangle.getLength()) + ", Width: " + formatNum(rectangle.getWidth()));

        System.out.println("\nSquare:");
        System.out.println("Name: " + square.getName());
        System.out.println("Color: " + square.getColor());
        System.out.println("Side: " + formatNum(square.getLength()));
        System.out.println("Area: " + formatNum(square.calculateArea()));
        System.out.println("Perimeter: " + formatNum(square.calculatePerimeter()));
        square.resize(50);
        System.out.println("Resized Square - Side: " + formatNum(square.getLength()));

        System.out.println("\nRight Triangle:");
        System.out.println("Name: " + rightTriangle.getName());
        System.out.println("Color: " + rightTriangle.getColor());
        System.out.println("Base: " + formatNum(rightTriangle.getBase()));
        System.out.println("Height: " + formatNum(rightTriangle.getHeight()));
        System.out.println("Hypotenuse: " + formatNum(rightTriangle.getHypotenuse()));
        System.out.println("Area: " + formatNum(rightTriangle.calculateArea()));
        System.out.println("Perimeter: " + formatNum(rightTriangle.calculatePerimeter()));
        rightTriangle.resize(50); // Resizing by 50%
        System.out.println("Resized Right Triangle - Base: " + formatNum(rightTriangle.getBase()) + ", Height: " + formatNum(rightTriangle.getHeight()) + ", Hypotenuse: " + formatNum(rightTriangle.getHypotenuse()));
    }

    //formatting the display for:
    // if the number input is a whole number it will be displayed with one decimal place: 4.0
    // all other numbers will be displayed without rounding to a decimal place: 7.278351461697903
    public static String formatNum(double value) {
        // convert decimal number to string
        String valueStr = Double.toString(value);

        // check if it's a whole number (4.0) and display with one decimal place
        if (value == Math.floor(value)) {
            return String.format("%.1f", value);
        }
        // return the number as it is without rounding
        return valueStr;
    }
}