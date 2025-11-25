import os
import openai

from openai import OpenAI
client = OpenAI(base_url = "http://localhost:1234/v1", api_key="lm-studio")

codeSnippet = """public void add(int pos, E item) {
	  if (item == null) {
		  throw new NullPointerException();
	  }
	  if(pos < 0 || pos > size()) {
		  throw new IndexOutOfBoundsException();
	  }
	  if(totalSize == 0) {
		 Page<E> page = new Page<E>(head,tail);
		 head.next = page;
		 tail.prev = page;
		 page.addItem(0, item);
		 IndexEntry<E> entry = new IndexEntry<>(page, 0, 1);
		 pageIndex.add(entry);
		 totalSize++;
		 modificationCount++;
		 return;
	  }
	  PageInfo<E> info = findPageForLogicalIndex(pos);
	  int offset = info.offset;
	  Page<E> page = info.page;
	  if(page.count < PAGE_CAPACITY) {
		  page.addItem(offset, item);
		  int indexInList = findIndexInPageIndex(page);
		  pageIndex.get(indexInList).count++;
		  updatePageIndex(indexInList + 1, 1);
		  totalSize++;
		  modificationCount++;
		  return;
	  }
	  Page<E> newPage = new Page<E>(page, page.next);
	  page.next = newPage;
	  newPage.next.prev = newPage;
	  for(int i = 0; i < HALF_CAPACITY; i++) {
		 E newItem = page.removeItem(HALF_CAPACITY);
		  newPage.addItem(i, newItem);
	  }
	  int indexInList = findIndexInPageIndex(page);
	  IndexEntry<E> oldEntry = pageIndex.get(indexInList);
	  oldEntry.count = HALF_CAPACITY;
	  
	  int pagePos = findIndexInPageIndex(page);
	  IndexEntry<E> newEntry = new IndexEntry<>(newPage,(oldEntry.logicalIndex + HALF_CAPACITY),HALF_CAPACITY);
	  pageIndex.add(indexInList + 1, newEntry);
	  
	  if(pos < newEntry.logicalIndex) {
		  page.addItem(pos - oldEntry.logicalIndex, item);
		  oldEntry.count++;
		  updatePageIndex(indexInList + 1,1);
	  } else {
		  newPage.addItem(pos - newEntry.logicalIndex, item);
		  newEntry.count++;
		  updatePageIndex(indexInList + 2,1);
	  }
	  totalSize++;
	  modificationCount++;"""


completion = client.chat.completions.create(
    model = "fireball-meta-llama-3.2-8b-instruct-agent-003-128k-code-dpo",
    messages =[
        {"role": "system", "content": "you are a friendly assistant named Merlin who helps review code and find any bugs and problems with optimization. you will ALWAYS look for easy errors first, and unoptimized code (UNOPTIMIZED MEANS LITERALLY SLOW AS IN O of N Notation). you will respond by starting with a summary of the code, then you will repeat their code back to them with highlights and comments on the code that can be fixed. it is **FINE** to say the code is good enough or doesn't have any problems, you don't have to be a perfecitonist. MAKE SURE TO FORMAT THE CODE CORRECTLY"},
        {"role": "user", "content": codeSnippet}
    ]
)
print(completion.choices[0].message.content)