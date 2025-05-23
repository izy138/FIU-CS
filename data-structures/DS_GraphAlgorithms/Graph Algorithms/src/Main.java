import algorithms.*;
import util.*;

public class Main{
    public static void main(String[] args) {
        // Create vertices for the graph
        String[] vertices = {"A", "B", "C", "D", "E", "F", "O", "T"};

        // Create edges with weights (for undirected graph, add each edge twice)
        String[] edges = {
                // Edge from A to other vertices
                "A,B,2", "B,A,2",     // A-B with weight 2
                "A,D,7", "D,A,7",     // A-D with weight 7
                "A,F,12", "F,A,12",   // A-F with weight 12
                "A,O,5", "O,A,5",     // A-O with weight 5

                // Edge from B to other vertices
                "B,C,1", "C,B,1",     // B-C with weight 1
                "B,D,4", "D,B,4",     // B-D with weight 4
                "B,E,3", "E,B,3",     // B-E with weight 3

                // Edge from C to other vertices
                "C,E,4", "E,C,4",     // C-E with weight 4
                "C,O,4", "O,C,4",     // C-O with weight 4

                // Edge from D to other vertices
                "D,E,4", "E,D,4",     // D-E with weight 4
                "D,T,5", "T,D,5",     // D-T with weight 5

                // Edge from E to other vertices
                "E,T,7", "T,E,7",     // E-T with weight 7

                // Edge from F to other vertices
                "F,T,3", "T,F,3"      // F-T with weight 3
        };

        // Create the weighted graph
        WeightedGraph graph = new WeightedGraph(vertices, edges);

        // Run Dijkstra's algorithm with A as the source
        try {
            ShortestPath.dijkstra(graph, "A");
            System.out.println("Shortest paths from vertex A:");

            // Print paths from vertex A to all other vertices
            for (Vertex v : graph.vertexSet()) {
                if (!v.name.equals("A")) { // Skip the source vertex itself
                    System.out.print("A to " + v.name + " (distance: " + v.rank + "): ");
                    printPath(v, graph);
                    System.out.println();
                }
            }
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Method to print the path from source to vertex
    private static void printPath(Vertex target, Graph graph) {
        if (target.pred == null) {
            // This is either the source or there is no path
            if (target.rank == Integer.MAX_VALUE) {
                System.out.print("No path exists");
            } else {
                System.out.print(target);
            }
            return;
        }

        printPath(target.pred, graph);
        System.out.print(" -> " + target);
    }
}