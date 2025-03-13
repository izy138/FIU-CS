import java.util.*;
import java.io.*;


class Account implements Comparable<Account>{
    String name;
    int balance;
    Account(String name, int balance){
        this.name = name;
        this.balance = balance;
    }
    @Override
    public int compareTo(Account account) {
        if(balance == account.balance)
            return name.compareTo(account.name);
        return balance - account.balance;
    }
}


public class Main {
    public static void main(String[] args) throws Exception {
        Scanner scn = new Scanner(new File("src/input.csv"));
        HashMap<String, Integer> accounts = new HashMap<>();

        // splits the lines by ','
        // assigns borrower to tokens[0], and lender to tokens[1]
        // amount to send it set to tokens[2]
        while (scn.hasNextLine()) {
            String[] tokens = scn.nextLine().split(",");
            String borrower = tokens[0];
            String lender = tokens[1];
            int amount = Integer.parseInt(tokens[2]);
            // sets the value of the borrow and lender by the amount specified
            // the borrower will be in the negative since they are giving money back
            // the lender will be in the positive since that are receiving money.
            accounts.put(borrower, accounts.getOrDefault(borrower, 0) - amount);
            accounts.put(lender, accounts.getOrDefault(lender, 0) + amount);
        }
        //treeset is for natural ordering of elements
        // object must implement Comparable interface
        TreeSet<Account> accountsBST = new TreeSet<>();

        // adds each account and its value to the tree set
        // the tree set will automatically order the accounts in order by its value:
        // smallest number first and largest number last
        // (Alice, -50)
        // (Bob, -25)
        // (Ann, 75)
        for (String name : accounts.keySet()) {
            if (accounts.get(name) == 0)
                continue;
            accountsBST.add(new Account(name, accounts.get(name)));
        }

        // set file to output results to
        PrintWriter p = new PrintWriter("src/output.csv");

        // while the tree set has accounts in it:
        while (!accountsBST.isEmpty()) {
            // set the borrower to the first element of the treeset,
            // the first element is the borrower who owes the most money
            // set the lender to the last element of the treeset.
            Account borrower = accountsBST.getFirst();
            Account lender = accountsBST.getLast();

            // Alice, -50 and Ann, 75 are removed from Tree Set
            accountsBST.remove(borrower);
            accountsBST.remove(lender);

            // the total money transferred will be the lowest amount between the two
            // the borrowers balance will be a negative number, so - is added to it to make it positive and then Math.min
            int total = Math.min(-borrower.balance, lender.balance);
            p.println(borrower.name + "," + lender.name + "," + total); // in the Alice to Ann transaction the total will be 50

            // Alice repays Ann 50 dollars, so her balance is no longer -50, it will 0.
            borrower.balance += total; // sets the borrowers balance to reflect payment
            // the lender has been repayed 50, so now 50 is taken off their balance.
            //in this case Ann is owed 75 dollars, 75-50 = 25 dollars left to be repayed.
            lender.balance -= total;

            // adds the account back if the balance is not 0
            // in this case Ann has a balance of 25, so that account is readded to the tree set.
            // the process will be done again in order for Bob (still in treeset) to repay the last 25.
            if(lender.balance != 0){
                accountsBST.add(lender);
            }
            // in this case Alice has a balance of 0 and is not readded to treeset.
            if (borrower.balance != 0){
                accountsBST.add(borrower);
            }
        }
        p.close();
    }
}
// ALICE ---> BOB = $20
// ALICE ---> ANN = $30
// BOB -----> ANN = $45

// Alice is negative 50 dollars
// ANN is positive 75 dollars

// BOB receives 20 dollars
// BOB sends 45 dollars
// BOB is negative 25 dollars

// ALICE - $50 to ANN
// BOB - $25 to ANN
