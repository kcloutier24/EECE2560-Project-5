/*
File Name: maze.h
Created by: Katherine Cloutier, Yizi Wang, & Zachary Spiegel
Course: EECE2560 Fundamentals of Engineering Algorithms
Semester: Fall 2022

This maze.h file was developed for part A of the Maze project. The main
objective of this file is to create a graph that represents legal moves between cells, 
a recursive function that looks for a path from the start cell to the end cell, and
a non recursive function that found a path from the start cell to the end cell.
This class uses both functions, recursive and non recursive, to find a path 
one after another. If no path exists a message will be printed. After each
function was run a seried of corrrect moves of the correct path will be
printed. Both functions took inputs of the graph, the source, and 
the destination point. To approach these two functions a
recursive-DFS, stack-based DFS, and a queue-based DFS 
was considered.

This consisted of:
1. A default constructor to initialize maze values 
2. A function to print the maze
3. A function to determine if the position in the grid is valid
4. A function to set mapping from a maze cell to a node
5. a function to retrieve the mapping of the maze cell
6. a function to graph the legal moves in the maze
7. a function that does the Depth-First Algorithm (recursion)
8. a function that does the Breath-First Algorithm(non-recursion)
9. a function to find the path recursively
10.a function to find the path nonrecursively
11.a function to print out the move sequence needed to solve the maze

NOTE 1: Some of these methods were borrowed from the p5.cpp file provided on
Canvas with the project. These will be noted with "<FROM CANVAS>"
*/

#ifndef MAZE_CLASS
#define MAZE_CLASS

// Include statements
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <queue>
#include <list>
#include <fstream>
#include <limits.h>

#include "d_matrix.h"
#include "d_except.h"
#include "graph.h"

using namespace std;



class Maze
{
	// Public members of Maze Class
	public:
		// Default constructor
		Maze(ifstream& fin);

		// Prints Maze
		void print(int goalI, int goalJ, int currI, int currJ);

		// Determines if position in grid is valid
		bool isLegal(int i, int j);

		// Set mapping from maze cell (i,j) to graph node n
		void setMap(int i, int j, int n);

		// Return mapping of maze cell (i,j) in the graph
		int getMap(int i, int j) const;

		// Create a graph g that represents the legal moves in the maze m
		void mapMazeToGraph(graph& g);

		// Depth-First Algorithm to appropriately solve the maze (recursive)
		void doDFS(graph& g, vector<int>& dfsNodes, int currNode, int endNode);

		// Breath-First Algorithm to appropriately solve the maze
		// (non-recursive)
		void doBFS(graph& g, vector<int>& bfsNodes, int currNode, int endNode);

		// General function for recursive algorithm (DFS) and printing result
		void findPathRecursive(graph& g, int sourceNode, int destNode);

		// General function for non-recursive algorithm (BFS) and printing
		// result
		void findPathNonRecursive(graph& g, int sourceNode, int destNode);

		// Function to print the move sequence needed to solve a given maze
		void printMoveSequence(graph& g, vector<int> sequenceOfNodes);

	// Private members of Maze Class
	private:
		// # rows in Maze
		int rows;

		// # columns in Maze
		int columns;

		// Matrix for storing the maze data from a provided input file
		matrix<bool> mazeGrid;

		// Mapping from maze (i,j) values to node index values
		matrix<int> mapGrid;
};


Maze::Maze(ifstream& fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
// <FROM CANVAS>
{
	fin >> rows;
	fin >> columns;

	char x;

	mazeGrid.resize(rows, columns);
	for (int i = 0; i <= rows - 1; i++)
	{
		for (int j = 0; j <= columns - 1; j++)
		{
			fin >> x;
			if (x == 'O')
				mazeGrid[i][j] = true;
			else
				mazeGrid[i][j] = false;
		}
	}

	mapGrid.resize(rows, columns);
}// End Maze constructor


void Maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board. <FROM CANVAS>
{
	cout << endl;

	if (goalI < 0 || goalI > rows - 1 || goalJ < 0 || goalJ > columns - 1)
		throw rangeError("Bad value in maze::print");

	if (currI < 0 || currI > rows - 1 || currJ < 0 || currJ > columns - 1)
		throw rangeError("Bad value in maze::print");

	for (int i = 0; i <= rows - 1; i++)
	{
		for (int j = 0; j <= columns - 1; j++)
		{
			if (i == goalI && i == currI && j == goalJ && j == currJ)
				cout << ">";
			else if (i == goalI && j == goalJ)
				cout << "*";
			else
				if (i == currI && j == currJ)
					cout << "+";
				else
					if (mazeGrid[i][j])
						cout << " ";
					else
						cout << "X";
		}
		cout << endl;
	}
	cout << endl;
}// End print() function


bool Maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze.
// < FROM CANVAS >
{
	if (i < 0 || i > rows - 1 || j < 0 || j > columns - 1)
		throw rangeError("Bad value in maze::isLegal");

	return mazeGrid[i][j];
}// End isLegal() function


void Maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n.
{
	mapGrid[i][j] = n;
}// End setMap() function


int Maze::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
	return mapGrid[i][j];
}// End getMap() function


