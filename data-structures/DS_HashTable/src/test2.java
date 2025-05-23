import java.util.LinkedList;

class KeyValuePair {
    String key;
    double value;

    public KeyValuePair(String key, double value) {
        this.key = key;
        this.value = value;
    }

    @Override
    public String toString() {
        return "(" + key + ", " + value + ")";
    }
}

class HashMapExample {
    private LinkedList<KeyValuePair>[] table;
    private int size;

    @SuppressWarnings("unchecked")
    public HashMapExample(int capacity) {
        this.table = new LinkedList[capacity];
        this.size = capacity;

        // Initialize each bin with an empty LinkedList
        for (int i = 0; i < capacity; i++) {
            table[i] = new LinkedList<>();
        }
    }

    public int hashCode(String key) {
        int h = 0;
        for (char val : key.toCharArray())
            h = 13 * h + val;
        return h;
    }

    public int getBinIndex(String key) {
        int hashCode = hashCode(key);
        return Math.abs(hashCode % size);
    }

    public void put(String key, double value) {
        int index = getBinIndex(key);

        // Check if key already exists, update if it does
        for (KeyValuePair pair : table[index]) {
            if (pair.key.equals(key)) {
                pair.value = value;
                return;
            }
        }

        // If key doesn't exist, add new pair
        table[index].add(new KeyValuePair(key, value));
    }

    public void printTable() {
        for (int i = 0; i < size; i++) {
            System.out.print("Bin " + i + ": ");
            if (table[i].isEmpty()) {
                System.out.println();
            } else {
                for (KeyValuePair pair : table[i]) {
                    System.out.print(pair + " ");
                }
                System.out.println();
            }
        }
    }

    public static void main(String[] args) {
        HashMapExample hashMap = new HashMapExample(7);

        // Insert the key-value pairs
        hashMap.put("apple", 3.25);
        hashMap.put("orange", 2.50);
        hashMap.put("kale", 5.45);
        hashMap.put("leek", 3.25);
        hashMap.put("dragon fruit", 8.50);
        hashMap.put("kiwi", 4.25);
        hashMap.put("banana", 1.25);

        // Print the hash codes and bin indices
        String[] keys = {"apple", "orange", "kale", "leek", "dragon fruit", "kiwi", "banana"};
        for (String key : keys) {
            int hashCode = hashMap.hashCode(key);
            int binIndex = hashMap.getBinIndex(key);
            System.out.println("Key: " + key + ", Hash Code: " + hashCode + ", Bin: " + binIndex);
        }

        // Print the final table
        System.out.println("\nFinal Hash Table:");
        hashMap.printTable();
    }
}public class test2 {

}
