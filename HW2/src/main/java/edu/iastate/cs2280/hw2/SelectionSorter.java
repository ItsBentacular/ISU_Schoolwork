/**
 * @author bfhurley
 */
package edu.iastate.cs2280.hw2;

public class SelectionSorter extends AbstractSorter {

	
	/**
	 * A constructor for the Selection Sort class that holds a student array and 
	 * that sets the algorithm value in AbstractSorter to match this algorithm.
	 * 
	 * @param students a Student array that holds students gpa and credits.
	 * @throws IllegalArgumentException if the student array is incorrect.
	 */
	protected SelectionSorter(Student[] students) throws IllegalArgumentException {
		super(students);
		this.algorithm = "SelectionSort";
	}

	@Override
	/**
	 * A simple selection sort method that sorts based on the comparator values, 
	 * either GPA in descending order (credits descending tie-breaker) or Credits in ascending
	 * order (GPA descending tie-breaker).
	 * by using 2 for loops to compare one value to another, the min value is found and can be swapped with the current value
	 * until the array is sorted. 
	 */
	public void sort() {
 		for(int i = 0; i < students.length; i++) {
 			int min_index = i;
 			for(int j = i + 1; j < students.length; j++) {
 				if(studentComparator.compare(students[j],students[min_index]) < 0) {
 					min_index = j;
 				}
 			}
 			swap(i,min_index);
 		}
	}

}
