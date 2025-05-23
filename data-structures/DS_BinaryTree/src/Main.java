import util.*;
public class Main {
    public static void main(String[] args) {
        
        //QUESTION 1
        BinaryNode<String> leftMostLeaf = new BinaryNode<>("Kelly");
        BinaryNode<String> midLeftLeaf = new BinaryNode<>("Amy");
        BinaryNode<String> midRightLeaf = new BinaryNode<>("Ben");
//        BinaryNode<String> rightMostLeaf = new BinaryNode<>("");

        BinaryNode<String> leftSubtree = new BinaryNode<>("Alice",leftMostLeaf,midLeftLeaf);
        BinaryNode<String> rightSubtree = new BinaryNode<>("Bob",midRightLeaf,null);

        BinaryNode<String> root = new BinaryNode<>("Ana",leftSubtree,rightSubtree);

        /*QUESTION 2
        The outputs are not in reverse order
        Preorder traversal starts at the root, then goes to the left subtree and its left and right leafs
        then goes to the right subtree and then its left and right leafs.

        Postorder traversal starts at the left subtree's left leaf then the right leaf, then goes to the left subtree.
        Next it goes to the right subtree's left leaf the right leaf, then goes to the right subtree.
        Lastly it goes to the root of the tree.
        */
        System.out.println("Pre-order Traversal");
        root.printPreOrder();
        System.out.println();

        System.out.println("\nPost-order Traversal");
        root.printPostOrder();
        System.out.println();



        //QUESTION 3
        BinaryNode<Double> leftLeft = new BinaryNode<>(4.0);
        BinaryNode<Double> leftRight = new BinaryNode<>(5.0);
        BinaryNode<Double> rightLeft = new BinaryNode<>(1.0);
//        BinaryNode<String> rightRight = new BinaryNode<>(null);

        BinaryNode<Double> leftSubtree1 = new BinaryNode<>(2.0,leftLeft,leftRight);
        BinaryNode<Double> rightSubtree1 = new BinaryNode<>(3.0,rightLeft,null);

        BinaryNode<Double> root1 = new BinaryNode<>(2.0,leftSubtree1,rightSubtree1);


        System.out.println(sum(root1));
          /*
        Recursive method with base case of root == null
        gets the roots data with root.element and call the sum method again on the roots left subtree
        root.left is checked if null, and then the sum method is called again on the left subtrees left leaf, then right leaf.
        it adds the data from these nodes as it goes down the tree and sums it to the root.element recursively.
        After the left subtree is done it goes to the right subtree and stops at the last leaf.
        Then all results are added.
     */
    }
    public static double sum(BinaryNode<Double> root){
        if(root == null){
            return 0;
        }
        return root.element + sum(root.left) + sum(root.right);
    }
}