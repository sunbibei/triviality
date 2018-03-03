# Second-chance page fault algorithm
A .zip archive of given code has been provided on the previous page.  This contains three files:

- PageTableEntry.java - you do not need to change this.  It comprises a single page table entry: a frame address, a valid bit, and a reference bit.  As noted in the lecture, these are actually all stored in a single int variable - so do have a look at the code so you can see what this looks like.  What you do need to note are the methods, including the constructor - which takes a frame address, and sets the valid bit to true.
- AllocationTest.java - a test harness.  It accesses pages in a given sequence, then check which lead to page faults, comparing this to a known-correct baseline.  Use this to debug your code.
- MemoryManagementUnit.java - where your work is to go.

In MemoryManagementUnit, the accessMemoryAddress method takes the page number from the memory access request.  It then sees if this page table entry is valid; and if so, sets the reference bit to true.  It then returns false to indicate that no page fault occurred.  Conversely, if the page table entry was not valid, it runs the pageFaultHandler() method, and then returns true, to indicate a page fault occurred.  (This return value is just to allow the code to be tested from the main method in AllocationTest)

Your task is to implement the pageFaultHandler() method.  This takes the page number that is being accessed.  In the easy case,  the freeFrames list is non empty: you can take a frame address from there.  Otherwise, you will need to choose a victim from the page table, using the second-chance algorithm.  Once you have a victim, mark it as invalid; then create a new page table entry for the page just accessed, with its frame address, and valid bit set to true.

Hint: to choose a victim, loop over the page table, going back to the start once you get to the end.  Don't start this loop at 0 each time, or you'll usually only victimise the first few pages -- start it at the index of the last chosen victim. 

If necessary, to implement the second-chance algorithm, you may add other member variables to the MemoryManagementUnit class. (Note the index into the page table should not be reset to 0 each time a page fault occurs).  Do not change the other two Java files, though.
