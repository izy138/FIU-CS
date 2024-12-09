import java.util.Scanner;

public class Calculator {

    // creates a private variable of type double names value equal to 0.0
    private double value = 0.0;

    // method that returns void and takes a double val and adds the val to value
    public void add(double val){
        value += val;

    }
    // method that returns void and takes a double val and subtracts the val to value
    public void subtract(double val){
        value -= val;

    }
    // method that returns void and takes a double val and multiplies the val to value
    public void multiply(double val){
        value *= val;

    }
    // method that returns void and takes a double val
    public void divide(double val){

        if (val == 0) {
            //error code shown when val is 0
            System.out.println("Error");
        } else {
            // when val is not 0, value will be divided by val and printed to console.
            value /= val;
            System.out.println(value);
        }
    }

    // public method that returns void and sets value to 0.0
    public void clear(){
        value = 0.0;
    }

    // public method that returns a double and gets the value of the variable value.
    public double getValue(){

        return value;
    }

    public static void main(String[] args) {

        Calculator calculator = new Calculator();  // creates a new Calculator instance named calculator
        Scanner scanner = new Scanner(System.in);  // creates a new Scanner
        double num1 = scanner.nextDouble();
        double num2 = scanner.nextDouble();

        System.out.println();
        System.out.println(calculator.getValue());  // fetches the value which is 0.0

        calculator.add(num1);                       //adds the input taken from the scanner and assigned to num1 and adds it to the value
        System.out.println(calculator.getValue());  //fetches the value of the variable

        calculator.multiply(3);                 //multiplies the value by 3
        System.out.println(calculator.getValue());

        calculator.subtract(num2);                  //subtracts the input taken from the scanner and assigned to num2 and subtracts it from the value
        System.out.println(calculator.getValue());

        calculator.divide(2);                   //divides the value by 2

        calculator.clear();                         //clears value to 0.0
        System.out.println(calculator.getValue());

        scanner.close();

    }

}
