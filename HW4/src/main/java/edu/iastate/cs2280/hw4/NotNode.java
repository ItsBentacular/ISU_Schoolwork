/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;

/**
 * Class for the NOT node of logic, different than the other nodes, as instead of taking two nodes and returning one, it just takes one node and flips the boolean.
 */
public class NotNode extends LogicNode {

	@Override
	public boolean evaluate() {
		if(left.evaluate()) {
			return false;
		}
		return true;
	}

}
