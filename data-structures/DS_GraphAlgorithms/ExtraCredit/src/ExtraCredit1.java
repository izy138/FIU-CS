import java.util.*;

public class ExtraCredit1 {
    public static HashMap<Vertex, HashSet<Vertex>> treeToGraph(BinaryNode<Vertex> root) {
        HashMap<Vertex, HashSet<Vertex>> graph = new HashMap<>();

        // base case: empty tree
        if (root == null) {
            return graph;
        }

        // a map that holds all nodes in the same level
        HashMap<Integer, List<BinaryNode<Vertex>>> levelMap = new HashMap<>();

        // uses a queue for traversal
        Queue<BinaryNode<Vertex>> queue = new LinkedList<>();
        //this tracks the respective level of each node
        Queue<Integer> levelQueue = new LinkedList<>();
        queue.add(root);
        levelQueue.add(0);
        // creates a graph and collects the nodes corresponding to their level
        while (!queue.isEmpty()) {
            BinaryNode<Vertex> current = queue.poll();
            int level = levelQueue.poll();
            Vertex currentVertex = current.element;

            //if current vertex does not exist
            if (!graph.containsKey(currentVertex)) {
                graph.put(currentVertex, new HashSet<>());
            }

            // adds to the map containing node levels
            if (!levelMap.containsKey(level)) {
                levelMap.put(level, new ArrayList<>());
            }
            levelMap.get(level).add(current);

            // left child
            if (current.left != null) {
                Vertex leftChild = current.left.element;
                graph.get(currentVertex).add(leftChild);
                queue.add(current.left);
                levelQueue.add(level + 1);
            }

            // right child
            if (current.right != null) {
                Vertex rightChild = current.right.element;
                graph.get(currentVertex).add(rightChild);
                queue.add(current.right);
                levelQueue.add(level + 1);
            }
        }

        // connects any nodes of the same level to a circular double linked list
        for (int level : levelMap.keySet()) {
            List<BinaryNode<Vertex>> nodesAtLevel = levelMap.get(level);

            // if the node is by itself in a level
            if (nodesAtLevel.size() <= 1) {
                continue;
            }

            // connect nodes to each other
            for (int i = 0; i < nodesAtLevel.size(); i++) {
                Vertex current = nodesAtLevel.get(i).element;

                // connects to the previous node
                Vertex prev = nodesAtLevel.get((i + nodesAtLevel.size() - 1) % nodesAtLevel.size()).element;
                graph.get(current).add(prev);

                // connects to the next node
                Vertex next = nodesAtLevel.get((i + 1) % nodesAtLevel.size()).element;
                graph.get(current).add(next);
            }
        }

        return graph;
    }

    public static void main(String[] args) {
        //       A
        //     /   \
        //    B     C
        //   / \   / \
        //  D   E F   G
        //     /
        //    H

        //creating tree above
        Vertex vA = new Vertex("A");
        Vertex vB = new Vertex("B");
        Vertex vC = new Vertex("C");
        Vertex vD = new Vertex("D");
        Vertex vE = new Vertex("E");
        Vertex vF = new Vertex("F");
        Vertex vG = new Vertex("G");
        Vertex vH = new Vertex("H");
        Vertex vI = new Vertex("I");

        BinaryNode<Vertex> nodeI= new BinaryNode<>(vI);
        BinaryNode<Vertex> nodeH = new BinaryNode<>(vH);
        BinaryNode<Vertex> nodeD = new BinaryNode<>(vD);
        BinaryNode<Vertex> nodeE = new BinaryNode<>(vE, nodeH, null);
        BinaryNode<Vertex> nodeF = new BinaryNode<>(vF);
        BinaryNode<Vertex> nodeG = new BinaryNode<>(vG, null, nodeI);
        BinaryNode<Vertex> nodeB = new BinaryNode<>(vB, nodeD, nodeE);
        BinaryNode<Vertex> nodeC = new BinaryNode<>(vC, nodeF, nodeG);
        BinaryNode<Vertex> root = new BinaryNode<>(vA, nodeB, nodeC);

        // prints the tree from root node
        System.out.println("Binary Tree:");
        System.out.println(root);

        //converts the tree to graph
        HashMap<Vertex, HashSet<Vertex>> graph = treeToGraph(root);
        System.out.println("\nTree to Graph:");
        printGraph(graph);

        System.out.println("\nCircular Double Linked List Connections");

        System.out.println("Level 1 (B and C):");
        System.out.print("B connects to: ");
        printLevelConnections(graph, vB, vA);

        System.out.print("C connects to: ");
        printLevelConnections(graph, vC, vA);

        System.out.println("\nLevel 2 (D, E, F, and G):");
        System.out.print("D connects to: ");
        printLevelConnections(graph, vD, vB);

        System.out.print("E connects to: ");
        printLevelConnections(graph, vE, vB);

        System.out.print("F connects to: ");
        printLevelConnections(graph, vF, vC);

        System.out.print("G connects to: ");
        printLevelConnections(graph, vG, vC);

        System.out.println("\nLevel 3 (H and I):");
        System.out.print("H connects to: ");
        printLevelConnections(graph, vH, vE);
        System.out.print("I connects to: ");
        printLevelConnections(graph, vI, vG);
    }

    private static void printGraph(HashMap<Vertex, HashSet<Vertex>> graph) {
        for (Map.Entry<Vertex, HashSet<Vertex>> entry : graph.entrySet()) {
            System.out.print(entry.getKey() + " -> ");
            if (entry.getValue().isEmpty()) {
                System.out.println("(no neighbors)");
            } else {
                boolean first = true;
                for (Vertex neighbor : entry.getValue()) {
                    if (!first) {
                        System.out.print(", ");
                    }
                    System.out.print(neighbor);
                    first = false;
                }
                System.out.println();
            }
        }
    }

    private static void printLevelConnections(HashMap<Vertex, HashSet<Vertex>> graph, Vertex vertex, Vertex parent) {
        boolean first = true;
        for (Vertex neighbor : graph.get(vertex)) {
            //connects node to each other from the same level
            if (!neighbor.equals(parent) && !hasParentChildRelation(graph, vertex, neighbor)) {
                if (!first) {
                    System.out.print(", ");
                }
                System.out.print(neighbor);
                first = false;
            }
        }
        System.out.println();
    }

    private static boolean hasParentChildRelation(HashMap<Vertex, HashSet<Vertex>> graph, Vertex parent, Vertex possibleChild) {
        //checks for parent-child
        if (graph.containsKey(parent)) {
            for (Vertex child : graph.get(parent)) {
                if (child.equals(possibleChild) && !graph.get(child).contains(parent)) {
                    return true;
                }
            }
        }
        return false;
    }

}
