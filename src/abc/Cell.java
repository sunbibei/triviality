package abc;

public class Cell extends Point2D {
	private boolean is_block_ = false;
	
	public Cell(int x, int y) {
		super(x, y);
	}
	
	public Cell(int x, int y, boolean is_block) {
		super(x, y);
		is_block_ = is_block;
	}
	
	public Cell(Point2D _l) {
		super(_l);
		
		if ((_l.getX() < 0) || (_l.getY() < 0)) {
			System.out.println("ERROR LOCATION" + _l.toString() + " FOR CELL");
			super.setLocation(0, 0);
		}
	}
	
	public Cell(Point2D _l, boolean is_block) {
		super(_l);
		is_block_ = is_block;
		
		if ((_l.getX() < 0) || (_l.getY() < 0)) {
			System.out.println("ERROR LOCATION" + _l.toString() + " FOR CELL");
			super.setLocation(0, 0);
		}
	}
	
	public Cell(Cell _l) {
		super(_l);
		is_block_ = _l.is_block_;
		
		if ((_l.getX() < 0) || (_l.getY() < 0)) {
			System.out.println("ERROR LOCATION" + _l.toString() + " FOR CELL");
			super.setLocation(0, 0);
		}
	}
	
	public void setBlock(boolean _b) { is_block_ = _b; }
	
	public boolean isBlock() { return is_block_; }
}
