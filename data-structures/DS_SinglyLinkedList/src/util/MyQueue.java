package util;

public class MyQueue<T> {
    private Node<T> tail;
    private Node<T> next;
    private Node<T> head;
    public boolean isEmpty(){
        return head == null;
    }
    public T peek(){
        return head.data;
    }
    public void add(T data) {
        Node node = new Node(data);
        if (tail != null) {
            tail.next = node;
        }
        tail = node;
        if (head == null)
            head = null;

    }
    public T remove(){
        T data = head.data;
        head = head.next;
        if (head == null)
            tail = null;
        return data;

    }


}