void Maze::mapMazeToGraph(graph &g)
// Create a graph g that represents the legal moves in the maze m
{
	// Loops through each cell of the maze. If the cell is empty and can be
	// occupied as a legal path to the end, a new node value is generated
	// and placed into the matrix with mapping information.
	for (int i = 0; i < rows; i++)
	// Loops through each row
	{
		for (int j = 0; j < columns; j++)
		// Loops through each column
		{
			// If the cell is legal, add a new node to the graph; else, set the
			// value in the map to -1
			if (isLegal(i, j))
			{
				setMap(i, j, g.addNode());
			}
			else
			{
				setMap(i, j, -1);
			}// End if
		}// End column for
	}// End row for

	// Loops through each cell of the map matrix from the for loop above. 
	// Determines if each of the nodes has a edge to the right, left, top,
	// and/or bottom. If this is the case, edge information is added to 
	// the graph for each satisfied case
	for (int i = 0; i < rows; i++)
	// Loops through each row
	{
		for (int j = 0; j < columns; j++)
		// Loops through each column
		{
			if ((i - 1 >= 0) && mazeGrid[i][j] && mazeGrid[i - 1][j])
			// Look for above edge
			{
				int currNode = getMap(i, j);
				int upNode = getMap(i - 1, j);
				g.addEdge(currNode, upNode);
				g.addEdge(upNode, currNode);
			}// End above edge if
						
			if ((i + 1 < rows) && mazeGrid[i][j] && mazeGrid[i + 1][j])
			// Look for down edge
			{
				int currNode = getMap(i, j);
				int downNode = getMap(i + 1, j);
				g.addEdge(currNode, downNode);
				g.addEdge(downNode, currNode);
			}// End down edge if
			
			if ((j + 1 < columns) && mazeGrid[i][j] && mazeGrid[i][j + 1])
			// Look for right edge
			{
				int currNode = getMap(i, j);
				int rightNode = getMap(i, j + 1);
				g.addEdge(currNode, rightNode);
				g.addEdge(rightNode, currNode);
			}// End right edge if

			if ((j - 1 >= 0) && mazeGrid[i][j] && mazeGrid[i][j - 1])
			// Look for left edge
			{
				int currNode = getMap(i, j);
				int leftNode = getMap(i, j - 1);
				g.addEdge(currNode, leftNode);
				g.addEdge(leftNode, currNode);
			}// End left edge if
		}// End column if
	}// End row if
}// End mapMazeToGraph() function


void Maze::doDFS(graph& g, vector<int>& dfsNodes, int currNode, int endNode)
// Depth-First Algorithm to appropriately solve the maze (recursive)
{
	//marks edges and visited nodes in the graph
	g.mark(currNode);
	g.visit(currNode);
	dfsNodes.push_back(currNode);

	//checking if the current place is finished with the maze
	if (currNode == endNode)
	{
		return;
	}

	// Vector to store adjacent edges from current node
	vector<int> adjacentEdges;

	// Create vector of adjacent edge nodes
	for (int a = 0; a < g.numNodes(); a++)
	{
		//checks if the current node isnt an edge
		if ((a != currNode) && g.isEdge(a, currNode))
		{
			adjacentEdges.push_back(a);
		}// End if
	}// End for

	// Loops through each of the adjacent edges
	for (int z = 0; z < adjacentEdges.size(); z++)
	{
		//gets information if an edge has been marked or not
		if (!g.isMarked(adjacentEdges[z]))
		{
			//runs function again with unmarked edge
			doDFS(g, dfsNodes, adjacentEdges[z], endNode);

			if (g.isMarked(endNode))
			{
				return;
			}
			else
			{
				dfsNodes.pop_back();
			}
		}
	}
}// End doDFS() function


