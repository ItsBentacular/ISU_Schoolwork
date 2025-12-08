/**
 * @author Bfhurley
 */
// Attempting the iterative approach for the constructor
package edu.iastate.cs2280.hw4;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Scanner;
import java.util.Stack;
/**
 * the main class for this HW, takes a circuit input and creates a circuit logic tree out of it, then evaluates tree and prints the result of all evaluated nodes, also prints a visual tree of the nodes.
 */
public class CircuitTree {
	/**
	 * the final (initial) node of the tree.
	 */
	private LogicNode root;
	/**
	 * Holds the values for all NumNodes so the evaluate function can access the state values.
	 */
	private HashMap<String, NumNode> variables;

	/**
	 * creates a stack to store the logic nodes and takes the given input and separates it. then based on the symbols in the input, 
	 * it creates a associated node (ae: & = AndNode), links the nodes above it on the stack to the given node,
	 * and pushes that finished node onto the stack. 
	 * Also takes NumNode and its associated name and puts it in to the variables HashMap for use during the evaluation method.
	 * @param circuitDescription the description of the tree in string format.
	 */
	public CircuitTree(String circuitDescription) {
		Stack<LogicNode> logicStack = new Stack<>();
		
		String[] input = circuitDescription.split(" ");
		
		variables = new HashMap<>();
		
		for(int i = input.length - 1; i >= 0; i--) {
			// switch case for the input, if the input is any of the symbols, create a node for said symbol and push onto stack, else it HAS to be a numNode, so create the numNode and push onto stack.
			switch(input[i]) {
			case "&":
				AndNode andNode = new AndNode();
				andNode.left = logicStack.pop();
				andNode.right = logicStack.pop();
				logicStack.push(andNode);
				break;
			case "|":
				OrNode orNode = new OrNode();
				orNode.left = logicStack.pop();
				orNode.right = logicStack.pop();
				logicStack.push(orNode);
				break;
			case "!":
				NotNode notNode = new NotNode();
				notNode.left = logicStack.pop();
				logicStack.push(notNode);
				break;
			case "^":
				XorNode xorNode = new XorNode();
				xorNode.left = logicStack.pop();
				xorNode.right = logicStack.pop();
				logicStack.push(xorNode);
				break;
			default:
				NumNode numNode = new NumNode(input[i]);
				logicStack.push(numNode);
				variables.put(numNode.getName(), numNode);
				break;
			}
		}
		if(!logicStack.isEmpty()) {
			this.root = logicStack.pop();
		}
		
	}
	/**
	 * evaluates the lines of text in a file with variable information on them, then updates the NumNodes with the given variable information to match the file, 
	 * then evaluates the root to find the final answer for the circuit.
	 * @param inputLine the input line of text that the method uses to set the variable information of the NumNodes.
	 */
	public void evaluate(String inputLine) {
		Scanner scan = new Scanner(inputLine);
		while(scan.hasNext()) {
			String symbol = scan.next();
			String[] parts = symbol.split("=");
			
			String name = parts[0];
			String value = parts[1];
			
			boolean state = value.equals("1");
			
			if(variables.containsKey(name)) {
				variables.get(name).updateState(state);
			}
		}
		scan.close();
		
		boolean result = root.evaluate();
		int resultInt = result ? 1 : 0;
		System.out.println(inputLine + " -> " + resultInt);
	}
	/**
	 * the main method for printing the circuit structure, simply calls the printHelper once.
	 */
	public void printStructure() {
		printHelper(root, " ", true);
	}
	/**
	 * a helper function for the printStructure method that recursively prints nodes.
	 * @param node the node being printed.
	 * @param prefix the string that goes before the node.
	 * @param rightChild boolean to check whether or not the node is a right child.
	 */
	private void printHelper(LogicNode node, String prefix, boolean rightChild) {
		String connector = "\\--";
		if(node == null) {
			return;
		}
		if(rightChild) {
			connector = "\\--";
			
		} else {
			connector = "|--";
		}
		
		System.out.println(prefix + connector + node.toString());
		
		String childPrefix;
		if(rightChild) {
			childPrefix = prefix + "   ";
		} else {
			childPrefix = prefix + "|  ";
		}
		printHelper(node.left, childPrefix, false);
		printHelper(node.right, childPrefix , true);
	}
	/**
	 * 
	 * @param args has a single argument that is the location of the text file, then creates the circuitTree based on said file.
	 * @throws FileNotFoundException 
	 */
	public static void main(String[] args) throws FileNotFoundException {
		File file = new File(args[0]);
		
		Scanner scan = new Scanner(file);
		
		CircuitTree circuit = new CircuitTree(scan.nextLine());
		
		System.out.println("CIRCUIT STRUCTURE: ");
		circuit.printStructure();
		System.out.println();
		
		System.out.println("EVALUATION: ");
		while(scan.hasNextLine()) {
			circuit.evaluate(scan.nextLine());
		}
		
		scan.close();
		
		
	}
	
	
	
}
