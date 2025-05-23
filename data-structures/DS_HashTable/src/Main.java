import java.util.LinkedList;

class HashTable {
    // Number of bins in the hash table
    private static final int SIZE = 7;

    // Array of linked lists for separate chaining
    private LinkedList<Node>[] table;

    // Node class to store key-value pairs
    static class Node {
        String key;
        double value;

        Node(String key, double value) {
            this.key = key;
            this.value = value;
        }
    }

    // Constructor: Initialize the table with empty linked lists
    public HashTable() {
        table = new LinkedList[SIZE];
        for (int i = 0; i < SIZE; i++) {
            table[i] = new LinkedList<>();
        }
    }

    // Custom hash function based on given formula
    private int getHash(String key) {
        int h = 0;
        for (char val : key.toCharArray()) {
            h = 13 * h + val;
        }
        return Math.abs(h) % SIZE;  // Ensure non-negative index
    }

    // Insert key-value pair into the hash table
    public void insert(String key, double value) {
        int index = getHash(key);
        table[index].add(new Node(key, value)); // Add to linked list at index
    }

    // Print the hash table for verification
    public void printTable() {
        for (int i = 0; i < SIZE; i++) {
            System.out.print("Bin " + i + ": ");
            for (Node node : table[i]) {
                System.out.print("(" + node.key + ", " + node.value + ") → ");
            }
            System.out.println("null");
        }
    }

    // Main function to test the hash table
    public static void main(String[] args) {
        HashTable ht = new HashTable();

        // Insert given key-value pairs
        ht.insert("apple", 3.25);
        ht.insert("orange", 2.50);
        ht.insert("kale", 5.45);
        ht.insert("leek", 3.25);
        ht.insert("dragon fruit", 8.50);
        ht.insert("kiwi", 4.25);
        ht.insert("banana", 1.25);

        // Print the hash table
        ht.printTable();
    }
}
