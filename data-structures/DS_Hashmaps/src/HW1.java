import java.util.*;

public class HW1 {

    public static char findDistinguishingChar(String str1, String str2) {
        Map<Character, Integer> freq1 = new HashMap<>();
        Map<Character, Integer> freq2 = new HashMap<>();

        System.out.println("Frequency of first string: " + str1);
        // frequency for string 1
        for(char c : str1.toCharArray()) {
            freq1.put(c, freq1.getOrDefault(c, 0) + 1);
        }

        System.out.println(freq1);
        // frequency for string 2
        for(char c : str2.toCharArray()) {
            freq2.put(c, freq2.getOrDefault(c, 0) + 1);
        }
        System.out.println("\nFrequency of second string: " + str2);
        System.out.println(freq2);
        // hash set to hold unique characters in both strings
        // "hello" and "world" = [r, d, e, w, h, l, o]
        Set<Character> charSet = new HashSet<>();
        charSet.addAll(freq1.keySet());
        charSet.addAll(freq2.keySet());

        System.out.println("\nSet of all characters:");
        System.out.println(charSet);

        char result = '\0';
        int maxDifference = 0; // will return the last character checked with the highest difference

        for (char c : charSet) {
            int count1 = freq1.getOrDefault(c,0);
            int count2 = freq2.getOrDefault(c,0);
            int diff = Math.abs(count1 - count2);

            if (diff >= maxDifference) {
                maxDifference = diff;
                result = c;
            }
        }
        System.out.print("\nMost Distinguished Char: ");
        return result;
    }

    public static boolean isAPotentialPalidrome(String str) {
        Map<Character, Integer> freq = new HashMap<>();

        // frequency of chars
        for(char c : str.toCharArray()) {
            freq.put(c, freq.getOrDefault(c, 0) + 1);
        }
        // dads 2 odd character = X
        // dad  1 odd character = palidrome
        // noon 0 odd characters = palidrome
        int oddCount = 0;
        //counts the number of odd chars in string
        for (int count : freq.values()) {
            if (count % 2 != 0) {
                oddCount++;
            }
        }
        System.out.print(str + ": ");
        return oddCount <= 1;
    }

    public static int sumOfUniqueElements(int[] arr) {
        Map<Integer, Integer> countMap = new HashMap<>();
        for (int num : arr) {
            countMap.put(num, countMap.getOrDefault(num, 0) + 1);
        }
        System.out.println("Array: " + Arrays.toString(arr) );
        System.out.print("Unique elements are: ");
        int sum = 0;
        // goes through each element in the HashMap
        // checks if the frequency value is 1 and adds it to sum
        for (Map.Entry<Integer,Integer> entry : countMap.entrySet()) {
            if (entry.getValue() == 1) {
                System.out.print(entry.getKey() + " ");
                sum += entry.getKey();
            }
        }
        System.out.print("\nSum of unique elements: ");
        return sum;

    }

    public static int countGreaterThanValue(BinaryNode<Integer> root, int value){
        if(root == null){
            return 0;
        }
        int count = 0;

        // checks if the element is greater than the value given
        if(root.element > value) {
            count = 1;
        }
        // recursively compares all elements of the tree with the value and add to the count
        count += countGreaterThanValue(root.left, value);
        count += countGreaterThanValue(root.right, value);
        return count;
    }

    public static boolean isSymmetric(BinaryNode<Integer> root) {
        if(root == null){
            return true;
        }
        // checks each node to the left and right of root
        return mirror(root.left, root.right);
    }

    // compares both root nodes to each other
    public static boolean mirror(BinaryNode left, BinaryNode right) {
        // checks if both nodes are null then true = symmetric
        if(left == null && right == null){
            return true;
        }
        // if one or the other node is null then false = not symmetric
        if(left == null || right == null){
            return false;
        }

        return (left.element.equals(right.element)) // checks if the elements inside the nodes are the same
                && mirror(left.left, right.right) // checks symmetry of the left.left and the right.right
                && mirror(left.right, right.left); // checks symmetry of the left.right and right.left
    }

