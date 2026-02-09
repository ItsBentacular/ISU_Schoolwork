/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;
/**
 * Class for the AND node of logic that extends logicNode, uses the evaluate method to check whether both the left node and the right node equal 1, if so, it returns 1.
 */
public class AndNode extends LogicNode {
	
	/**
	 * takes a left and right node, if both nodes equal 1, return 1, else return 0.
	 * @return 1 if both nodes equal 1, else return 0.
	 */
	@Override
	public boolean evaluate() {
		if(left.evaluate() && right.evaluate()) {
			return true;
		}
		return false;
	}
	
	@Override
	public String toString() {
		return "&";
		
	}
}
