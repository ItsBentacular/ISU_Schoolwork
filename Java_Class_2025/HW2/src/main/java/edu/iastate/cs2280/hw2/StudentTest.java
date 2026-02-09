/**
 * @author bfhurley
 */
package edu.iastate.cs2280.hw2;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class StudentTest {

    @Test
    /**
     * tests whether the compareTo method works with the GPA comparisons.
     */
    void testCompareToGPADominant() {
        Student highGPA = new Student(4.0, 100);
        Student lowGPA = new Student(3.5, 120);
        
        assertTrue(highGPA.compareTo(lowGPA) < 0);
        assertTrue(lowGPA.compareTo(highGPA) > 0);
    }

    @Test
    /**
     * tests whether the compareTo method works with the credit comparisons.
     */
    void testCompareToCreditsTieBreaker() {
        Student highCredits = new Student(3.8, 120);
        Student lowCredits = new Student(3.8, 100);
        
        assertTrue(highCredits.compareTo(lowCredits) < 0);
        assertTrue(lowCredits.compareTo(highCredits) > 0);
    }

    @Test
    /**
     * tests to make the the compareTo method works correctly in an equal scenario.
     */
    void testCompareToEqualStudents() {
        Student student1 = new Student(3.9, 90);
        Student student2 = new Student(3.9, 90);
        
        assertEquals(0, student1.compareTo(student2));
    }

    @Test
    /**
     * tests whether a copy of the student constructor works the same as the original, 
     * even though they are seperate entities.
     */
    void testCopyConstructor() {
        Student original = new Student(3.5, 80);
        Student copy = new Student(original);
        
        assertEquals(original.getGpa(), copy.getGpa());
        assertEquals(original.getCreditsTaken(), copy.getCreditsTaken());
        assertNotSame(original, copy);
    }

    @Test
    /**
     * Tests whether the getters and setters work within the student class.
     */
    void testGetters() {
        Student student = new Student(2.75, 55);
        
        assertEquals(2.75, student.getGpa());
        assertEquals(55, student.getCreditsTaken());
    }
}