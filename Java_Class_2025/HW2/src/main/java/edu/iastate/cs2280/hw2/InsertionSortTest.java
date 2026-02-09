/**
 * @author bfhurley
*/
package edu.iastate.cs2280.hw2;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class InsertionSortTest {
    
	private Student[] students;
    private Student s1, s2, s3, s4;

    @BeforeEach
    /**
     * a pre test setup that gives a bunch of useful student objects and puts them
     * into the students array.
     * one that has high gpa and high credits.
     * one that has low gpa and middle credits.
     * one that has high gpa and low credits.
     * and one with the highest gpa.
     */
    void setup() {
        s1 = new Student(3.8, 120);
        s2 = new Student(2.5, 100);
        s3 = new Student(3.8, 90);
        s4 = new Student(4.0, 110);
        students = new Student[]{s1, s2, s3, s4};
    }

    @Test
    /**
     *  tests the comparator to sort by GPA and checks the expected values.
     */
    void testSortByGpaOrder0() {
        InsertionSorter sorter = new InsertionSorter(students);
        sorter.setComparator(0); 
        // Sort by GPA descending
        sorter.sort();

        Student[] expected = {s4, s1, s3, s2};
        assertArrayEquals(expected, sorter.students);
    }

    @Test
    /**
     * tests the comparator to sort by credits and compares to excpected values.
     */
    void testSortByCreditsOrder1() {
        InsertionSorter sorter = new InsertionSorter(students);
        sorter.setComparator(1); 
        // Sort by Credits ascending
        sorter.sort();

        Student[] expected = {s3, s2, s4, s1};
        assertArrayEquals(expected, sorter.students);
    }

    @Test
    /**
     *  checks if the comparator sorts correctly on an already sorted array.
     */
    void testSortOnAlreadySortedArray() {
        Student[] sorted = {s4, s1, s3, s2};
        InsertionSorter sorter = new InsertionSorter(sorted);
        sorter.setComparator(0);
        sorter.sort();
        assertArrayEquals(sorted, sorter.students);
    }
}