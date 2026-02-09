/**
 * @author bfhurley
 */
package edu.iastate.cs2280.hw2;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.Random;
import java.util.Scanner;

/**
 * This class serves as the main driver for the sorting algorithm comparison application.
 * It provides a prompt interface for users to generate or load student data,
 * run four different sorting algorithms (Selection, Insertion, Merge, Quick),
 * and compare their performance based on execution time. The results can be
 * displayed in the console and optionally exported to a CSV file.
 */
public class CompareSorters {

  /**
   * The main entry point of the application. It presents a menu to the user to
   * choose between generating random student data, reading data from a file, or exiting.
   * For each set of data, it runs the sorting algorithms and prints their performance statistics.
   * Should handle InputMismatchExceptions and FileNotFoundExceptions gracefully.
   *
   * @param args Command-line arguments (not used).
   */
  public static void main(String[] args) {
	  System.out.println("Sorting Algorithms Performance Analysis using Student Data\n");
	    Scanner scan = new Scanner(System.in);
	    int trialCount = 1;

	    while (true) {
	      System.out.println("keys:  1 (random student data)  2 (file input)  3 (exit)");
	      System.out.print("Trial " + trialCount + ": ");

	      int choice;
	      try {
	        String inputLine = scan.nextLine();
	        choice = Integer.parseInt(inputLine);
	      } catch (NumberFormatException e) {
	        System.out.println("Invalid choice. Please enter 1, 2, or 3.");
	        trialCount++;
	        continue;
	      }

	      Student[] students = null;

	      switch (choice) {
	      
	        case 1: // Random student data
	          System.out.print("Enter number of random students: ");
	          try {
	            int numStudents = Integer.parseInt(scan.nextLine());
	            if (numStudents < 1) {
	              System.out.println("Number of students must be at least 1.");
	            } else {
	              students = generateRandomStudents(numStudents, new Random());
	            }
	          } catch (NumberFormatException e) {
	            System.out.println("Invalid number. Please enter an integer.");
	          }
	          break;

	        case 2: // File input data
	          System.out.print("File name: ");
	          String filename = scan.nextLine();
	          try {
	            students = readStudentsFromFile(filename);
	          } catch (FileNotFoundException e) {
	            System.out.println("Error: File not found: " + filename);
	          } catch (InputMismatchException e) {
	            System.out.println("Error: Input file format is incorrect. " + e.getMessage());
	          }
	          break;

	        case 3: // Exit
	          System.out.println("Exiting program.");
	          scan.close();
	          return;

	        default:
	          System.out.println("Invalid choice. Please enter 1, 2, or 3.");
	          break;
	      }
	      
	      
	      
	      if (students != null) {
	        StudentScanner[] scanners = new StudentScanner[4];
	        scanners[0] = new StudentScanner(students, Algorithm.SelectionSort);
	        scanners[1] = new StudentScanner(students, Algorithm.InsertionSort);
	        scanners[2] = new StudentScanner(students, Algorithm.MergeSort);
	        scanners[3] = new StudentScanner(students, Algorithm.QuickSort);

	        System.out.println("\nalgorithm       size  time (ns)");
	        System.out.println("--------------------------------------------------");
	        for (StudentScanner s : scanners) {
	          s.scan(); // Run the two-pass sort and timing
	          System.out.println(s.stats());
	        }
	        System.out.println("--------------------------------------------------");
	        System.out.println("Median Student Profile: " + scanners[0].getMedianStudent().toString());
	        System.out.println();
	        handleExportOption(scan, scanners);
	      }
	      trialCount++;
	      System.out.println();
	    }
	  }
 

  /**
   * Handles the user prompt for exporting sorting performance results to a CSV file.
   * This method catches and handles a {@link FileNotFoundException} if the specified
   * output file cannot be created or written to, printing an error message to the console.
   *
   * @param scan     The Scanner object to read user input.
   * @param scanners An array of StudentScanner objects containing the performance stats.
   */
  private static void handleExportOption(Scanner scan, StudentScanner[] scanners) {
	  System.out.print("Export results to CSV? (y/n): ");
	    String response = scan.nextLine().trim();

	    if (response.equalsIgnoreCase("y")) {
	        System.out.print("Enter filename for export (e.g., results.csv): ");
	        String filename = scan.nextLine().trim();

	        try (PrintWriter writer = new PrintWriter(filename)) {
	            writer.println("algorithm,size,time (ns)");

	            for (StudentScanner scanner : scanners) {
	                String consoleOutput = scanner.stats();
	                String csvLine = consoleOutput.trim().replaceAll("\\s+", ",");
	                writer.println(csvLine);
	            }

	            System.out.println("Data exported successfully to " + filename);

	        } catch (FileNotFoundException e) {
	            System.out.println("Error: Cannot write to file " + filename);
          }
      }
  }

  /**
   * Generates an array of Student objects with random GPA and credit values.
   *
   * @param numStudents The number of random students to generate.
   * @param rand        The Random object to use for generating values.
   * @return An array of randomly generated Student objects.
   * @throws IllegalArgumentException if numStudents is less than 1.
   */
  public static Student[] generateRandomStudents(int numStudents, Random rand) {
	  Student[] students = new Student[numStudents];

      for (int i = 0; i < numStudents; i++) {
          double gpa = rand.nextDouble() * 4.0;
          int credits = rand.nextInt(151);

          students[i] = new Student(gpa, credits);
      }

      return students;
  }

  /**
   * Reads student data from a text file. Each line of the file should contain a
   * GPA (double) followed by credits taken (int), separated by whitespace.
   *
   * @param filename The name of the file to read from.
   * @return An array of Student objects created from the file data.
   * @throws FileNotFoundException    if the specified file does not exist.
   * @throws InputMismatchException   if the file content is not in the expected format
   * (e.g., non-numeric data, empty file).
   */
  private static Student[] readStudentsFromFile(String filename) throws FileNotFoundException, InputMismatchException {
	  ArrayList<Student> studentList = new ArrayList<>();
      
      File file = new File(filename);

      try (Scanner fileScanner = new Scanner(file)) {
          while (fileScanner.hasNextLine()) {
              String line = fileScanner.nextLine().trim();
              if (line.isEmpty()) {
                  continue;
              }

              try (Scanner lineScanner = new Scanner(line)) {
                  if (!lineScanner.hasNextDouble()) {
                      throw new InputMismatchException("File format error: Invalid GPA format. Expected a double.");
                  }
                  double gpa = lineScanner.nextDouble();

                  if (!lineScanner.hasNextInt()) {
                      throw new InputMismatchException("File format error: Invalid credits format. Expected an integer.");
                  }
                  int credits = lineScanner.nextInt();
                  studentList.add(new Student(gpa, credits));
              }
          }
      }
      if (studentList.isEmpty()) {
          throw new InputMismatchException("File is empty or contains no valid student data.");
      }
      return studentList.toArray(new Student[0]);
  }
}
