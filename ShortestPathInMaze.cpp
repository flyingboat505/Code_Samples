/******************************************************************************
Complete the function FindPath below.  This function takes a matrix object
(defined below), and a start and an end cells within that matrix.
The matrix is filled with values of either 0 or 1.  The function should return
the shortest path between the start and the end cells given, such that no
matrix cells with value of 1 are part of the path.  The function should return
this path in the form of sequence of Matrix cells, including start and end.
A valid path consists of sequence of matrix cells adjacent to each other.
Cell A is adjacent to cell B if it is immediately to the right, left,
above, or below.  For example, in the following matrix:

 0  1  2
 3  4  5
 6  7  8

 Cell "4" is adjacent to cells 3, 1, 5, and 7.

There are no wraparounds, e.g. cells 7 and 1 are not adjacent.

Cells are identified via (row, col), starting from 0, i.e. in the following matrix
0 0 0 0 0
0 0 0 0 1
0 0 0 0 0

Cell with value 1 is at (1, 4)

Example 1:

Matrix:
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
Start:(0,0) End:(2, 3)

Possible solution (one of many):  (0,0),(0,1),(0,2),(0,3),(1,3),(2,3)

Example 2:

Matrix:
0 1 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0 0
0 1 0 0 0 0 0 0 0
Start:(0,0) End:(2, 3)

Solution:  (0,0),(1,0),(1,1),(1,2),(2,2),(2,3)

If no path could be found, the function should return an empty list.

******************************************************************************/

#include <vector>
#include <memory>
#include <iostream>
#include <queue>



using namespace std;
struct Location
{
    size_t row_;//
    size_t col_;
};

using Locations = std::vector<Location>;

struct Matrix
{
public:
    explicit Matrix(size_t rows, size_t cols, const Locations& walls) 
    : rows_{ rows }, cols_{ cols }, data_(new int[rows * cols])
    { InitializeMatrix(walls); }

    void InitializeMatrix(const Locations& walls)
    {
        // Zero data
        for (size_t row = 0; row < rows_; row++)
        {
            for (size_t col = 0; col < cols_; col++)
            {
                data_[cols_ * row + col] = 0;
            }
        }

        // Add walls
        for (const auto& wall : walls)
        {
            data_[cols_ * wall.row_ + wall.col_] = 1;
        }
    }
    
    //adding helper function-print matrix
    void printMatrix()
    {
        // Zero data
        for (size_t row = 0; row < rows_; row++)
        {
            for (size_t col = 0; col < cols_; col++)
            {
                cout<<data_[cols_ * row + col];
            }
            cout<<"\n";
        }
    }

    const int& operator()(const Location& location) const
    {
        if (location.row_ >= rows_ || location.col_ >= cols_)
            throw std::out_of_range("OOB (Assuming you want an exception and not return an empty list if the cordinate is out of range) ");

        return data_[cols_ * location.row_ + location.col_];
    }

    size_t NumRows() const { return rows_; }
    size_t NumCols() const { return cols_; }

private:
    size_t rows_;
    size_t cols_;
    std::unique_ptr<int[]> data_;
};


struct Visited_Matrix
{
public:
    explicit Visited_Matrix(size_t rows, size_t cols) 
    : rows_{ rows }, cols_{ cols }, visited_data_(new int[rows * cols])
    { InitializeMatrix(); }

    void InitializeMatrix()
    {
        // Zero data
        for (size_t row = 0; row < rows_; row++)
        {
            for (size_t col = 0; col < cols_; col++)
            {
                visited_data_[cols_ * row + col] = 0;
            }
        }
    }
    //adding helper function-print matrix
    void printMatrix()
    {
        // Zero data
        for (size_t row = 0; row < rows_; row++)
        {
            for (size_t col = 0; col < cols_; col++)
            {
                cout<<visited_data_[cols_ * row + col];
            }
            cout<<"\n";
        }
    }
    
    const int& operator()(const Location& location) const
    {
        if (location.row_ >= rows_ || location.col_ >= cols_)
            throw std::out_of_range("OOB");

        return visited_data_[cols_ * location.row_ + location.col_];
    }
    
    void markVisited(const Location& location)
    {
        visited_data_[cols_ * location.row_ + location.col_] = 1;
    }
    
     void markFree(const Location& location) 
    {
        visited_data_[cols_ * location.row_ + location.col_] = 0;
    }
    
    bool isFree(const Location& location)
    {
        return (visited_data_[cols_ * location.row_ + location.col_] == 0);
    }
    
    
    size_t NumRows() const { return rows_; }
    size_t NumCols() const { return cols_; }

private:
    size_t rows_;
    size_t cols_;
    std::unique_ptr<int[]> visited_data_;
};

ostream & operator << (ostream &out, const Location &value) 
{ 
    out << "Row " << value.row_ <<" Column " <<value.col_ <<endl; 
    return out; 
}

