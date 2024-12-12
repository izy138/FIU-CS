import java.util.Scanner;

class DigitSum{
    // recursion
    public static int sumOfDigits(int n) {
        // base case
        if (n<10){
            return n;
        }
        // recursion for summing each digit
        // when n is 16: 16 % 10 = 6 + sumOfDigits(16/10)
        // 16/10 = 1 => sumOfDigits(1) base case 1<10 returns 1
        // 1 + 6 = 7
        return (n % 10) + sumOfDigits(n/10);
    }
}
class Handshakes{
    public static int totalHandshakes(int n) {
        // base case - no handshakes for 1 or 0 people
        if (n <= 1){
            return 0;
        }
        // base case - two people can only handshake once
        if (n == 2){
            return 1;
        }
        // recursion for total handshakes
        // n is 4 = 4-1 = 3
        // 3 + totalHandshakes(3)
        // totalHandshakes(3) returns 2 + totalHandshakes(2)
        // totalHandshakes(2) returns 1.
        // 1 + 2 + 3 = 6
        return (n - 1) + totalHandshakes(n-1);
    }
}
public class Main {
    public static void main(String[] args) {
        System.out.println();
        System.out.println("Digit Sum");
        System.out.println("------------");
        Scanner scn = new Scanner(System.in);
        System.out.print("Enter a number: ");
        int n = scn.nextInt();

        int sum = DigitSum.sumOfDigits(n);
        System.out.println("Sum of digits of " + n + ": " + sum);

        System.out.println();
        System.out.println("Handshakes");
        System.out.println("------------");
        System.out.print("Enter number of people: ");
        int people = scn.nextInt();

        int handshakes = Handshakes.totalHandshakes(people);
        System.out.println("Total number of handshakes for "+ people+ " people: " + handshakes);
    }
}
//2, 3, 4, 5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,  16
//2, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120