/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;

public class AndNode extends LogicNode {
	
	@Override
	public boolean evaluate() {
		if(left.evaluate() && right.evaluate()) {
			return true;
		}
		return false;
	}

}