ostream & operator << (ostream &out, const vector<Location> &value) 
{ 
    for(auto it=value.begin(); it!=value.end(); ++it){
        cout<<*it;
    }
    cout<<"\n";
}



bool operator==(const Location& cell_1, const Location& cell_2)
{
     return cell_1.row_ == cell_2.row_  &&
            cell_1.col_ == cell_2.col_ ;
}


bool isValid (const Location& location,const Matrix& m) 
    {
        if (location.row_ >= m.NumRows() || location.row_ <0 ||  location.col_ >= m.NumCols() ||  location.col_ < 0)
            return false; 
        else
            return true;
    }
//works with the for loop to generate all possible direction a node can go    
Location getUpdatedNode(const Location location, int index){
    Location val;
    switch(index) {
        case 0:
            val.row_=location.row_-1;
            val.col_ =location.col_;
            break;
        case 1:
            val.row_=location.row_;
            val.col_ =location.col_-1;
            break;
            break;
        case 2:
            val.row_=location.row_;
            val.col_ =location.col_+1;
            break;
        case 3:
            val.row_=location.row_+1;
            val.col_ =location.col_;
            break;
    }
    return val;         

}
//retrieves shortest distance- that will be used as a constraint/optimization for GetShortestPath
int GetShortestDistance(const Matrix& m, const Location& start, const Location& end){
    queue<Location> myQueue;
    queue<int> distanceQueue;
    
    Visited_Matrix visited(m.NumRows(),m.NumCols()); 
    visited.markVisited(start);
    
    myQueue.push(start);
    distanceQueue.push(0);
    
    while (!myQueue.empty()){
        Location val = myQueue.front();
        int currentDist = distanceQueue.front();
        myQueue.pop();
        distanceQueue.pop();
        
        if (val==end)
		{
			return currentDist;
		}
		
		for (int i = 0; i < 4; i++){
		    Location temp=getUpdatedNode(val,i);
		    if (isValid(temp,m) && visited.isFree(temp) && m(temp)!=1)
			{
			        visited.markVisited(temp);
    				myQueue.push(temp);
    				distanceQueue.push(currentDist+1);
			}
		}
    }
    
    return -1;
}
Locations FinalAnswer; //global variable-would've put this and the function in a class if the Question allowed me(for encapsulation)
int flag=0; //flag variable to optimize return path 
//optimized backtracking solution-(got shortest_Distance first and then generated all paths with shortest_Distance as a constraint)
//included a flag variable to speed the stack up.
void GetShortestPath(const Matrix& m, const Location& end,const int shortest_Distance,int current_Distance,Locations &current_path,Location current_Location,Visited_Matrix &visited) {
    if(flag==1){
        return;
    }
    if(current_Location==end && current_Distance==shortest_Distance ){
         cout<<"Final Answer:RETURN THIS VECTOR"<<endl;
         cout<<current_path;
         flag=1;
         FinalAnswer=current_path;
        return ;
    }
    else{
    	for (int i = 0; i < 4; i++){
    		    Location current_pos=getUpdatedNode(current_Location,i);
    		    if (isValid(current_pos,m) && visited.isFree(current_pos) && (current_Distance+1<=shortest_Distance) && m(current_pos)!=1)
    			{
                     //cout<<current_path;
        			 current_path.push_back(current_pos);
        			 visited.markVisited(current_pos);
        			 GetShortestPath(m,end,shortest_Distance,current_Distance+1,current_path,current_pos,visited  );
                     if(flag==0){
                         current_path.pop_back();
                         visited.markFree(current_pos);
                     }
                     else
                         return;
    			}
    	}
    }
}


Locations FindPath(const Matrix& m, const Location& start, const Location& end)
{

    if(m(start )==1 or m(end )==1 or m.NumRows()<1 or m.NumCols()<1 ){
        return {};
    }
    else if (start==end) {
        return {start};
    }

    int shortest_dist=GetShortestDistance(m,start, end);
    cout<<"Distance "<<shortest_dist<<endl;
    
    if(shortest_dist==-1)
        return {};  
    else{
        Locations shortest_path;
        Visited_Matrix visited(m.NumRows(),m.NumCols()); 
        visited.markVisited(start);
        shortest_path.push_back(start);
        GetShortestPath(m,end,shortest_dist,0,shortest_path,start, visited);
        return FinalAnswer;
        
    }
    
}
 
    
int main ()
{
    Locations walls;
    walls.push_back({1, 1});
    walls.push_back({2, 2});
    walls.push_back({1, 9});
    Matrix matrix(3,10, walls);
    matrix.printMatrix();
    Locations ans=FindPath(matrix, {2,1}, {2,9});
    cout<<"Final Answer"<<endl;
    cout<<ans;
    
}


