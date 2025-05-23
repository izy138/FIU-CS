import algorithms.ShortestPath;
import util.WeightedGraph;

public class RunDijkstra {
    public static void main(String[] args) {
        WeightedGraph graph = new WeightedGraph(
                new String[]{
                        "A", "B", "C", "D", "E", "F", "O", "T"
                },
                new String[]{
                        "A,B,2", "B,A,2",
                        "A,O,2", "O,A,2",
                        "A,D,7", "D,A,7",
                        "A,F,12", "F,A,12",
                        "O,B,5", "B,O,5",
                        "O,C,4", "C,O,4",
                        "B,C,1", "C,B,1",
                        "B,E,3", "E,B,3",
                        "B,D,4", "D,B,4",
                        "C,E,4", "E,C,4",
                        "D,E,4", "E,D,4",
                        "D,T,5", "T,D,5",
                        "E,T,7", "T,E,7",
                        "T,F,3", "F,T,3"
                }
        );

        try {
            ShortestPath.dijkstra(graph, "A");
            ShortestPath.printShortestPathSolution("A", graph);
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
