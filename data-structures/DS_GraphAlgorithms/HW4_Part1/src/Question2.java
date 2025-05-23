import java.util.*;
import java.io.*;

class Island {
    int id;
    double latitude;
    double longitude;

    public Island(int id, double latitude, double longitude) {
        this.id = id;
        this.latitude = latitude;
        this.longitude = longitude;
    }
}

class Edge implements Comparable<Edge> {
    int src;
    int dest;
    double weight;

    public Edge(int src, int dest, double weight) {
        this.src = src;
        this.dest = dest;
        this.weight = weight;
    }

    @Override
    public int compareTo(Edge other) {
        return Double.compare(this.weight, other.weight);
    }
}

class DisjointSet {
    int[] parent;
    int[] rank;

    public DisjointSet(int n) {
        parent = new int[n];
        rank = new int[n];

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    public int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    public void union(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) return;

        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

public class Question2 {
    // calculates the distance between two points
    public static double getDirectDistance(double lat1, double lon1, double lat2, double lon2) {
        //finds the direct distance in meters
        //point 1 at (lat1° N, lon1° W) and point 2 at (lat2° N, lon2° W)
        //formula to find correct distance in meters:
        double psi1 = lat1 * Math.PI/180;
        double psi2 = lat2 * Math.PI/180;
        double deltaPsi = psi2 - psi1;
        double deltaLambda = (lon2-lon1) * Math.PI/180;
        double a = Math.sin(deltaPsi/2) * Math.sin(deltaPsi/2) +
                Math.cos(psi1) * Math.cos(psi2) *
                        Math.sin(deltaLambda/2) * Math.sin(deltaLambda/2);
        return  2 * 6371e3 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
    }

    // Kruskal's algorithm for minimum spanning tree
    public static double findMinimumBridgeLength(List<Island> islands) {
        // stores all possible edges in an arraylist
        List<Edge> edges = new ArrayList<>();
        for (int i = 0; i < islands.size(); i++) {
            for (int j = i + 1; j < islands.size(); j++) {
                Island island1 = islands.get(i);
                Island island2 = islands.get(j);
                double distance = getDirectDistance(
                        island1.latitude, island1.longitude,
                        island2.latitude, island2.longitude
                );
                edges.add(new Edge(i, j, distance));
            }
        }

        // sorts the edges by distance
        Collections.sort(edges);

        // kruskals alg.
        DisjointSet ds = new DisjointSet(islands.size());
        double totalBridgeLength = 0;
        int edgesAdded = 0;
        List<Edge> resultEdges = new ArrayList<>();

        for (Edge edge : edges) {
            if (ds.find(edge.src) != ds.find(edge.dest)) {
                ds.union(edge.src, edge.dest);
                totalBridgeLength += edge.weight;
                resultEdges.add(edge);
                edgesAdded++;

                if (edgesAdded == islands.size() - 1) {
                    break;
                }
            }
        }

        // prints the bridges in order by shortest distance
        System.out.println("\nBridge connections from shortest to longest distance:");
        for (Edge edge : resultEdges) {
            Island src = islands.get(edge.src);
            Island dest = islands.get(edge.dest);
            System.out.printf("Island %d to Island %d: %.2f meters\n",
                    src.id, dest.id, edge.weight);
        }

        return totalBridgeLength;
    }

    public static void main(String[] args) {
        try {
            // reads island csv file
            List<Island> islands = readIslandsFromFile("src/islandslocations.csv");

            //finds the min length for all bridges
            double minimumLength = findMinimumBridgeLength(islands);

        } catch (IOException e) {
            System.err.println("Error reading CSV file: " + e.getMessage());
            e.printStackTrace();
        }
    }

    // buffered reader for csv
    public static List<Island> readIslandsFromFile(String filename) throws IOException {
        List<Island> islands = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;

        // Skip header line (assuming the CSV has a header)
        reader.readLine();

        while ((line = reader.readLine()) != null && !line.trim().isEmpty()) {
            String[] parts = line.split(",");
            if (parts.length >= 3) {
                int id = Integer.parseInt(parts[0].trim());
                double latitude = Double.parseDouble(parts[1].trim());
                double longitude = Double.parseDouble(parts[2].trim());
                islands.add(new Island(id, latitude, longitude));
            }
        }
        reader.close();

        return islands;
    }
}