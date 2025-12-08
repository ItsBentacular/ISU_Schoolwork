/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;

public abstract class LogicNode {
	
	LogicNode left;
	LogicNode right;
	
	public LogicNode() {
		this.left = null;
		this.right = null;
	}
	
	public abstract boolean evaluate();
	
	
}
