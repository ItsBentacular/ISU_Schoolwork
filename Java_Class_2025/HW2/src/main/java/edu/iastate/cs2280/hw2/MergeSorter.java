/**
 * @author bfhurley
 */
package edu.iastate.cs2280.hw2;

public class MergeSorter extends AbstractSorter {
	
	/**
	 * A constructor for the Merge Sort class that holds a student array and 
	 * that sets the algorithm value in AbstractSorter to match this algorithm.
	 * 
	 * @param students a Student array that holds students gpa and credits.
	 * @throws IllegalArgumentException if the student array is incorrect.
	 */
	protected MergeSorter(Student[] students) throws IllegalArgumentException {
		super(students);
		this.algorithm = "MergeSort";
	}

	@Override
	/**
	 * The overarching sort algorithm that starts the sorting of the array.
	 * Using the mergeSortRec and merge methods, it splits the array as small as possible,
	 * sorts back up the "recursive tree", and merges into a final sorted array.
	 */
	public void sort() {
		mergeSortRec(0, students.length - 1);
	}
	
	/**
	 * the recursive part of the merge sort algorithm.
	 * base case checks if the first value is greater than the last value, if so, it breaks.
	 * otherwise, it finds the middle value then recursively breaks the array down into the
	 * smallest possible amounts. It finally calls merge to sort them. 
	 * 
	 * @param first the inital value in the array.
	 * @param last the final value in the array.
	 */
	private void mergeSortRec(int first, int last) {
		if(first >= last) return;
		int middle = first + (last - first) / 2;
		
		mergeSortRec(first, middle);
		mergeSortRec(middle + 1, last);
		merge(first, middle, last);
	}
	/**
	 * The complicated part of the merge sort algorithm.
	 *  the size value is created (either with array.length or by math like I did) then a temporary array is
	 *  created to hold the student values and are copied over with a for loop. Then using three values i,j, and k,
	 *  the temp array is used to replace values in the main array based on the comparator values, either 
	 *  the GPA in descending order (credits descending tie-breaker) or Credits in ascending order (GPA descending tie-breaker).
	 * 
	 * 
	 * @param first the inital value in the array.
	 * @param middle the middle value in the array (or as close as you can get within an even numbered array).
	 * @param last the final value in the array.
	 */
	private void merge(int first, int middle, int last) {
		
		int size = last - first + 1;
		
		Student[] temp = new Student[size];
		
		for(int i = 0; i < size; i++) {
			temp[i] = students[first + i];
		}
		int i = 0;
		int j = middle - first + 1;
		int k = first;
		
		while(i <= (middle - first) && j < size) {
			if(studentComparator.compare(temp[i],temp[j]) <= 0) {
				students[k] = temp[i];
				i++;
			} else {
				students[k] = temp[j];
				j++;
			}
			k++;
		}
		while(i <= (middle - first)) {
			students[k] = temp[i];
			i++;
			k++;
		}
		while(j < size) {
			students[k] = temp[j];
			j++;
			k++;
		}
		
	}

}
