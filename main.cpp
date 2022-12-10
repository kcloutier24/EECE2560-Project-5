/*
File Name: main.cpp
Created by: Katherine Cloutier, Yizi Wang, & Zachary Spiegel
Course: EECE2560 Fundamentals of Engineering Algorithms
Semester: Fall 2022

This main.cpp file was developed for part A of the Maze project. The main
objective of this file is to initialize a graph and maze object, read a
maze grid, and solve the maze both recursively and non-recursively.

NOTE 1: Some of these methods were borrowed from the p5.cpp file provided on
Canvas with the project. These will be noted with "<FROM CANVAS>"
*/



// Include statements
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>

#include "maze.h"

using namespace std;



int main()
// Main function to meet the requirements of the Maze project Part A
// <FROM CANVAS>, modified by team
{
    // String to store input filename
    string inputFileName;

    // Input file object
    ifstream fin;

    cout << "Welcome to the Maze Game!\n\n";

    // Output to console to ask user to enter .txt filename
    // NOTE: This file must be located in the same directory as this C++ file
    cout << "Please enter the name of the Maze File that should be used. "
         << "Please ensure file is located in same folder as this CPP file "
         << "and contains the filename along with the .txt extension: ";

    cin >> inputFileName;

    // Open input from inputFileName
    fin.open(inputFileName.c_str());

    while (!fin)
    // Error checking to see if input file is valid
    {
        // If not valid, user if forced to keep entering file until entry is
        // valid
        cout << "File name entered is not valid. Please try again: ";
        cin >> inputFileName;

        fin.open(inputFileName.c_str());
    }// End while

    try
    {
        // Graph class object
        graph g;

        while (fin && fin.peek() != 'Z')
        // Checks to ensure maze grid data is present and that the end of 
        // file	character (Z) is not present as the next character from the current
        // one
        {
            // Instantiate Maze object and convert maze to graph
            Maze m(fin);
            m.mapMazeToGraph(g);

            // Call Recursive and Non-Recursive functions to solve the maze
            m.findPathRecursive(g, 0, g.numNodes() - 1);
            m.findPathNonRecursive(g, 0, g.numNodes() - 1);
        }

    }
    catch (indexRangeError& ex)
    // Error handling
    {
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError& ex)
    // Error handling
    {
        cout << ex.what() << endl; exit(1);
    }

	return 0;
}// End main() function

// End main.cpp