/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;

/**
 * Class for the NOT node of logic which extends logicNode, uses the evaluate method to take the left nodes value (and only the left), and flips said boolean value.
 */
public class NotNode extends LogicNode {

	/**
	 * takes the left node and flips the boolean value (1 becomes 0, 0 becomes 1)
	 * @return Returns the flipped value.
	 */
	@Override
	public boolean evaluate() {
		if(left.evaluate()) {
			return false;
		}
		return true;
	}
	
	@Override
	public String toString() {
		return "!";
		
	}

}
