/**
 * @author bfhurley
 */
package edu.iastate.cs2280.hw2;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.BeforeEach;

public class MergeSortTest {
	
	private Student[] students;
    private Student s1, s2, s3, s4, s5, s6;

    @BeforeEach
    /**
     * a pre test setup that gives a bunch of useful student objects and puts them
     * into the students array.
     * one that has high gpa and high credits.
     * one that has low gpa and middle credits.
     * one that has high gpa and low credits.
     * one with the highest gpa.
     * onw with low gpa and high credits, tied with another low gpa.
     * and one with the high gpa and low credits, also tied with another low credit.
     */
    void setup() {
        s1 = new Student(3.8, 120);
        s2 = new Student(2.5, 100);
        s3 = new Student(3.8, 90);
        s4 = new Student(4.0, 110);
        s5 = new Student(2.5, 130);
        s6 = new Student(3.7, 90);
        students = new Student[]{s1, s2, s3, s4, s5, s6};
    }

    @Test
    /**
     * checks that the comparator works with gpa sorting (but checks the second condition because of duplicate gpa).
     */
    void testSortByGpaOrder0() {
        MergeSorter sorter = new MergeSorter(students);
        sorter.setComparator(0);
        sorter.sort();

        Student[] expected = {s4, s1, s3, s6, s5, s2};
        assertArrayEquals(expected, sorter.students);
    }

    @Test
    /**
     * checks that the comparator works with credit sorting(but checks the second condition because of duplicate credits)
     */
    void testSortByCreditsOrder1() {
        MergeSorter sorter = new MergeSorter(students);
        sorter.setComparator(1);
        sorter.sort();

        Student[] expected = {s3, s6, s2, s4, s1, s5};
        assertArrayEquals(expected, sorter.students);
    }
}
