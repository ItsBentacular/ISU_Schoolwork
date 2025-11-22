package edu.iastate.cs2280.hw3;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class AddMethodTest {

	
	@Test
	void testAddOnEmpty() {
		IndexedPagedList<String> test = new IndexedPagedList<>();
		IndexedPagedList<String> test2 = new IndexedPagedList<>();
		test.add(0, "A");
		test2.add("A");
		
		assertEquals(test,test2);
	}
	
	
	
	
}
