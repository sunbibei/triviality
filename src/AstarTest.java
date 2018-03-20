import java.util.Stack;

import abc.BoxMap;
import abc.Point2D;
import planning.Astar;

public class AstarTest {
	///! Just for debug BoxMap
	public static void main(String[] args) {
		BoxMap map = new BoxMap(5, 5);
		// test block 1
		;
		// test block 2
		map.setBlock(new Point2D(2, 2), new Point2D(4, 4));
		// test block 3
		// map.setBlock(new Point2D(2, 2), new Point2D(6, 6));
		System.out.println(map);
		
		// test path 1
		System.out.println("test path 1");
		Point2D start = new Point2D(0, 0);
		Point2D end   = new Point2D(3, 3);
		
		Astar slover = new Astar();
		Stack<Point2D> path = slover.planning(map, start, end);
		if (null == path) {
			System.out.println("Could not found the path!");
			return;
		}
		
		System.out.println("The path:");
		System.out.println(map.printPath(path));
		
		Point2D p = path.pop();
		System.out.print(p);
		while (!path.isEmpty()) {
			p = path.pop();
			System.out.print("->" + p);
		}
		System.out.println();
	}
	
	///! Just for debug BoxMap
	public static void main1(String[] args) {
		BoxMap map = new BoxMap(20, 20);
		// test block 1
		;
		// test block 2
		map.setBlock(new Point2D(2, 2), new Point2D(10, 10));
		// test block 3
		// map.setBlock(new Point2D(2, 2), new Point2D(6, 6));
		System.out.println(map);
		
		// test path 1
		System.out.println("test path 1");
		Point2D start = new Point2D(0, 0);
		Point2D end   = new Point2D(11, 11);
		
		Astar slover = new Astar();
		Stack<Point2D> path = slover.planning(map, start, end);
		if (null == path) {
			System.out.println("Could not found the path!");
			return;
		}
		
		System.out.println("The path:");
		System.out.println(map.printPath(path));
		
		Point2D p = path.pop();
		System.out.print(p);
		while (!path.isEmpty()) {
			p = path.pop();
			System.out.print("->" + p);
		}
		System.out.println();
		
		// test path 2
		System.out.println("test path 2");
		start = new Point2D(4, 1);
		end   = new Point2D(19, 17);
		path = slover.planning(map, start, end);
		if (null == path) {
			System.out.println("Could not found the path!");
			return;
		}
		
		System.out.println("The path:");
		System.out.println(map.printPath(path));
		
		p = path.pop();
		System.out.print(p);
		while (!path.isEmpty()) {
			p = path.pop();
			System.out.print("->" + p);
		}
		
		// test path 3
		System.out.println("test path 3");
		start = new Point2D(1, 2);
		end   = new Point2D(19, 12);
		path = slover.planning(map, start, end);
		if (null == path) {
			System.out.println("Could not found the path!");
			return;
		}
		
		System.out.println("The path:");
		System.out.println(map.printPath(path));
		
		p = path.pop();
		System.out.print(p);
		while (!path.isEmpty()) {
			p = path.pop();
			System.out.print("->" + p);
		}
	}
}
