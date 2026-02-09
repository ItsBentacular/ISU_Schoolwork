/**
 * @author bfhurley
 */
package edu.iastate.cs2280.hw2;

/**
 * This class implements the quicksort algorithm for an array of Student objects.
 * It uses a median-of-three pivot selection strategy to improve performance and
 * avoid worst-case scenarios with already sorted or reverse-sorted data.
 */
public class QuickSorter extends AbstractSorter {

	
	/**
	 * A constructor for the Quick Sort class that holds a student array and 
	 * that sets the algorithm value in AbstractSorter to match this algorithm.
	 * 
	 * @param students a Student array that holds students gpa and credits.
	 * @throws IllegalArgumentException if the student array is incorrect.
	 */
  protected QuickSorter(Student[] students) throws IllegalArgumentException {
		super(students);
		this.algorithm = "QuickSort";
	}

  /**
   * Selects a pivot using the median-of-three strategy. It considers the first,
   * middle, and last elements of the subarray, sorts them, and uses the median
   * as the pivot. The pivot is swapped to the second-to-last position (last - 1)
   * to simplify the partition step.
   *
   * @param first The starting index of the subarray.
   * @param last  The ending index of the subarray.
   */
  private void medianOfThree(int first, int last) {
	  int middle = (first + last) / 2;
	  
	  if(studentComparator.compare(students[first], students[middle]) > 0) {
		 swap(first, middle); 
	  }
	  if(studentComparator.compare(students[first], students[last]) > 0) {
		  swap(first, last);
	  }
	  if(studentComparator.compare(students[middle], students[last]) > 0) {
		  swap(middle, last);
	  }
	  
	  swap(middle, last - 1);
  }

  @Override
  /**
   * the overarching method in the quicksort class.
   * Uses the quickSortRec method to recursively sort the student array similar to
   * the merge sort class, but using the median of three strategy to more efficiently sort
   * the array.
   */
  public void sort() {
	quickSortRec(0, students.length - 1);
  }
  
  /**
   * recursively sorts the students array.
   * base case checks if inital value is greater than the last value, if so, end.
   * if not, it finds the pivot based on the partition method (using median of three),
   * then recursively sorts the array, similar to the merge sort class.
   * 
   * @param first the initial value in the array
   * @param last the final value in the array
   */
  private void quickSortRec(int first, int last) {
	  if(first >= last) {
		  return;
	  }
	  
	  int pivotIndex = partition(first,last);
	  
	  quickSortRec(first, pivotIndex - 1);
	  quickSortRec(pivotIndex + 1, last);
  }
  
  /**
   *  Uses the median of three method to partition the array, then makes a Student pivot out of the last element in
   *  the students array. then going through the array, while i is greater than j, we check for the comparator values, either 
   *  GPA in descending order (credits descending tie-breaker) or Credits in ascending order (GPA descending tie-breaker).
   *  Finally the values are swapped and i is returned to the quickSortRec method for the pivot.
   * 
   * @param first the initial value in the array.
   * @param last the final value in the array.
   * @return the i value.
   */
  private int partition(int first, int last) {
	  int size = last - first + 1;
	  //trying to fix my errors, pretty sure my values are incorrect if the array is smlaller than 3, so im hoping a base case will fix it.
	  if(size == 1) {
		  return first;
	  }
	  else if(size == 2 && studentComparator.compare(students[first], students[last]) > 0) {
		  swap(first,last);
		  return first;
	  }
	  
	  //if the base cases don't apply, im pretty sure my main logic is still solid, I mean ive changed it like 4 times and it hasn't completely broken yet...
	  medianOfThree(first,last);
	  Student pivot = students[last - 1];
	  
	  int i = first;
	  int j = last - 1;
	  
	  while(true) {
		  
		  while(i < last -1 && studentComparator.compare(students[++i], pivot) < 0) {
		  }
		  while(j > first && studentComparator.compare(students[--j], pivot) > 0) {
		  }
		  
		  if(i >= j) {
			  break;
		  }
		  
		  swap(i,j);
	  }
	  
	  swap(i, last - 1);
	  
	  return i;
  }
  
}
