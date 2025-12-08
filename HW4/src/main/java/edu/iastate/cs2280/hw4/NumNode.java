/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;

/**
 * Node for a variable in the tree, no child nodes, extends LogicNode.
 */
public class NumNode extends LogicNode {
	
	/**
	 * variable for holding the state of the node (0 or 1), uses boolean 0 and 1 rather than an integer 0 and 1.
	 */
	private boolean state;
	/**
	 * the name of the given node that holds a state, can be as long as needed.
	 */
	private String stateName;

	/**
	 * constructor for NumNode, creates a stateName and initializes the state to false (0) by default.
	 * @param stateName the name of the given node.
	 */
	public NumNode(String stateName) {
		this.stateName = stateName;
		this.state = false;
	}
	/**
	 * LogicNode's evaluate method, returns the state, as these nodes only hold a value rather than an action.
	 * @return state returns the given state, 0 by default, can also be changed by updateState.
	 */
	@Override
	public boolean evaluate() {
		return state;
	}
	/**
	 * method that updates the state of the node to either 0 or 1 (boolean).
	 * @param newState The new state that state is getting updated to.
	 */
	public void updateState(boolean newState) {
		this.state = newState;
	}
	/**
	 * gets the state's name.
	 * @return returns the state's name.
	 */
	public String getName() {
		return stateName;
	}
	
	@Override
	public String toString() {
		return stateName;
		
	}

}
