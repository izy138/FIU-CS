import java.util.*;
import java.io.*;

public class Question4 {
    public static void traversePrerequisitesBFS(String filename) {
        try {
            //reads the prerequisites using the method made in question3
            HashMap<String, List<String>> prereqMap = Question3.readPrerequisites(filename);
            HashMap<String, Vertex> courseVertices = new HashMap<>();

            for (String course : prereqMap.keySet()) {
                courseVertices.put(course, new Vertex(course));
                for (String prereq : prereqMap.get(course)) {
                    if (!courseVertices.containsKey(prereq)) {
                        courseVertices.put(prereq, new Vertex(prereq));
                    }
                }
            }

            HashMap<Vertex, Set<Vertex>> graph = new HashMap<>();
            for (String course : prereqMap.keySet()) {
                Vertex Vertex = courseVertices.get(course);
                for (String prereq : prereqMap.get(course)) {
                    Vertex prereqVertex = courseVertices.get(prereq);
                    if (!graph.containsKey(prereqVertex))
                        graph.put(prereqVertex, new HashSet<Vertex>());
                    graph.get(prereqVertex).add(Vertex);
                }
            }

            //find the starting vertex by finding the vertex with no in-degrees (no prereqs)
            List<Vertex> startVertices = new ArrayList<>();
            for (Vertex v : courseVertices.values()) {
                if (v.indegree == 0) {
                    startVertices.add(v);
                }
            }

            // Bredth First Search
            Queue<Vertex> queue = new LinkedList<>();
            Set<Vertex> visited = new HashSet<>();

            System.out.println("\nBFS Traversal Order:");

            for (Vertex start : startVertices) {
                if (!visited.contains(start)) {
                    queue.add(start);
                    visited.add(start);

                    while (!queue.isEmpty()) {
                        Vertex current = queue.poll();
                        System.out.println(current.name);

                        if (graph.containsKey(current)) {
                            for (Vertex neighbor : graph.get(current)) {
                                if (!visited.contains(neighbor)) {
                                    visited.add(neighbor);
                                    queue.add(neighbor);
                                }
                            }
                        }
                    }
                }
            }

        } catch (IOException e) {
            System.err.println("Error reading prerequisites file: " + e.getMessage());
        }
    }

    public static void main(String[] args) {

        traversePrerequisitesBFS("src/prerequisite.txt");

    }
}
