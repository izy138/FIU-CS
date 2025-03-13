class Node {
    int data;
    Node next;
    public Node(int data) {
        this.data = data;
        this.next = null;
    }
}
class SinglyLinkedList {
    public Node head;

    public SinglyLinkedList() {
        this.head = null;
    }

    // add a node at the end of list
    public void add(int data) {
        Node newNode = new Node(data);
        if (head == null) {
            head = newNode; // if the list is empty, set the new node as the head
        } else {
            Node current = head;
            while (current.next != null) {
                current = current.next; // traverses through to the end of the list
            }
            current.next = newNode; // adds the new node to the end
        }
    }

    // method to print the list
    public void list() {
        Node current = head;

        while (current != null) {
            System.out.print(current.data + " -> ");
            current = current.next;
        }
        System.out.println("null"); // the end of the list is null
    }

    public static void removeValuesLargerThanMax(SinglyLinkedList list, int max) {
        Node current = list.head;
        Node previous = null;
        // iterates through the list and checks if the value > max and removes them
        while (current != null) {
            if (current.data > max) {
                // if the head needs to be removes
                if (previous == null) {
                    // sets the head to the next node which removes it.
                    list.head = current.next;
                } else {
                    // removes node
                    previous.next = current.next;
                }
            } else {
                // moves the previous pointer one node forward to the current
                previous = current;
            }
            // moves the current pointer forward to the next node
            current = current.next;
        }
    }
}
public class AnswerToQ3A {
    public static void main(String[] args) {
        SinglyLinkedList singlelist = new SinglyLinkedList();

        singlelist.add(5);
        singlelist.add(10);
        singlelist.add(15);
        singlelist.add(7);
        singlelist.add(3);
        singlelist.add(27);
        singlelist.add(14);
        singlelist.add(1);

        System.out.println("Linked list:");
        singlelist.list();

        int max = 10;
        System.out.println("\nList after removing numbers greater than " + max + ":");
        SinglyLinkedList.removeValuesLargerThanMax(singlelist, max);
        singlelist.list();
    }
}
