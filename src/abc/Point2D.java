package abc;

public class Point2D {
	private int x_ = 0;
	private int y_ = 0;
	
	public Point2D(int _x, int _y) {
		x_ = _x;
		y_ = _y;
	}
	
	public int  getX() { return x_; }
	public int  getY() { return y_; }
	
	public void setX(int _x) { x_ = _x; }
	public void setY(int _y) { y_ = _y; }
	public void setLocation(int _x, int _y) {
		x_ = _x;
		y_ = _y;
	}
	public void setLocation(Point2D _o) {
		setLocation(_o.x_, _o.y_);
	}
	
	public Point2D add(Point2D _o) {
		x_ += _o.x_;
		y_ += _o.y_;
		return this;
	}
	
	public Point2D sub(Point2D _o) {
		x_ -= _o.x_;
		y_ -= _o.y_;
		return this;
	}
	
	public boolean equals(Point2D _o) {
		return ( (x_ == _o.x_) && (y_ == _o.y_) );
	}
	
	@Override
	public String toString() {
		return "(" + x_ + ", " + y_ + ")";
	}
}
