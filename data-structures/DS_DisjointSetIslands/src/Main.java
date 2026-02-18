import java.util.*;

public class Main {
    // Assuming DisjointSet<T> is already defined and in the same package

    public static boolean canTravel(
            HashMap<String, List<String>> bridges,
            String source,
            String destination){
        //creates a disjoint set to hold the island and its connections
        DisjointSet<String> disjointSet = new DisjointSet<>();

        for (Map.Entry<String, List<String>> entry : bridges.entrySet()) {
            //gets the island and for each connection makes a union in the disjoint set of its other connections
            String island = entry.getKey();
            for (String connectedIsland : entry.getValue()) {
                disjointSet.union(island, connectedIsland);
            }
        }
        //boolean to check if the source has a connection in the disjoint set corresponding to the destination
        return disjointSet.find(source).equals(disjointSet.find(destination));
    }

    public static void main(String[] args) {
        HashMap<String, List<String>> bridges = new HashMap<>();

        // adding connections between islands
        bridges.put("Alpha", Arrays.asList("Brave", "Charlie"));
        bridges.put("Bravo", Arrays.asList("Alpha", "Delta"));
        bridges.put("Charlie", Arrays.asList("Alpha"));
        bridges.put("Delta", Arrays.asList("Bravo"));
        bridges.put("Echo", Arrays.asList("Foxtrot"));
        bridges.put("Foxtrot", Arrays.asList("Echo"));

        System.out.println("Connections:");
        System.out.println(bridges);
        System.out.println();

        // Test cases
        System.out.println(canTravel(bridges, "Alpha", "Delta")); // true
        System.out.println(canTravel(bridges, "Alpha", "Foxtrot")); // false
        System.out.println(canTravel(bridges, "Echo", "Foxtrot")); // true
        System.out.println(canTravel(bridges, "Bravo", "Echo")); //false
    }
}
