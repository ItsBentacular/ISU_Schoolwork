/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;
/**
 * The main class for all of the logic nodes, gives a left and right node, and sets up the evaluate method.
 */
public abstract class LogicNode {
	/**
	 * the left logicNode.
	 */
	LogicNode left;
	/**
	 * the right logicNode
	 */
	LogicNode right;
	/**
	 * constructor for logicNode that sets both logicNodes to null so that the logic nodes can be manually set with dot notation (ae: {@code ParentNode.left = new AndNode()} ).
	 */
	public LogicNode() {
		this.left = null;
		this.right = null;
	}
	/**
	 * abstract method that evaluates the child nodes of the current node with logic and creates a boolean value from said logic.
	 * @return the logics' output.
	 */
	public abstract boolean evaluate();
	
	
}
