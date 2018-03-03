import java.util.LinkedList;

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

    ///! The current top pointer
    private int topPointer;
    ///! The size of freeFrames
    private int sizeFreeFrames;

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
            pageTable[i] = new PageTableEntry();
        }
        
        freeFrames = new LinkedList<>();
        
        // assume there are half as many frames, as there are pages        
        for (int i = 0; i < (pageTableSize / 2); ++i) {
            freeFrames.addLast((2 << bitsForOffset) * i);
        }
        
        ///! initialize the top pointer
        topPointer = 0;
        sizeFreeFrames = freeFrames.size();
    }
    
    /** Access a memory address; returning true if there was a page fault */
    public boolean accessMemoryAddress(int address) {
        int offset = address & offsetMask;
        int pageNumber = (address & pageNumberMask) >> bitsForOffset;
        
        // System.out.println("");
        // System.out.println("");
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
        // System.out.println("==================================================");
        while (true) {
            int address = freeFrames.get(topPointer);
            int victim  = (address & pageNumberMask) >> bitsForOffset;
            victim >>= 1;
            // System.out.println("Before Location: " + topPointer + ", Address: " + freeFrames.get(topPointer) + " => " + victim);

            if (pageTable[victim].getValidBit() && pageTable[victim].getReferenceBit()) {
                pageTable[victim].setReferenceBit(false);
                topPointer = (topPointer + 1) % sizeFreeFrames;
                continue;
            }

            freeFrames.set(topPointer, ((2 << bitsForOffset) * pageNumber));
            // System.out.println("After  Location: " + topPointer + ", Address: " + freeFrames.get(topPointer) + " => " + pageNumber); 

            pageTable[victim].setValidBit(false);
            pageTable[victim].setReferenceBit(false);

            pageTable[pageNumber].setValidBit(true);
            pageTable[pageNumber].setReferenceBit(false);
            topPointer = (topPointer + 1) % sizeFreeFrames;

            // System.out.println("--------------------------------------------------");
            return;
        }
    }
}