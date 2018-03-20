import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

/**
 * Shortest Job First Scheduler
 * 
 * @version 2017
 */
public class SJFScheduler extends AbstractScheduler {

  // TODO
  private double tau_    = 0;
  private double alpha_  = 0;
  private List<Process> pcb_queue_ = null;
  /**
   * Initializes the scheduler from the given parameters
   */
  public void initialize(Properties parameters) {
    try {
      tau_   =
          Double.parseDouble(parameters.getProperty("initialBurstEstimate", "10"));
      alpha_ =
          Double.parseDouble(parameters.getProperty("alphaBurstEstimate", "10"));
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
    pcb_queue_.add(process);
  }

  /**
   * Removes the next process to be run from the ready queue 
   * and returns it. 
   * Returns null if there is no process to run.
   */
  public Process schedule() {
    int sjf   = -1;
    double burst = Double.MAX_VALUE;
    for (int idx = 0; idx < pcb_queue_.size(); ++idx) {
      BurstProcess _p = (BurstProcess) pcb_queue_.get(idx);
      tau_ = alpha_ * _p.getRecentBurst() + (1 - alpha_) * tau_;
      
      if (tau_ < burst) {
        sjf   = idx;
        burst = tau_;
      }
    }
    if (-1 == sjf) return null;
    
    Process ret = pcb_queue_.get(sjf);
    pcb_queue_.remove(sjf);
    System.out.println("Scheduler selects process " + ret);
    return ret;
  }
}
