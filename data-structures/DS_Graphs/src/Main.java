import java.util.*;
public class Main{
    public static class Vertex{
        public String name;
        public int indegree;
        @Override
        public int hashCode() {
            return name.hashCode();
        }
        @Override
        public boolean equals(Object o) {
            Vertex another = (Vertex) o;
            return this.name.equals(another.name);
        }
        @Override
        public String toString() {
            return name;
        }
        public Vertex(String name) {
            this.name = name;
            this.indegree = 0;
        }
    }
    private static void addEdge(Vertex from, Vertex to, Map<Vertex, Set<Vertex>> graph) {
        //Adding edge from--to
        if (!graph.containsKey(from))
            graph.put(from, new HashSet<Vertex>());
        if (!graph.containsKey(to))
            graph.put(to, new HashSet<Vertex>());
        graph.get(from).add(to);
        to.indegree++;
    }
    public static void dfs(Map<Vertex, Set<Vertex>> graph, Vertex src){
        System.out.print("Depth First Search Traversal: ");
        Stack<Vertex>stack = new Stack<>();
        Set<Vertex> visited = new HashSet<>();
        stack.push(src);
        while(true){
            while(!stack.isEmpty()){
                Vertex temp = stack.pop();
                if(!visited.contains(temp)) {
                    System.out.print(temp.name + " ");
                    visited.add(temp);
                    for (Vertex neighbor : graph.get(temp)) {
                        if (!visited.contains(neighbor))
                            stack.push(neighbor);
                    }
                }
            }
            boolean complete = true;
            for(Vertex v: graph.keySet())
                if(!visited.contains(v)){
                    complete = false;
                    stack.push(v);
                    break;
                }
            if(complete)
                break;
        }
        System.out.println();
    }
    public static void addEdges(Vertex[] vertex, String[] edges,
                                Map<Vertex,Set<Vertex>> graph, boolean directed){
        for(String edge: edges){
            int first = Integer.parseInt(edge.substring(0, edge.indexOf(',')));
            int second = Integer.parseInt(edge.substring(edge.indexOf(',')+1));
            addEdge(vertex[first], vertex[second], graph);
            if(!directed)
                addEdge(vertex[second], vertex[first], graph);
        }
    }
    public static void main(String[] args) {
        Map<Vertex, Set<Vertex>> undirectedGraph = new HashMap<>();
        Vertex[] vertex = new Vertex[7];
        for(int i = 0; i < 7;i++)
            vertex[i] = new Vertex(i + "");
        String[] edges = new String[]{
                "0,1", "0,3",
                "1,2", "1,3", "1,5", "1,6",
                "2,3", "2,4", "2,5",
                "3,4", "4,6"
        };
        addEdges(vertex, edges, undirectedGraph, false);//adding all edges
        dfs(undirectedGraph, vertex[0]);
    }
}