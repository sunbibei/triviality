import java.util.*;
import java.util.Properties;

/**
 * Ideal Shortest Job First Scheduler
 * 
 * @version 2017
 */
public class IdealSJFScheduler extends AbstractScheduler {

  // TODO
  private List<Process> pcb_queue_;
  
  public IdealSJFScheduler() {
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
    int burst = Integer.MAX_VALUE;
  	for (int idx = 0; idx < pcb_queue_.size(); ++idx) {
  		BurstProcess _p = (BurstProcess) pcb_queue_.get(idx);
  		if (_p.getNextBurst() < burst) {
  		  sjf   = idx;
  		  burst = _p.getNextBurst();
  		}
  	}
  	if (-1 == sjf) return null;
  	
  	Process ret = pcb_queue_.get(sjf);
  	pcb_queue_.remove(sjf);
    System.out.println("Scheduler selects process " + ret);
    return ret;
  }
}
