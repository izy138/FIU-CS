package util;

public class SinglyLinkedList<T> {
    public Node<T> head;

    public SinglyLinkedList(Node<T> head) {
        this.head = head;
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();
        result.append("[");
        Node<T> temp = head;
        while (temp != null) {
            result.append(temp);
            if (temp.next != null)// haven't reached to the tail yet!
                result.append(" -> ");
            temp = temp.next;
        }
        result.append("]");
        return result.toString();
        /*
         * if(head == null)//base case
         * return "[]";
         * String result = "[" + head;
         * if(head.next != null)
         * result += ", ";
         * SinglyLinkedList rest = new SinglyLinkedList(head.next);
         * 
         * return result + rest.toString().substring(1);
         */
    }

    // also called enque
    public void append(T value) {
        Node<T> new_node = new Node<>(value, null);
        if (head == null) {
            head = new_node;
            return;
        }
        Node<T> temp = head;
        while (temp.next != null)
            temp = temp.next;
        temp.next = new_node;
    }

    public void prepent(T value) {
        head = new Node<>(value, head);
        // used for pushing an element
    }

    // also called dequeue
    public T removeFirst() {
        if (head == null)
            return null;
        T ft = head.data;
        head = head.next;
        return ft;
    }

    public int length(SinglyLinkedList list) {
        int counter = 0;
        for (Node<T> temp = head; temp != null; temp = temp.next) {
            counter++;
        }
        return counter;
    }

    public static void removeEvenIndices(SinglyLinkedList list) {

        System.out.println("Deleting node at even index 0: " + list.head);
        list.head = list.head.next; // Remove head (index 0)

        Node temp = list.head;
        int indexCounter = 0;
        while (temp != null && temp.next != null) {
            indexCounter += 2;
            System.out.println("Deleting node at even index " + indexCounter + ": " + temp.next);
            temp.next = temp.next.next;
            temp = temp.next;
        }
    }

    public static void whatDoesItDo(SinglyLinkedList list) {
        Node prev = list.head; // assigns prev as the head
        if (prev == null) // checks if first value is null
            return; // return if list is empty
        Node curr = prev.next, next;
        prev.next = null;// head becomes new tail
        while (curr != null) {
            next = curr.next;// save curr.next so that you don't lose the rest of nodes!
            curr.next = prev;// reverse the arrow direction connecting curr and prev nodes!
            prev = curr;// update prev
            curr = next;// update curr
        }
        list.head = prev; // reassigned list head
    }
}