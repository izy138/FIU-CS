import java.util.*;
import java.io.*;

public class Question3 {
    private static void addEdge(Vertex from, Vertex to, HashMap<Vertex, Set<Vertex>> graph) {
        if (!graph.containsKey(from))
            graph.put(from, new HashSet<>());
        graph.get(from).add(to);
        to.indegree++;
    }

    public static void topologicalSortUsingDFS(HashMap<Vertex, Set<Vertex>> graph, Vertex[] vertices) {
        LinkedList<Vertex> starters = new LinkedList<>();
        for (Vertex v : vertices)
            if (v.indegree == 0)
                starters.add(v);
        Stack<Vertex> stack = new Stack<>();
        int time = 0;
        int topNum = vertices.length;
        for (Vertex startVertex : starters) {
            stack.push(startVertex);
            while (!stack.empty()) {
                Vertex current = stack.pop();
                if (current.finished > 0)
                    continue;
                if (current.processed) {
                    current.finished = ++time;
                    current.topNum = topNum--;
                    continue;
                }
                current.discovered = ++time;
                if (!graph.containsKey(current)) {
                    current.finished = ++time;
                    current.topNum = topNum--;
                    continue;
                }
                current.processed = true;
                stack.push(current);
                for (Vertex neighbor : graph.get(current))
                    if (neighbor.discovered == 0) {
                        neighbor.pred = current;
                        stack.push(neighbor);
                    }
            }
        }
    }

    public static void printCourseSequence(String filename) {
        try {
            // reads the file
            HashMap<String, List<String>> prereqMap = readPrerequisites(filename);

            // creates vertices from file
            HashMap<String, Vertex> courseVertices = new HashMap<>();
            for (String course : prereqMap.keySet()) {
                courseVertices.put(course, new Vertex(course));
                for (String prereq : prereqMap.get(course)) {
                    if (!courseVertices.containsKey(prereq)) {
                        courseVertices.put(prereq, new Vertex(prereq));
                    }
                }
            }

            //creates the graph
            HashMap<Vertex, Set<Vertex>> graph = new HashMap<>();
            for (String course : prereqMap.keySet()) {
                Vertex Vertex = courseVertices.get(course);
                for (String prereq : prereqMap.get(course)) {
                    Vertex prereqVertex = courseVertices.get(prereq);
                    addEdge(prereqVertex, Vertex, graph);
                }
            }

            // stores vertices in an array
            Vertex[] vertices = courseVertices.values().toArray(new Vertex[0]);

            //topological sort
            topologicalSortUsingDFS(graph, vertices);

            // sorts the vertices by their number
            Arrays.sort(vertices, new Comparator<Vertex>() {
                @Override
                public int compare(Vertex v1, Vertex v2) {
                    return v1.topNum - v2.topNum;
                }
            });

            System.out.println("Course sequence:");
            for (Vertex v : vertices) {
                System.out.println(v.name);
            }

        } catch (IOException e) {
            System.err.println("Error reading prerequisites file: " + e.getMessage());
        }
    }

    //buffered reader for file
    public static HashMap<String, List<String>> readPrerequisites(String filename) throws IOException {
        HashMap<String, List<String>> prereqMap = new HashMap<>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;

        // skips the header
        reader.readLine();

        while ((line = reader.readLine()) != null) {
            String[] parts = line.split("\t");
            if (parts.length >= 2) {
                String course = parts[0];
                List<String> prereqs = new ArrayList<>();

                if (parts[1].startsWith("\"") && parts[1].endsWith("\"")) {
                    // stores the courses in an array
                    String[] prereqArray = parts[1].substring(1, parts[1].length() - 1).split(",");
                    for (String prereq : prereqArray) {
                        prereqs.add(prereq);
                    }
                } else if (!parts[1].isEmpty()) {
                    // Single prerequisite
                    prereqs.add(parts[1]);
                }

                prereqMap.put(course, prereqs);
            }
        }
        reader.close();
        return prereqMap;
    }
    public static void main(String[] args) {
        String filename = "src/prerequisite.txt";
        if (args.length > 0) {
            filename = args[0];
        }

        try {
            HashMap<String, List<String>> prereqMap = readPrerequisites(filename);

            System.out.println("\nCourse prerequisites:");
            for (Map.Entry<String, List<String>> entry : prereqMap.entrySet()) {
                System.out.print(entry.getKey() + " requires: ");
                if (entry.getValue().isEmpty()) {
                    System.out.println("(no prerequisites)");
                } else {
                    System.out.println(String.join(", ", entry.getValue()));
                }
            }

            System.out.println("\n------------------------------\n");

            printCourseSequence(filename);

        } catch (IOException e) {
            System.err.println("Error reading prerequisites file: " + e.getMessage());
            e.printStackTrace();
        }
    }
}