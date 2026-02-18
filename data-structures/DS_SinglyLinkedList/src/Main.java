import util.Node;
import util.SinglyLinkedList;
// import java.util.LinkedList;
// import java.util.Queue;

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
public class Main {
    public static void main(String[] args) {

        SinglyLinkedList<String> empty = new SinglyLinkedList<>(null);

        System.out.println("\nCreating Empty List:" + empty);
        empty.append("apple");
        empty.append("cherry");
        empty.append("strawberry");
        System.out.println(empty);

        SinglyLinkedList<Integer> list = new SinglyLinkedList<>(
                new Node<>(1, new Node<>(2, new Node<>(3))));
        System.out.println("\nList:" + list);

        // reverse the linked list
        SinglyLinkedList.whatDoesItDo(list);
        System.out.println("Reversed List: " + list);
        System.out.println();

        SinglyLinkedList<Integer> q1 = new SinglyLinkedList<>(
                new Node<>(1, new Node<>(5, new Node<>(6,
                        new Node<>(7, new Node<>(13, new Node<>(17)))))));
        System.out.println(q1);

        SinglyLinkedList.removeEvenIndices(q1);
        System.out.println(q1);

    }
}