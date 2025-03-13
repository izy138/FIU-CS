import java.util.Iterator;
import java.util.LinkedList;

public class AnswerToQ4 {
    public static void removeValuesLargerThanMax(LinkedList<Integer> list, int max) {
        Iterator<Integer> iterator = list.iterator();
        // while loop to go through the list
        while (iterator.hasNext()) {
            int current = iterator.next();
            // if the value for current is greater than the max value input
            if (current > max) {
                iterator.remove(); //removes current
            }
        }
    }

    public static void main(String[] args) {
        LinkedList<Integer> list = new LinkedList();
        list.add(5);
        list.add(10);
        list.add(15);
        list.add(7);
        list.add(3);
        list.add(27);
        list.add(14);
        list.add(1);

        System.out.println("Linked list:");
        // use an iterator to go through the list
        Iterator<Integer> iterator = list.iterator();
        while (iterator.hasNext()) {
            System.out.print(iterator.next() + " -> ");
        }
        System.out.println("null");
//        System.out.println(list);


        int max = 10;
        removeValuesLargerThanMax(list, max);

        System.out.println("\nList after removing numbers greater than " + max + ":");
        Iterator<Integer> iterator2 = list.iterator();
        while (iterator2.hasNext()) {
            System.out.print(iterator2.next() + " -> ");
        }
        System.out.println("null");
//        System.out.println(list);


    }
}