    public static BinaryNode removeLeaves(BinaryNode root) {
        if (root == null){
            // if the root is null
            return null;
        }
        // if root.left is null or the root.right is null
        // if the root has a missing child return null
        if (root.left == null && root.right == null){
            root = null;
            return root;
        }
//
        root.left = removeLeaves(root.left);// recursively calls the method, removing all left children
        root.right = removeLeaves(root.right); // recursively calls the method, removing all right children

//        root.left = null;
//        root.right = null;

        if (root.left != null || root.right != null){
            root.left = null;
            root.right = null;
        }
//        System.out.println(root.left);
//        System.out.println(root.right);

        //after all left and right subtrees are removed return the tree root.
        return root;
    }

    public static void main(String[] args) {
        System.out.println("\nProblem 1: ");
        System.out.println("-----------------");
        System.out.println(findDistinguishingChar("hello", "world"));

        System.out.println("\nProblem 2:");
        System.out.println("-----------------");
        System.out.println("Potential palindrome?");
        System.out.println(isAPotentialPalidrome("dads"));
        System.out.println(isAPotentialPalidrome("dad"));
        System.out.println(isAPotentialPalidrome("noon"));

        System.out.println("\nProblem 3:");
        System.out.println("-----------------");
        System.out.println(sumOfUniqueElements(new int[]{1,2,2,3,4,4}));
        System.out.println();
        System.out.println(sumOfUniqueElements(new int[]{1,2,3,4,4,4}));

        System.out.println("\nProblem 4:");
        System.out.println("-----------------");
        int element = 3;
        BinaryNode<Integer> root4 = new BinaryNode<>(6);
        root4.left = new BinaryNode<>(3);
        root4.right = new BinaryNode<>(8);
        root4.left.left = new BinaryNode<>(1);
        root4.left.right = new BinaryNode<>(4);
        root4.right.left = new BinaryNode<>(10);
        root4.right.right = new BinaryNode<>(13);
        System.out.println(root4.toString2());
        System.out.println("Elements greater than " + element + ": "+ countGreaterThanValue(root4, element));

        System.out.println("\nProblem 5:");
        System.out.println("-----------------");
        BinaryNode<Integer> symmetricRoot = new BinaryNode<>(1);
        symmetricRoot.left = new BinaryNode<>(2);
        symmetricRoot.right = new BinaryNode<>(2);
        symmetricRoot.left.left = new BinaryNode<>(3);
        symmetricRoot.left.right = new BinaryNode<>(4);
        symmetricRoot.right.left = new BinaryNode<>(4);
        symmetricRoot.right.right = new BinaryNode<>(3);
        System.out.println("Tree is Symmetric?");
        System.out.println(isSymmetric(symmetricRoot));
        System.out.println(symmetricRoot.toString2());

        BinaryNode<Integer> nonSymmetricRoot = new BinaryNode<>(1);
        nonSymmetricRoot.left = new BinaryNode<>(2);
        nonSymmetricRoot.right = new BinaryNode<>(2);
        nonSymmetricRoot.left.left = new BinaryNode<>(3);
        nonSymmetricRoot.right.left = new BinaryNode<>(4);
        nonSymmetricRoot.right.right = new BinaryNode<>(3);
        System.out.println("Tree is Symmetric?");
        System.out.println(isSymmetric(nonSymmetricRoot));
        System.out.println(nonSymmetricRoot.toString2());

        System.out.println("\nProblem 6:");
        System.out.println("-----------------");
        BinaryNode<Integer> problem6Tree = new BinaryNode<>(10);
        problem6Tree.left = new BinaryNode<>(8);
        problem6Tree.right = new BinaryNode<>(12);
        problem6Tree.left.left = new BinaryNode<>(7);
        problem6Tree.left.right = new BinaryNode<>(9);
        problem6Tree.right.left = new BinaryNode<>(11);
        problem6Tree.right.right = new BinaryNode<>(13);
        System.out.println("Before removal:");
        System.out.println(problem6Tree.toString2());
        removeLeaves(problem6Tree);
        System.out.println("After removal:");
        System.out.println(problem6Tree.toString2());

    }

}
