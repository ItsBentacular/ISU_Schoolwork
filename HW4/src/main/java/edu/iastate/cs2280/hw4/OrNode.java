/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;
/**
 * Class for the OR node of logic that extends logicNode, which uses the evaluate method to check if either the given left or right node equals 1, if either equals one, return 1.
 */
public class OrNode extends LogicNode {

	/**
	 * takes a left and right node, if EITHER of the nodes equal 1, return 1.
	 * @return 1 if either node is 1, else 0.
	 */
	@Override
	public boolean evaluate() {
		if(left.evaluate() || right.evaluate()) {
			return true;
		}
		return false;
	}
	
	@Override
	public String toString() {
		return "|";
		
	}

}
