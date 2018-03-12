package planning;

import java.util.ArrayList;
import java.util.List;
import java.util.PriorityQueue;

import abc.*;

public class Astar {

	public class AstarCell extends Cell implements Comparable<AstarCell> {
		private int g_cost_ = 0;
		private int h_cost_ = 0;
		private int f_cost_ = 0;
		private Cell parent = null;
		
		public AstarCell(Point2D _p) {
			super.setLocation(_p);
		}
		
		public AstarCell(Cell _p) {
			super.setLocation(_p);
		}
				
		public AstarCell(Cell _c, Cell _p, int g, int h, int f) {
			;
		}
		
		public boolean equals(Point2D _p) {
			return location().equals(_p);
		}
		
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
	
	public static List<Point2D> planning(BoxMap map, Point2D start, Point2D end) {
		List<Point2D> path = new ArrayList<Point2D>();
		path.add(start);
		PriorityQueue<AstarCell> _opens  = new PriorityQueue<AstarCell>();
		List<Point2D> _closes = new ArrayList<Point2D>();
		
		while (!_opens.isEmpty()) {
			AstarCell c_node = _opens.poll();
			
			if (c_node.equals(end)) return path;
			
			
		}
		
		
		path.forEach((_p) -> System.out.print("adv"));
		
		return path;
	}
}
