import java.util.*;
public class DisjointSet<T> {
    private int[] parent;
    private T[] elements;
    private int size;
    private HashMap<T, Integer> index;
    public DisjointSet(){
        this(100);
    }
    public DisjointSet(int capacity) {//constructor receives an integer specifying the maximum # of elements
        index = new HashMap<T, Integer>();
        int i = 0;
        parent = new int[capacity];
        elements = (T[]) new Object[capacity];
        size = 0;
        for(i = 0; i < parent.length;i++)
            parent[i] = -1;
    }
    public T find(T t) {
        int i = index.getOrDefault(t, -1);
        if(i == -1){
            index.put(t, size);
            elements[size++] = t;
            return t;
        }
        while(parent[i] >= 0)
            i = parent[i];
        return elements[i];
    }
    public void union(T u, T v) {
        int i = index.get(find(u));
        int j = index.get(find(v));
        if(i == j)//u & v are already in the same set
            return;
        if(parent[i] < parent[j]){//union by size: u is in a bigger disjoint set
            parent[i] += parent[j];//update the size
            parent[j] = i;//add v's set to u's set
        }
        else{//union by size: u is not in a bigger disjoint set
            parent[j] += parent[i];//update the size
            parent[i] = j;//add u's set to v's set
        }
    }
}