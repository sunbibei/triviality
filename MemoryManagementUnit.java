import java.util.LinkedList;
import java.util.ListIterator;

public class MemoryManagementUnit {
    /** The page table */
    private PageTableEntry[] pageTable;
    
    /** How many bits of logical memory addresses are used for the offset */
    private int bitsForOffset;
    
    /** A bitmask, where the bits for the offset are set to 1; and everything else is 0 */
    private int offsetMask;
    
    /** A bitmask, where the bits for the page number are set to 1; and everything else is 0 */
    private int pageNumberMask;
    
    /** The free frames list */
    private LinkedList<Integer> freeFrames;

    /** The node for the oldest page */
    private ListIterator<Integer> oldest;

    /** Create an MMU
     *
     * @param pageTableSize  The number of entries in the page table
     * @param bitsForOffset  How many bits of logical memory addresses are used for the offset.  The rest are assumed to be the page number.
     */
    public MemoryManagementUnit(int pageTableSize, int bitsForOffset) {
        this.bitsForOffset = bitsForOffset;
        
        offsetMask = 0;
        for (int o = 0; o < bitsForOffset; ++o) {
            offsetMask = offsetMask | (1 << o);
        }
        
        pageNumberMask = ~offsetMask;
        
        
        pageTable = new PageTableEntry[pageTableSize];
        
        // Initialise all the page table entries to invalid, by making default page table entries
        for (int i = 0; i < pageTableSize; ++i) {
            pageTable[i] = new PageTableEntry((2 << bitsForOffset) * i);
            pageTable[i].setValidBit(false);
        }
        
        freeFrames = new LinkedList<>();
        
        // assume there are half as many frames, as there are pages        
        for (int i = 0; i < (pageTableSize / 2); ++i) {
            freeFrames.addLast((2 << bitsForOffset) * i);
        }
        
        // it initialize the interator
        oldest = freeFrames.listIterator(0);
    }
    
    /** Access a memory address; returning true if there was a page fault */
    public boolean accessMemoryAddress(int address) {
        int offset = address & offsetMask;
        int pageNumber = (address & pageNumberMask) >> bitsForOffset;
        
        // System.out.println("Accessing memory with page " + pageNumber + ", offset " + offset);
        
        if (pageTable[pageNumber].getValidBit()) {
            // no page fault, woot
            // set the reference bit            
            pageTable[pageNumber].setReferenceBit(true);            
            return false; // no page fault - return false
        } else {
            pageFaultHandler(pageNumber);
            return true; // page fault - return true
        }
    }
    
    /** An implementation of the second-chance page-fault handler algorithm
     *
     * Your task is to implement this
     *
     * @param pageNumber
     */
    private void pageFaultHandler(int pageNumber) {
        // TODO: your code goes here
        // Implement the second-chance page-fault handler algorithm
        // (Don't forget to check if there is a free frame)
        while (true) {
            // update the oldest.
            if (!oldest.hasNext())
                oldest  = freeFrames.listIterator(0);

            int addr  = oldest.next();
            int pageN = ((addr & pageNumberMask) >> bitsForOffset) / 2;
            if (!pageTable[pageN].getValidBit() || !pageTable[pageN].getReferenceBit()) {
                // clear the all of flag
                pageTable[pageN].setValidBit(false);
                pageTable[pageN].setReferenceBit(false);
                // set the flag
                oldest.set(pageTable[pageNumber].getAddress());
                pageTable[pageNumber].setValidBit(true);
                pageTable[pageNumber].setReferenceBit(false);

                return;
            }

            pageTable[pageN].setReferenceBit(false);
        }
    }
}
