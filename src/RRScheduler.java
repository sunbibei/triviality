import java.util.*;

/**
 * Round Robin Scheduler
 * 
 * @version 2017
 */
public class RRScheduler extends AbstractScheduler {

  // TODO
  private int time_quantum_        = 0;
  private List<Process> pcb_queue_ = null;
  /**
   * Initializes the scheduler from the given parameters
   */
  public void initialize(Properties parameters) {
    try {
      time_quantum_ =
          Integer.parseInt(parameters.getProperty("timeQuantum", "20"));
    } catch(NumberFormatException e) {
      System.err.println("timeQuantum not a number.");
      System.exit(1);
    }
    
    pcb_queue_ = new ArrayList<>();
  }
  /**
   * Adds a process to the ready queue.
   * usedFullTimeQuantum is true if process is being moved to ready
   * after having fully used its time quantum.
   */
  public void ready(Process process, boolean usedFullTimeQuantum) {

    // TODO
    pcb_queue_.add(process);
  }

  /**
   * Removes the next process to be run from the ready queue 
   * and returns it. 
   * Returns null if there is no process to run.
   */
  public Process schedule() {

    if (pcb_queue_.isEmpty()) return null;
    
    Process p = pcb_queue_.get(0);
    pcb_queue_.remove(0);
    return p;
  }
  
  /**
   * Returns the time quantum of this scheduler
   * or -1 if the scheduler does not require a timer interrupt.
   */
  public int getTimeQuantum() {
    return time_quantum_;
  }
}
