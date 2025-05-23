
public class Main {
    public static double findMedianSortedArrays(int[] list1, int[] list2) {
        //both arrays must be of the same length for this method.
        if (list1.length != list2.length) {
            throw new IllegalArgumentException("Arrays must be of equal size");
        }
        //the length of the array set to length to check edge cases
        int length = list1.length;
        //edge cases for 1 element arrays will be the average of those elements
        if (length == 1){
            return (list1[0] + list2[0]) / 2.0;
        }
        //empty array will throw exception
        if (length == 0) {
            throw new IllegalArgumentException("Arrays cannot be empty");
        }
        //sets the start to index 0 and end to the length of the array.
        int start = 0;
        int end = length;

        while(start <= end){
            //the left partition of array 1 is set to half its length
            int partition1 = (start + end)/2;
            //the left partition of array 2 is set to the length - the left partition of array 1
            int partition2 = end - partition1;

            //array 1
            // the maximum of the left partition is set to the element at partitionL1-1
            int list1maxL = (partition1 == 0) ? Integer.MIN_VALUE : list1[partition1 - 1];
            //the minimum of the right partition is set to the element at partitionL1
            int list1minR = (partition1 == end) ? Integer.MAX_VALUE : list1[partition1];

            //arry 2
            // the maximum of the left partition is set to the element at partition2-1
            int list2maxL = (partition2 == 0) ? Integer.MIN_VALUE : list1[partition1 - 1];
            //the minimum of the right partition is set to the element at partition2
            int list2minR = (partition2 == end) ? Integer.MAX_VALUE : list1[partition2];

            //if the maximum of the left in array 1 is less than then minimum of the right in array 2
            // AND if the maximum of the left in array 2 is less than the minimum of the right in array 1
            //this is to make sure all the elements to the left of the partition are less than then
            // the elements of the right partition

            if(list1maxL <= list2minR && list2maxL <= list1minR){
                //finds the element by taking the maximum of the max of array 1 and 2
                int maxLeft = Math.max(list1maxL, list2maxL);
                //find the minimum by taking the min of the array 1 and 2
                int minRight = Math.min(list1minR, list2minR);

                //returns the median
                return (maxLeft + minRight) / 2.0;


            } else if (list1maxL > list2minR) {
                // if you have:
                // array 1: {8,9 | 10,11}
                // array 2: {3,4 | 7,8}
                // 9 is NOT less than 7 so this will make our median INCORRECT.
                // to fix this we must make the move length one less to change the partition and continue the while loop.
                end = partition1 - 1;

            }else {
                //moves the start to one more to change the partition and continue the while loop
                // in the case where the elements of the left partition are not less than the right partition of the opposite array.
                start = partition1 + 1;
            }
        }
        return -1;
    }


    public static void main(String[] args) {

        int[] array1 = {1,3,5,7};
        int[] array2 = {2,4,6,8};

        double median1 = findMedianSortedArrays(array1,array2);
        System.out.println(median1);

        int[] array3 = {1,2,3,4};
        int[] array4 = {5,6,7,8};

        double median2 = findMedianSortedArrays(array3,array4);
        System.out.println(median2);

        int[] array5 = {3,5,8,9};
        int[] array6 = {2,6,10,12};

        double median3 = findMedianSortedArrays(array5,array6);
        System.out.println(median3);

    }
}
