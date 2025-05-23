import java.util.*;

public class Question1 {

    static class Vertex {
        public String name;

        public Vertex(String name) {
            this.name = name;
        }

        @Override
        public int hashCode() {
            return name.hashCode();
        }

        @Override
        public boolean equals(Object o) {
            if (!(o instanceof Vertex)) return false;
            Vertex another = (Vertex) o;
            return this.name.equals(another.name);
        }

        @Override
        public String toString() {
            return name;
        }

        public static HashMap<Vertex, HashSet<Vertex>> treeToGraph(BinaryNode<Vertex> root) {
            HashMap<Vertex, HashSet<Vertex>> graph = new HashMap<>();

            // Base case is an empty tree
            if (root == null) {
                return graph;
            }

            // queue for level order traversals
            Queue<BinaryNode<Vertex>> queue = new LinkedList<>();
            queue.add(root);

            while (!queue.isEmpty()) {
                BinaryNode<Vertex> current = queue.poll();
                Vertex currentVertex = current.element;

                // if vertex does not exist
                if (!graph.containsKey(currentVertex)) {
                    graph.put(currentVertex, new HashSet<>());
                }

                //left children
                if (current.left != null) {
                    Vertex leftChild = current.left.element;
                    graph.get(currentVertex).add(leftChild);
                    queue.add(current.left);
                }

                //right children
                if (current.right != null) {
                    Vertex rightChild = current.right.element;
                    graph.get(currentVertex).add(rightChild);
                    queue.add(current.right);
                }
            }

            return graph;

        }
        //prints the graph by using getKey and getValue
        //format: Parent -> Children
        private static void printGraph(HashMap<Vertex, HashSet<Vertex>> graph) {
            for (Map.Entry<Vertex, HashSet<Vertex>> entry : graph.entrySet()) {
                System.out.print(entry.getKey() + " -> ");
                for (Vertex neighbor : entry.getValue()) {
                    System.out.print(neighbor + " ");
                }
                System.out.println();
            }
        }

        public static void main(String[] args) {
            //       A
            //     /   \
            //    B     C
            //   / \     \
            //  D   E     F

            Vertex vA = new Vertex("A");
            Vertex vB = new Vertex("B");
            Vertex vC = new Vertex("C");
            Vertex vD = new Vertex("D");
            Vertex vE = new Vertex("E");
            Vertex vF = new Vertex("F");

            BinaryNode<Vertex> nodeD = new BinaryNode<>(vD);
            BinaryNode<Vertex> nodeE = new BinaryNode<>(vE);
            BinaryNode<Vertex> nodeF = new BinaryNode<>(vF);
            BinaryNode<Vertex> nodeB = new BinaryNode<>(vB, nodeD, nodeE);
            BinaryNode<Vertex> nodeC = new BinaryNode<>(vC, null, nodeF);
            BinaryNode<Vertex> root = new BinaryNode<>(vA, nodeB, nodeC);

            System.out.println("\nBinary Tree:");
            System.out.println(root);
            HashMap<Vertex, HashSet<Vertex>> graph = treeToGraph(root);

            System.out.println("\nTree to Graph:");
            printGraph(graph);
        }
    }
}
