import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

/**
 * Feedback Round Robin Scheduler
 * 
 * @version 2017
 */
public class FeedbackRRScheduler extends AbstractScheduler {

  // TODO
  private int time_quantum_   = -1;
  private List<List<Process>> mul_lev_pcb_queues_;
  
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
    
    mul_lev_pcb_queues_ = new ArrayList<>();
    mul_lev_pcb_queues_.add(new ArrayList<>());
  }
  
  /**
   * Adds a process to the ready queue.
   * usedFullTimeQuantum is true if process is being moved to ready
   * after having fully used its time quantum.
   */
  public void ready(Process process, boolean usedFullTimeQuantum) {
    if (usedFullTimeQuantum) {
      int priority = process.getPriority();
      ++priority;
      process.setPriority(priority);
      
      while ( mul_lev_pcb_queues_.size() <= priority )
        mul_lev_pcb_queues_.add(new ArrayList<>());
      
      mul_lev_pcb_queues_.get(priority).add(process);
    } else {
      mul_lev_pcb_queues_.get(0).add(process);
    }
  }

  /**
   * Removes the next process to be run from the ready queue 
   * and returns it. 
   * Returns null if there is no process to run.
   */
  public Process schedule() {
    for (List<Process> queue : mul_lev_pcb_queues_) {
      if (queue.isEmpty())
        continue;
      
      Process p = queue.get(0);
      queue.remove(0);
      return p;
    }
    return null;
  }

  /**
   * Returns the time quantum of this scheduler
   * or -1 if the scheduler does not require a timer interrupt.
   */
  public int getTimeQuantum() {
    return time_quantum_;
  }

  /**
   * Returns whether the scheduler is preemptive
   */
  public boolean isPreemptive() {
    return true;
  }
}
