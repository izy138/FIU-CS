import java.util.*;
import java.io.*;

public class Question5 {
    public static void traversePrerequisitesDFS(String filename) {
        try {
            //uses read prereqs method from question 3
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

            // find the start vertex with no indegrees (no prereqs)
            List<Vertex> startVertices = new ArrayList<>();
            for (Vertex v : courseVertices.values()) {
                if (v.indegree == 0) {
                    startVertices.add(v);
                }
            }

            //  Depth First Search
            Set<Vertex> visited = new HashSet<>();

            System.out.println("\nDFS Traversal Order:");

            for (Vertex start : startVertices) {
                if (!visited.contains(start)) {
                    dfsHelper(start, graph, visited);
                }
            }

        } catch (IOException e) {
            System.err.println("Error reading prerequisites file: " + e.getMessage());
        }
    }

    private static void dfsHelper(Vertex current, HashMap<Vertex, Set<Vertex>> graph, Set<Vertex> visited) {
        visited.add(current);
        System.out.println(current.name);

        if (graph.containsKey(current)) {
            for (Vertex neighbor : graph.get(current)) {
                if (!visited.contains(neighbor)) {
                    dfsHelper(neighbor, graph, visited);
                }
            }
        }
    }

    public static void main(String[] args) {

        traversePrerequisitesDFS("src/prerequisite.txt");

    }
}