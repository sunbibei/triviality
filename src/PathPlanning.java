import java.util.PriorityQueue;

/*
Pseudo code:
OPEN //the set of nodes to be evaluated
CLOSED //the set of nodes already evaluated
add the start node to OPEN
loop
 current = node in OPEN with the lowest f_cost
 remove current from OPEN
 add current to CLOSED
 if current is the target node //path has been found
 	return
 foreach neighbour of the current node
	 if neighbour is not traversable or neighbour is in CLOSED
	 	skip to the next neighbour
	 if new path to neighbour is shorter OR neighbour is not in OPEN
	 	set f_cost of neighbour
	 	set parent of neighbour to current
	 if neighbour is not in OPEN
	 	add neighbour to OPEN
*/

public class PathPlanning {
    public static final int DIAGONAL_COST = 6; //
    public static final int V_H_COST = 3; //Cost for robot to go straight
    //The class defining the grid cell with all the variables
    public class Cell {
        int heuristicCost = 0; //Heuristic cost
        int g = V_H_COST;
        int finalCost = 0; //
        int x, y;
        Cell parent;
        Cell(int x, int y){
            this.x = x;
            this.y = y;
        }

        public int getX(){
            return x;
        }

        public int getY(){
            return y;
        }

        @Override
        public String toString(){
            return "["+this.x+", "+this.y+"]";
        }
    }
    //The grid for the robot to explore
    Cell[][] grid = new Cell[60][60];
    //The open and close list
    PriorityQueue<Cell> open;
    boolean closed[][];
    //x and y coordinates of the start and target position
    int startI, startJ;
    int endI, endJ;
    //Blocked cells are just null Cell values in grid, resemble the obstacles
    public void setBlocked(int i, int j){
        grid[i][j] = null;
    }
    public void setStartCell(int i, int j){
        startI = i;
        startJ = j;
    }
    public void setEndCell(int i, int j){
        endI = i;
        endJ = j;
    }
    public int heuristic(Cell current, Cell target) {
        int d1 = Math.abs(current.getX() - target.getX());
        int d2 = Math.abs(current.getY() - target.getY());
        return d1 + d2;
    }
    //Main function to check the cost of target position and decide whether to put it into
    //the close list or open list.
    public void checkAndUpdateCost(Cell current, Cell target, int gcost){

        if(target == null || closed[target.x][target.y]){
            return;
        }
        // total cost for the target position
        int t_final_cost = gcost + heuristic(current, target);
        boolean inOpen = open.contains(target);
        if(!inOpen || t_final_cost<target.finalCost){
            target.finalCost = t_final_cost;
            target.parent = current;
            if(!inOpen)open.add(target);
        }
    }
    public void AStar(){
    //add the start location to open list.
    open.add(grid[startI][startJ]);
    Cell current;
    /* What while loop does:
     current = node in OPEN with the lowest f_cost
     remove current from OPEN
     add current to CLOSED

     if current is the target node //path has been found
     return

     foreach neighbour of the current node
     if neighbour is not traversable or neighbour is in CLOSED
     skip to the next neighbour
     if new path to neighbour is shorter OR neighbour is not in OPEN
     set f_cost of neighbour
     set parent of neighbour to current
     if neighbour is not in OPEN
     add neighbour to OPEN
    */
        while(true){
            current = open.poll();
            if(current==null)break;
            closed[current.x][current.y]=true;

            // if current is the target node which means path has been found, return,
            //fill in the underlined places
            if(current.equals(grid[endI][endJ])){
                return;
            }
            Cell neighbour;
            if(current.x-3>=0){
                neighbour = grid[current.x-3][current.y];
                checkAndUpdateCost(current, neighbour, current.finalCost+V_H_COST); // If
                //the neighbour can be reached by going straight, use V_H_COST
                if(current.y-3>=0){
                    neighbour = grid[current.x-3][current.y-3];
                    checkAndUpdateCost(current, neighbour,
                            current.finalCost+DIAGONAL_COST); // If the neighbour can be reached by going diagonal, use
                    //DIAGONAL_COST
                }
                if(current.y+3<grid[0].length){
                    neighbour = grid[current.x-1][current.y+1];
                    checkAndUpdateCost(current, neighbour,
                            current.finalCost+DIAGONAL_COST);
                }
            }
            if(current.y-3>=0){
                neighbour = grid[current.x][current.y-1];
                checkAndUpdateCost(current, neighbour, current.finalCost+V_H_COST);
            }
            if(current.y+3<grid[0].length){
                neighbour = grid[current.x][current.y+1];
                checkAndUpdateCost(current, neighbour, current.finalCost+V_H_COST);
            }
            if(current.x+3<grid.length){
                neighbour = grid[current.x+1][current.y];
                checkAndUpdateCost(current, neighbour, current.finalCost+V_H_COST);
                if(current.y-3>=0){
                    neighbour = grid[current.x+1][current.y-1];
                    checkAndUpdateCost(current, neighbour,
                            current.finalCost+DIAGONAL_COST);
                }
                if(current.y+3<grid[0].length){
                    neighbour = grid[current.x+1][current.y+1];

                    //right type of cost for this situation
                    checkAndUpdateCost(current, neighbour, current.finalCost+ DIAGONAL_COST);
                }
            }
        }
    }
}
