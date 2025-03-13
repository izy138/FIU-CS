//package util;
import java.util.*;
/*
This class implements Binary Node for Binary Trees

Methods:
    I) Two constructors
    II) Two toString methods
    III) height and size methods
    IV) preorder, postorder, and inorder traversals
    V) BFS and DFS traversal
 */

public class BinaryNode<E> {
    public E element;//data
    public BinaryNode<E> left;//left child
    public BinaryNode<E> right;//right child
    //constructor for leaves
    public BinaryNode(E element){
        this(element, null, null);
    }
    //constructor for internal nodes
    public BinaryNode(E element, BinaryNode<E> left, BinaryNode<E> right){
        this.left = left;
        this.right = right;
        this.element = element;
    }

    @Override
    public String toString(){
        if(left == null && right == null)//base case
            return element + "";
        return element + "(" + left + ", " + right + ")";
    }
    public int height() {
        if(left == null && right == null)
            return 0;
        if(left == null)
            return 1 + right.height();
        if(right == null)
            return 1 + left.height();
        return 1 + Math.max(left.height(), right.height());
    }
    public int size(){
        int size = 1;//counting root
        if(left != null)//counting left subtree nodes
            size += left.size();
        if(right != null)//counting right subtree nodes
            size += right.size();
        return size;
    }
    public void printPreOrder(){
        System.out.print(element + " ");
        if(left != null)
            left.printPreOrder();
        if(right != null)
            right.printPreOrder();
    }
    public void printPostOrder(){
        if(left != null)
            left.printPostOrder();
        if(right != null)
            right.printPostOrder();
        System.out.print(element + " ");
    }
    public void printInOrder(){
        if(left != null)
            left.printInOrder();
        System.out.print(element + " ");
        if(right != null)
            right.printInOrder();
    }
    public void printBFS(){//breadth first search traversal - non-recursive method
        Queue<BinaryNode> q = new LinkedList<>();
        q.add(this);
        while(!q.isEmpty()){
            BinaryNode<E> cur = q.remove();
            System.out.print(cur.element + " ");
            if(cur.left != null)
                q.add(cur.left);
            if(cur.right != null)
                q.add(cur.right);
        }
    }
    public void printDFS(){//depth first search traversal - equivalent to pre-order traversal
        Stack<BinaryNode> stack = new Stack<>();
        stack.add(this);
        while(!stack.empty()){
            BinaryNode<E> cur = stack.pop();
            System.out.print(cur.element + " ");
            if(cur.right != null)
                stack.push(cur.right);
            if(cur.left != null)
                stack.push(cur.left);
        }
    }
    public String toString2() {
        if(left == null && right == null && element == null)
            return "";
        Queue<BinaryNode> list = new LinkedList<>();
        String result = "";
        list.add(this);
        list.add(null);
        int level = (int) Math.pow(2, height());
        BinaryNode dummy = new BinaryNode(null);
        while(!list.isEmpty()) {
            boolean allDummies = true;
            for(BinaryNode<E> b: list)
                if(b != dummy && b != null) {
                    allDummies = false;
                    break;
                }
            BinaryNode<E> cur = list.remove();
            if(cur == null || allDummies)
                break;
            for(int i = 0; i < level - 1;i++)
                result += '\t';
            if(cur != dummy)
                result += cur.element;
            for(int i = 0; i < level + 1;i++)
                result += '\t';
            if(cur.left != null)
                list.add(cur.left);
            else
                list.add(dummy);
            if(cur.right != null)
                list.add(cur.right);
            else
                list.add(dummy);
            if(list.peek() == null) {
                for(int i = 0; i < height();i++)
                    result += '\n';
                list.remove();
                list.add(null);
                level/=2;
            }

        }
        return result + "\n";
    }
}