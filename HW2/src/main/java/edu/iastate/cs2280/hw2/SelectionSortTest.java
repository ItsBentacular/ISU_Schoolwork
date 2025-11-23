/**
 * @author bfhurley
*/
package edu.iastate.cs2280.hw2;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.BeforeEach;

class SelectionSortTest {
	
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
     * checks if the GPA comparator sorts correctly.
     */
    void testSortByGpaOrder0() {
        SelectionSorter sorter = new SelectionSorter(students);
        sorter.setComparator(0);
        sorter.sort();

        Student[] expected = {s4, s1, s3, s2}; 
        // should give 4.0, 3.8(120), 3.8(90), 2.5
        assertArrayEquals(expected, sorter.students);
    }

    @Test
    /**
     * checks if the Credit Comparator sorts correctly.
     */
    void testSortByCreditsOrder1() {
        SelectionSorter sorter = new SelectionSorter(students);
        sorter.setComparator(1);
        sorter.sort();

        Student[] expected = {s3, s2, s4, s1}; 
        // this should give 90, 100, 110, 120
        assertArrayEquals(expected, sorter.students);
    }

    @Test
    /**
     * checks the two illegal arguments, one for it being null, and another for it being zero.
     */
    void testSortWithEmptyArray() {
        Student[] studentNull = null;
        Student[] studentZero = new Student[0];
        assertThrows(IllegalArgumentException.class, () -> new SelectionSorter(studentNull));
        assertThrows(IllegalArgumentException.class, () -> new SelectionSorter(studentZero));
    }

    @Test
    /**
     * checks to make sure the comparator has no errors when "sorting" a single element.
     */
    void testSortWithSingleElement() {
        Student[] single = {s1};
        SelectionSorter sorter = new SelectionSorter(single);
        sorter.setComparator(0);
        sorter.sort();
        assertArrayEquals(new Student[]{s1}, sorter.students);
    }
}