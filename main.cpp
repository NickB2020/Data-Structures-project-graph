/*
Nick Bischoff
Data Structures
Assignment 4
File Name: main.cpp, included is a .cpp source code file called GraphMethods that hold the calling functions
Purpose:This program uses an adjacency list graph that loads in github users and followers from a file called github-graph.csv. This program then has a repeating menu for the options to print the top ten most connected users and to print the median and average connections for the vertices of the graph and to print the shortest path between two vertices apon user request. This progrma is to simulate basic social media analysis algorithms.
*/
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <limits>
//using
using std::cout;
using std::cin;
using std::endl;
using std::streamsize;
using std::numeric_limits;
using std::vector;
using std::string;
using std::list;
// declarations
int menu();
void loadData(std::vector<std::list<int>> &, std::ifstream &);
void UserSort(std::vector<std::pair<int, int>> &, int, int);
void generateUsers(std::vector<std::list<int>> &, std::vector<std::pair<int, int>> &);
int indexOfUser(std::vector<std::list<int>> &, int);
bool BreadthFirstSearch(std::vector<std::list<int>> &, int, int, size_t, std::vector<int> &, std::vector<int> &);
void findshortestPath(std::vector<std::list<int>> &, int, int, size_t);
void findTopTen(std::vector<std::pair<int, int>> &);
void findMedian(std::vector<std::pair<int, int>> &);
void findAverage(std::vector<std::pair<int, int>> &);
void shortestPathUtil(std::vector<std::list<int>> &);

int main()
{
    // call the menu interface function
    menu();
}

//***********************************************************************************************
//                          Menu Function For User Interface
//
//***********************************************************************************************
//pre: The graph and supporting functions, and vectors must exist to preform operations in the menu.
//post: The user can now navigate this program with the help of the function that provides a user interface complete if validation and file in reading to populate the graph for ease of testing.
int menu()
{
    //file stream object we're gonna use to open the .csv file holding the data for follower/ following users
    std::ifstream csvFile;
    // and now open the .csv file to access the data within
    csvFile.open("github-graph.csv");

    //Two vectors, one for the graph and one for the users (vertices) which pairs the follower to their following
    vector<std::list<int>> theGraph;
    vector<std::pair<int, int>> users;

    // call to load the data  from .csv into an adjacent list graph
    loadData(theGraph, csvFile);

    //This creates a vector of pairs that hold vertices and amount of connected followers.
    generateUsers(theGraph, users);

    //variable for switch case
    int choice;
    // While loop controls the repeating functionality of the menu after each operation is completed.
    while (1)
    {
        cout << "This program reads in a list of users and their followers from a file called github-graph.csv\nso the graph is"
        << " already created for the user to test and use all operations from the menu.\n" << endl;
        cout << "*****************************************************************************************************" << endl;
        cout << "**** Adjacency List Representation Graph For Basic Social Network Analysis Algorithms For Github ****" << endl;
        cout << "*****************************************************************************************************" << endl;
        cout << "1.Identify the top 10 most connected users " << endl;
        cout << "2.Compute the median number of connections in the graph " <<endl;
        cout << "3.Compute the average number of connections for all the nodes in the graph" << endl;
        cout << "4.Compute the shortest paths between a pair of nodes in the graph" << endl;
        cout << "5.Quit" << endl;
        // Menu choice input validation, will ONLY accept a choice of 1-5.
        do{
                cout << "Enter your choice: ";
                cin >> choice;
                if(cin.fail() || choice < 1 || choice > 5)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cout << " \n* Please enter a numeric option from the selection above *\n" << endl;
                }
           }while(cin.fail() || choice < 1 || choice > 5);
        // Switch statement controls which graph function will be called based on the user's choice from the menu.
        switch(choice)
        {
            // top ten connected users.
        case 1:
            cout << endl;
            findTopTen(users);
            cout << endl;
            system("pause");
            system("cls");
            break;
            // median number of connections
        case 2:
            cout << endl;
            findMedian(users);
            cout << endl;
            system("pause");
            system("cls");
            break;
            // average number of connections for all nodes
        case 3:
            cout << endl;
            findAverage(users);
            cout << endl;
            system("pause");
            system("cls");
            break;
            // shortest path between two nodes
        case 4:
            cout << endl;
            shortestPathUtil(theGraph);
            cout << endl;
            system("pause");
            system("cls");
            break;

            // break loop for menu and exit the program
        case 5:
            return 0;
        }
    }
}
