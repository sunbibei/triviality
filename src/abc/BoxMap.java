package abc;

import java.util.Stack;
import java.util.function.Consumer;

/*!
 * @brief The class for BoxMap, the origin is placed at the left top. example as:
 * 			0 1 2 3 4 -- y
 *          1
 *          2
 *          3
 *          4
 *          |
 *          x
 */
public class BoxMap {	
	///! the data of map
	private int WIDTH  = 0;
	private int HEIGHT = 0;
	private Cell[][] map_ = null;
	
	public BoxMap(final int WIDTH, final int HEIGHT) {
		this.WIDTH  = WIDTH;
		this.HEIGHT = HEIGHT;
		
		map_ = new Cell[WIDTH][HEIGHT];
		for (int w = 0; w < WIDTH; ++w) {
			for (int h = 0; h < HEIGHT; ++h) {
				map_[w][h] = new Cell(w, h);
			}
		}
	}
	
	public void forEachNeighbour(final Cell _p, Consumer<Cell> action) {
		action.accept(new Cell(_p));
	}
	
	public Cell getCell(Point2D p) { 
		if ((p.getX() < 0) || (p.getX() >= WIDTH) ||
				(p.getY() < 0) || (p.getY() >= HEIGHT)) {
			// System.out.println("ERROR LOCATION" + p.toString() + " FOR CELL");
			return null;
		}
		return map_[p.getX()][p.getY()];
	}
	
	public void setBlock(int x, int y/*, boolean _b*/) { map_[x][y].setBlock(true); }
	public void setBlock(int x, int y, boolean _b)     { map_[x][y].setBlock(_b); }
	/**
	 * Set the block in the left-top to right-bottom.
	 */
	public void setBlock(Point2D _lt, Point2D _rb) {
		for (int x = _lt.getX(); x < _rb.getX(); ++x) {
			for (int y = _lt.getY(); y < _rb.getY(); ++y) {
				setBlock(x, y);
			}
		}
	}
	public void setBlock(Point2D _lt, Point2D _rb, boolean _b) {
		for (int x = _lt.getX(); x < _rb.getX(); ++x) {
			for (int y = _lt.getY(); y < _rb.getY(); ++y) {
				setBlock(x, y, _b);
			}
		}
	}
	
	public String printPath(Stack<Point2D> path) {
		String ret = "";
		for (int w = 0; w < WIDTH; ++w) {
			for (int h = 0; h < HEIGHT; ++h) {
				if (map_[w][h].isBlock())
					ret += "*";
				else if (-1 != path.indexOf(new Point2D(w, h)))
					ret += "-";
				else
					ret += "o";
			}
			ret += "\n";
		}
		return ret;
	}
	
	@Override
	public String toString() {
		String ret = "";
		for (int w = 0; w < WIDTH; ++w) {
			for (int h = 0; h < HEIGHT; ++h) {
				if (map_[w][h].isBlock())
					ret += "*";
				else
					ret += "o";
			}
			ret += "\n";
		}
		return ret;
	}
}
