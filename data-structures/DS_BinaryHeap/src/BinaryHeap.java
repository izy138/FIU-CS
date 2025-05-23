public class BinaryHeap<T extends Comparable<? super T>>{
    private Comparable[] data;
    private int size = 0;

    public BinaryHeap(int capacity){
        data = new Comparable[capacity];
    }

    public BinaryHeap(Comparable[] data){
        this.data = data;
        size = data.length - 1;
        for(int i = size/2; i >= 1;i--)
            percolateDown(i);
    }

    public int size(){
        return size;
    }

    private void percolateDown(int root){//heapify
        int leftChild = root * 2;
        int rightChild = leftChild + 1;
        int min; //min of left, right, and parent nodes.

        //comparing parent with left child
        if(leftChild <= size && data[leftChild].compareTo(data[root]) < 0)
            min = leftChild;
        else
            min = root;

        //comparing the min with the right child
        if (rightChild <= size && data[rightChild].compareTo(data[min]) < 0)
            min = rightChild;

        if(min != root){ //if not the base case
            Comparable temp = data[root];//swapping min w/ root
            data[root] = data[min];
            data[min] = temp;
            percolateDown(min);
        }
    }

    public Comparable extractMin(){
        if(size == 0)
            throw new IllegalStateException();

        Comparable rv = data[1];
        data[1] = data[size];//swap root w/ last leaf
        data[size] = rv;
        size--;
        percolateDown(1);//heapify the root
        return rv;
    }

    public static BinaryHeap buildHeap(Comparable[] values){
        return new BinaryHeap(values);
    }

    public static void heapSort(Comparable[] values){
        BinaryHeap heap = buildHeap(values);
        for(int i = 0; i < values.length - 1;i++)
            heap.extractMin();
    }

    public static void main(String[] args){
        Comparable[] array = new Comparable[]{-1, 5, 6, 2, 3, 1, 8, 4, 0};
        heapSort(array);
        for(int i = 1; i < array.length;i++)
            System.out.println(array[i]);
    }
}