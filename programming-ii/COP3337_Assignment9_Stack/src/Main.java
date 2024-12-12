import java.util.Stack;

public class Main {
    public static boolean isBalanced(String expression) {
        // stores brackets in a new Stack made of characters
        Stack<Character> stack = new Stack<>();

        // goes through each char in the line
        for (char next : expression.toCharArray()) {
            // if (, [, or { it is added into the stack
            if (next == '(' || next == '[' || next == '{') {
                stack.push(next);
            }

            // checks if the next char is a closing bracket ), ], or }
            else if (next == ')' || next == ']' || next == '}') {
                // checks if the stack is empty, indicating no matches
                // returns false - unbalanced
                if (stack.isEmpty()) {
                    return false;
                }

                // assigns the last open bracket to variable char last
                char last = stack.pop();

                // checks if the next closing bracket does not match its respective opening bracket
                // returns false - unbalanced
                if ((next == ')' && last != '(') || (next == ']' && last != '[') || (next == '}' && last != '{')) {
                    return false;
                }
            }
        }

        // once all characters are checked the stack is empty.
        return stack.isEmpty();
    }

    public static void main(String[] args) {
        // sample test cases
        String[] testCase = {
                "[()]{}{[()()]()}",
                "[(])",
                "((()))",
                "[{()()}]",
                "}{"
        };

        System.out.println();
        // goes through each test case and checks if balanced
        for (String test : testCase) {
            System.out.println("Input: " + test);
            System.out.println("Output: " + (isBalanced(test) ? "Balanced" : "Not Balanced"));
            System.out.println();
        }
    }
}