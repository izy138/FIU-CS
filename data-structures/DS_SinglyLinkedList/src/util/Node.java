package util;


public class Node<T>{
    public T data;
    public Node next;
    @Override
    public String toString() {
        return data.toString();
    }
    public Node(T data, Node next) {
        this.data = data;
        this.next = next;
    }
    public Node(T data) {
        this.data = data;
    }
}