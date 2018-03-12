package abc;

public class Cell {
	private Point2D location_ = new Point2D(0, 0);
	private boolean is_block_ = false;
	
	public Cell() { }
	
	public Cell(Point2D _l) {
		if ((_l.getX() < 0) || (_l.getY() < 0)) {
			System.out.println("ERROR LOCATION" + _l.toString() + " FOR CELL");
			_l.setLocation(0, 0);
		}
		
		location_.setLocation(_l);
	}
	
	public void setLocation(Cell _o) { location_.setLocation(_o.location_); }
	
	public void setLocation(Point2D _l) { location_.setLocation(_l); }
	
	public Point2D location() { return location_; }
	
	public void setBlock(boolean _b) {
		is_block_ = _b;
	}
	
	public boolean isBlock() { return is_block_; }
}
