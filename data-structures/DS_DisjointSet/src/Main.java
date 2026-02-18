import java.util.*;

public class Main {
    public static void main(String[] args){
        Scanner keyboard = new Scanner(System.in);
        DisjointSet<String> friendsList = new DisjointSet<>();
        while(true){//Phase I
            System.out.println("Phase I: Enter friendship info like \"Alice and Bob are now friends.\"." +
                    "Enter \"DONE\" to end Phase I.");
            String line = keyboard.nextLine();
            if(line.equalsIgnoreCase("DONE"))
                break;
            String[] words = line.split(" ");
            String userA = words[0], userB = words[2];
            //Use DisjointSet.union method to store the friendship between userA and userB
            friendsList.union(userA, userB);
        }
        System.out.println("Friends List: " + friendsList);

        while(true){//Phase II
            System.out.println("Phase II: Ask a question like \"Are Alice and Bob friends?\"." +
                    " Enter \"QUIT\" to end phase II");
            String line = keyboard.nextLine();
            if(line.equalsIgnoreCase("QUIT"))
                break;
            String[] words = line.split(" ");
            String userA = words[1], userB = words[3];
            //Use DisjointSet.find method to check the friendship between userA and userB
            //Print "Yes" or "No" to respond
            if(friendsList.find(userA).equalsIgnoreCase(friendsList.find(userB))){
                System.out.println("Yes");
            }
            else{
                System.out.println("No");
            }
        }
    }
}