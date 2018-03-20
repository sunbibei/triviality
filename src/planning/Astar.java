package planning;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

import abc.*;

public class Astar {

	public class AstarCell extends Cell implements Comparable<AstarCell> {
		public int g_cost_ = 0;
		public int h_cost_ = 0;
		public int f_cost_ = 0;
		
		public AstarCell parent = null;
		
		final int MOVE_COST = 1;
		
//		public AstarCell(Point2D _p) {
//			super(_p);
//		}
//		
//		public AstarCell(Point2D _c, AstarCell _p) {
//			super(_c);
//			
//		}
//		
		public AstarCell(Cell _c) {
			super(_c);
			if (_c.isBlock()) f_cost_ = Integer.MAX_VALUE;
		}
		/**
		 * initialize the AstarCell
		 * @param _c     The location
		 * @param _p     The parent
		 * @param h_cost The H cost
		 */
		public AstarCell(Cell _c, AstarCell _p, int h_cost) {
			super(_c);
			parent = _p;
			
			if (_c.isBlock()) {
				f_cost_ = Integer.MAX_VALUE;
			} else {
				g_cost_ = _p.g_cost_ + MOVE_COST;
				h_cost_ = h_cost;
				f_cost_ = g_cost_ + h_cost_;
			}
		}
		
		public int f_cost() { return f_cost_; }
		
		public boolean equals(Point2D _p) {
			return super.equals(_p);
		}
		
		/**
		 * If this.f > _o.f, return -1;
		 * If this.f = _o,f, return  0;
		 * If this.f < _o.f, return +1;
		 */
		@Override
		public int compareTo(AstarCell _o) {
			if (f_cost_ == _o.f_cost_)
				return 0;
			else if (f_cost_ < _o.f_cost_)
				return 1;
			else
				return -1;
		}
	}
	
	private Point2D[] neighbors = null;
	List<AstarCell>   opens_    = null;
	List<AstarCell>   closes_   = null;
	
	public Astar() {
		neighbors = new Point2D[8];
		neighbors[0] = new Point2D(-1, -1);neighbors[1] = new Point2D(-1,  0);
		neighbors[2] = new Point2D(-1, +1);neighbors[3] = new Point2D(+1, -1);
		neighbors[4] = new Point2D(+1,  0);neighbors[5] = new Point2D(+1, +1);
		neighbors[6] = new Point2D( 0, -1);neighbors[7] = new Point2D( 0, +1);
		
		opens_  = new ArrayList<AstarCell>();
		closes_ = new ArrayList<AstarCell>();
	}
	
    private int heuristic(Point2D current, Point2D target) {
        return Math.abs(current.getX() - target.getX())
        		+ Math.abs(current.getY() - target.getY());
    }
    
    ///! find the minimum cost cell, return it and remove it from opens.
    private AstarCell getMinCostCell() {
    	if (opens_.isEmpty()) return null;
    	
    	int idx = 0;
    	AstarCell min = opens_.get(idx);
    	for (int i = 1; i < opens_.size(); ++i) {
    		AstarCell e = opens_.get(i);
    		// if min.f > e.f
    		if (min.compareTo(e) < 0) {
    			idx = i;
    			min = e;
    		}
    	}
    	opens_.remove(idx);
    	return min;
    }
	
	public Stack<Point2D> planning(BoxMap map, Point2D start, Point2D end) {
		opens_.clear();
		closes_.clear();
		opens_.add(new AstarCell(map.getCell(start)));

		AstarCell curr_cell = null;
		while (!opens_.isEmpty()) {
			// find the minimum f_cost object.
			curr_cell = getMinCostCell();
			if (null == curr_cell) return null;
			
			///! The only place to exit while, if have a path.
			if (curr_cell.equals(end)) {
				Stack<Point2D> path = new Stack<Point2D>();
				do {
					path.push(curr_cell);
					curr_cell = curr_cell.parent;
				} while (null != curr_cell);
				
				return path;
			}
			
			closes_.add(curr_cell);
			// for each neighbor
			for (Point2D _n : neighbors) {
				Cell _nbor = map.getCell(curr_cell.plus(_n));
				if (null == _nbor) continue;
				
				AstarCell nbor_cell = new AstarCell(_nbor, curr_cell, heuristic(_nbor, end));
				int index_open  = opens_.indexOf(_nbor);
				int index_close = closes_.indexOf(_nbor);
				///! not in opens and not in closes
				if ((-1 == index_open) && (-1 == index_close)) {
					opens_.add(nbor_cell);
				///! in opens
				} else if (-1 != index_open) { 
					// get the nbor_open cell from opens
					AstarCell nbor_open = opens_.get(index_open);
					// nbor_open.f > nbor_cell.f
					if (nbor_open.compareTo(nbor_cell) < 0) {
						nbor_open = new AstarCell(nbor_cell);
						// re-insert into opens
						opens_.set(index_open, nbor_cell);
					}
				///! in closes.
				} else { 
					// get the cell from closes
					AstarCell nbor_close = closes_.get(index_close);
					// nbor_close.f > nbor_cell.f
					if (nbor_close.compareTo(nbor_cell) < 0) {
						// remove the nbor_close cell from closes.
						closes_.remove(index_close);
						opens_.add(nbor_cell);
					}
				}
			} // end for each neighbor
		} // end while (!_opens.isEmpty())
		
		return null;
	}
}
