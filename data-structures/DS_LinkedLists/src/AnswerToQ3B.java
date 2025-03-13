public class AnswerToQ3B {
    public static int bobsLastPen(int numberOfPens, int k) {
        System.out.println("\nFor " + numberOfPens + " pens, losing one pen every " + k + " days");

        SinglyLinkedList pens = new SinglyLinkedList();
        for (int i = 1; i <= numberOfPens; i++) {
            pens.add(i);
        }
        // lists the linked list of pens
        pens.list();


        // make the list a circular linked list:
        // if temp.next is not null and that means there are still pens, temp will now move to temp.next.
        // once we reach the end of the list the temp.next will be null
        // if temp.next is null then the next node will be the pens.head, because we are starting over.
        // we have to circle back around to the head of the list and keep going until there is only one left.
        Node temp = pens.head;
        while (temp.next != null) {
            temp = temp.next; // temp continues going down the list
        }
        temp.next = pens.head; // the last node points back to head

        Node current = pens.head;
        Node previous = temp; // Make previous point to last node (circular structure)

        while (numberOfPens > 1) {
            // starting at first node 1
            // if k is 2 it will move 1 step forward so that [1 2 3 4 5] pen 2 is removed.
            // move k-1 steps forward remove the node
            for (int i = 0; i < k-1; i++) {
                previous = current;
                current = current.next;
            }
            // removes the node and move to the next node
            previous.next = current.next;
            current = current.next;
            // the number of pens is one less
            numberOfPens--;
        }
        System.out.println("\nLast remaining pen:");

        return current.data; // last remaining pen
    }
    public static void main(String[] args) {
        System.out.println(bobsLastPen(5, 1));
        System.out.println(bobsLastPen(5, 2));

    }
}