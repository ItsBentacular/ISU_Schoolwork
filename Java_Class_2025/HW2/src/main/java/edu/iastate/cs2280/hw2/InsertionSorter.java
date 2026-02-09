/**
 * @author bfhurley
 */
package edu.iastate.cs2280.hw2;

public class InsertionSorter extends AbstractSorter {

	
	/**
	 * A constructor for the Insertion Sort class that holds a student array and 
	 * that sets the algorithm value in AbstractSorter to match this algorithm.
	 * 
	 * @param students a Student array that holds students gpa and credits.
	 * @throws IllegalArgumentException if the student array is incorrect
	 */
	protected InsertionSorter(Student[] students) throws IllegalArgumentException {
		super(students);
		this.algorithm = "InsertionSort";
	}

	@Override
	/**
	 * A basic insertion sort method that shifts the elements to sort into a set order.
	 * It loops over the length of the given students array and finds (depending on the comparator) 
	 * Either the GPA in descending order (credits descending tie-breaker) or Credits in ascending
	 * order (GPA descending tie-breaker).
	 */
	public void sort() {
		for(int i = 1; i < students.length; i++) {
			Student key = students[i];
			int j = i - 1;
			while( j >= 0 && studentComparator.compare(students[j], key) > 0) {
				students[j+1] = students[j];
				j = j - 1;
			}
			students[j+1] = key;
		}
		
	}

}