void Maze::doBFS(graph& g, vector<int>& bfsNodes, int currNode, int endNode)
// Breadth-First Algorithm to appropriately solve the maze (non-recursive)
{
	// Vector to store predecessor information; resize based on number of nodes
	// in graph
	vector<int> pred;
	pred.resize(g.numNodes());

	// Clears the marked edges and visited nodes in the graph
	g.clearMark();
	g.clearVisit();

	// Instantiate queue to track appropriate visit order
	queue<int> q;

	// Push start vertex into queue and mark node as visited
	q.push(currNode);
	g.visit(currNode);

	while (!q.empty())
	// While the queue is not empty...
	{
		// Set integer v to the value at the front of the queue
		int v = q.front();

		if (v == endNode)
		// Checks if the value at the front of the queue is the endNode
		{
			// If v == endNode, iterate backwards through pred vector to
			// determine the shortest path from start ot finish of the maze
			int tempNode = g.numNodes() - 1;

			// Push ending node into the vector
			bfsNodes.push_back(tempNode);

			while (tempNode != 0)
			// While the index of the node is != 0...
			{
				// Determine the next node up one level to get closer to the
				// starting node
				int storedVal = pred[tempNode];

				// Push value into vector containing node path information
				bfsNodes.push_back(storedVal);

				// Update the tempNode value
				tempNode = storedVal;
			}// End while

			// Flip the vector bfsNodes to be in correct order from start to
			// end (while loop above is determine from end to start)
			reverse(bfsNodes.begin(), bfsNodes.end());

			return;
		}// End if

		// Vector to store adjacent edges from current node
		vector<int> adjacentEdges = {};

		// Create vector of adjacent edge nodes
		for (int a = 0; a < g.numNodes(); a++)
		{
			if ((a != v) && g.isEdge(a, v))
			{
				adjacentEdges.push_back(a);
			}// End if
		}// End for

		for (int z = 0; z < adjacentEdges.size(); z++)
		// Loops through each of the adjacent edges
		{
			if (!g.isMarked(adjacentEdges[z]))
			// Checks if the neighbor of the current node v is unvisited
			{
				// If the node is unvisited, mark the neighbor as visited and
				// mark the edge
				g.mark(adjacentEdges[z]);
				g.visit(adjacentEdges[z]);

				// Add the predecessor information for the neighbor edge
				pred[adjacentEdges[z]] = v;

				// Push the neighbor node into the queue
				q.push(adjacentEdges[z]);
			}// End if
		}// End for

		// After going through each adjacent edge of v, pop the front node
		// (which is v) off the queue
		q.pop();
	}// End while
}// End doBFS() function


void Maze::findPathRecursive(graph& g, int sourceNode, int destNode)
// General function for recursive algorithm (DFS) and printing result
{
	// Integers to store start and end cell coordinates of the maze
	int iGoal = 0;
	int jGoal = 0;
	int iStart = 0;
	int jStart = 0;

	// Clears the marked edges and visited nodes in the graph
	g.clearMark();
	g.clearVisit();

	// Double-nested for loop to determine the coordinates for the starting
	// and ending nodes based on the two inputs (sourceNode and destNode)
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (getMap(i, j) == destNode)
			{
				iGoal = i;
				jGoal = j;
			}// End if

			if (getMap(i, j) == sourceNode)
			{
				iStart = i;
				jStart = j;
			}// End if
		}// End column for
	}// End row for

	// Print starting recursive information and intitial grid
	cout << "\nThis is finding a solution to the maze recursively using "
		 << "a Recurse Depth-First-Search (DFS) Algorithm:";

	cout << "\nPrinting starting configuration of Maze. A '+' represents the "
		 << "starting node, and a '*' represents the end node. Locations with "
		 << "an 'X' represent a path that can't be occupied by the maze:\n";
	print(iGoal, jGoal, iStart, jStart);

	// Create vector to store the node path and completes the DFS algorithm
	vector<int> dfsNodes;
	doDFS(g, dfsNodes, 0, g.numNodes() - 1);

	// Print the sequence of moves from start to finish to reach destination
	printMoveSequence(g, dfsNodes);
}


