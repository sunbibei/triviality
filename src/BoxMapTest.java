import abc.*;

public class BoxMapTest {
	///! Just for debug BoxMap
	public static void main(String[] args) {
		BoxMap map = new BoxMap(4, 4);
		/*map.setBlock(0, 1);
		map.setBlock(0, 2);
		map.setBlock(0, 3);
		map.setBlock(1, 1);
		*/
		map.setBlock(new Point2D(2,2), new Point2D(4,4));
		
		map.forEachNeighbour(new Point2D(100, 100), (_p) -> {
			System.out.println("foreach: [" + _p.getX() + ", " + _p.getY() + "]");
		});
		
		// System.out.println(map);
	}
}
