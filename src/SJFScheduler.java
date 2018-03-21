import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import java.util.Vector;

/**
 * Shortest Job First Scheduler
 * 
 * @version 2017
 */
public class SJFScheduler extends AbstractScheduler {
  
  private double tau_[] = null;
  private double alpha_ = 0;
  private List<Process> pcb_queue_ = null;
  /**
   * Initializes the scheduler from the given parameters
   */
  public void initialize(Properties parameters) {
    double _tau = 10;
    try {
      _tau   =
          Double.parseDouble(parameters.getProperty("initialBurstEstimate", "10"));
      alpha_ =
          Double.parseDouble(parameters.getProperty("alphaBurstEstimate", "10"));
    } catch(NumberFormatException e) {
      System.err.println("timeQuantum not a number.");
      System.exit(1);
    }
    
    pcb_queue_ = new ArrayList<>();
    tau_ = new double[1024]; ///! The default maximum number of process is 1024
    for (int i = 0; i < tau_.length; ++i)
      tau_[i] = _tau;
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
      double _tau = alpha_ * _p.getRecentBurst() + (1 - alpha_) * tau_[_p.getId()];
      
      if (_tau < burst) {
        sjf   = idx;
        burst = _tau;
      }
    }
    if (-1 == sjf) return null;
    
    BurstProcess ret = (BurstProcess) pcb_queue_.get(sjf);
    tau_[ret.getId()] = alpha_ * ret.getRecentBurst() + (1 - alpha_) * tau_[ret.getId()];
    pcb_queue_.remove(sjf);
    // System.out.println("Scheduler selects process " + ret);
    return ret;
  }
}