void Maze::findPathNonRecursive(graph& g, int sourceNode, int destNode)
// General function for non-recursive algorithm (BFS) and printing result
{
	// Integers to store start and end cell coordinates of the maze
	int iGoal = 0;
	int jGoal = 0;
	int iStart = 0;
	int jStart = 0;

	// Double-nested for loop to determine the coordinates for the starting
	// and ending nodes based on the two inputs (sourceNode and destNode)
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (getMap(i, j) == destNode)
			{
				iGoal = i;
				jGoal = j;
			}// End if

			if (getMap(i, j) == sourceNode)
			{
				iStart = i;
				jStart = j;
			}// End if
		}// End column for
	}// End row for

	// Print starting non-recursive information and intitial grid
	cout << "\nThis is finding a solution to the maze non-recursively using "
		<< "a Breadth-First-Search (BFS) Algorithm:";

	cout << "\nPrinting starting configuration of Maze. A '+' represents the "
		<< "starting node, and a '*' represents the end node. Locations with "
		<< "an 'X' represent a path that can't be occupied by the maze:\n";
	print(iGoal, jGoal, iStart, jStart);

	// Create vector to store the node path and completes the BFS algorithm
	vector<int> bfsNodes;
	doBFS(g, bfsNodes, 0, g.numNodes() - 1);

	// Print the sequence of moves from start to finish to reach destination
	printMoveSequence(g, bfsNodes);
}


void Maze::printMoveSequence(graph& g, vector<int> sequenceOfNodes)
{
	// Integers to store current and neighbor cell coordinates of the path to
	// solve the maze
	int iCurr = 0;
	int jCurr = 0;
	int iNeighbor = 0;
	int jNeighbor = 0;

	// The goal cell (end) is always the bottom right corner cell
	int iGoal = rows - 1;
	int jGoal = columns - 1;

	if (sequenceOfNodes.size() <= 1)
	// If the passed array containing path information has <= 1 element, then
	// no path was found; print this information
	{
		cout << "No path exists from the start (+) to end (*) of the maze. "
			 << "Quitting...\n";
	}
	else
	// Otherwise, a path exists; print the information
	{
		cout << "A path exists from the start to end of the maze. Printing "
			 << "result:\n";

		for (int a = 0; a < sequenceOfNodes.size() - 1; a++)
		// Iterate through each node in the start to finish path
		{
			for (int i = 0; i < rows; i++)
			// Loop through each row
			{
				for (int j = 0; j < columns; j++)
				// Loop through each column
				{
					// Get current node coordinate information
					if (getMap(i, j) == sequenceOfNodes[a])
					{
						iCurr = i;
						jCurr = j;
					}// End if

					// Get neighbor node coordinate information
					if (getMap(i, j) == sequenceOfNodes[a + 1])
					{
						iNeighbor = i;
						jNeighbor = j;
					}// End if
				}// End column for
			}// End row for

			if (a == 0)
			{
				cout << "\nStarting Configuration:";
			}// End if

			// Print the maze grid with the provided print function
			print(iGoal, jGoal, iCurr, jCurr);

			// Integers used to determine which direction the maze path is
			// moving to be solved
			int iDifference = iCurr - iNeighbor;
			int jDifference = jCurr - jNeighbor;

			// Logic to properly print the direction the maze path is moving
			// bbased on the iDifference and jDifference values
			if (iDifference == 0 && jDifference == 1)
			{
				cout << "Move Left:";
			}
			else if (iDifference == 0 && jDifference == -1)
			{
				cout << "Move Right:";
			}
			else if (iDifference == 1 && jDifference == 0)
			{
				cout << "Move Up:";
			}
			else if (iDifference == -1 && jDifference == 0)
			{
				cout << "Move Down:";
			}// End if

			if (a == sequenceOfNodes.size() - 2 
			   && sequenceOfNodes[a + 1] == g.numNodes() - 1)
			// If the last node is reached, print the grid showing the end has
			// been reached and print corresponding statement
			{
				print(iGoal, jGoal, iGoal, jGoal);
				cout << "The '>' symbol means the end of the maze has been "
					 << "reached! Quitting...\n\n";
			}// End if
		}// End for
	}// End if
}// End printMoveSequence() function

#endif

// End maze.h