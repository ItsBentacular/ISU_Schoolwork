/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;
/**
 * Class for the XOR node of logic which extends logicNode, which uses the evaluate method to return a 1 only when the given left and right node differ.
 */
public class XorNode extends LogicNode {

	/**
	 * takes a given left and right node, and if the nodes are different, returns 1.
	 * @return 1 if nodes differ, 0 if not.
	 */
	@Override
	public boolean evaluate() {
		if(left.evaluate() ^ right.evaluate()) {
			return true;
		}
		return false;
	}
	
	@Override
	public String toString() {
		return "^";
		
	}
	

}
