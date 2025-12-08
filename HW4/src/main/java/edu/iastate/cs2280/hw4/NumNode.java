/**
 * @author Bfhurley
 */
package edu.iastate.cs2280.hw4;

/**
 * Node for the variable in the tree, no child nodes.
 */
public class NumNode extends LogicNode {
	
	/**
	 * variable for holding the state of the node (0 or 1), uses boolean 0 and 1 vs integer 0 and 1.
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
	
	public void updateState(boolean newState) {
		this.state = newState;
	}
	
	public String getName() {
		return stateName;
	}

}
