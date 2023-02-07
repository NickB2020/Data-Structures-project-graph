
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

//***********************************************************************************************
//                          load data
//
//***********************************************************************************************
//pre: menu function must exist to beable to load in the .csv data
//post: The adjacency list is graph if now created
//load data from .csv into an adjacency list graph
void loadData(std::vector<std::list<int>> &theGraph, std::ifstream &csvFile)
{
    //the adjacency list
    list<int> theList;
    //the position in the file
    string csvPos;
    // one for each the follower and following
    int userFollower, userFollowing = 0;


    //We need to remove the "follower, following" line in the .csv file and jump to the the next line which has the first user and who they are following
    getline(csvFile, csvPos);
    getline(csvFile, csvPos);

    //use the find the comma and use the comma to separate the users
    size_t separateFollowersFromFollowing = csvPos.find(',');
    // make into sub strings then we need to convert to int's so we can do the median and average
    userFollower = stoi(csvPos.substr(0, separateFollowersFromFollowing));
    userFollowing = stoi(csvPos.substr(separateFollowersFromFollowing + 1, csvPos.find("\n")));

    //push on to the list the first round of users
    theGraph.push_back(theList);
    //push the first user that in the follower
    theGraph[0].push_back(userFollower);
    //push the first user that in the following user
    theGraph[0].push_back(userFollowing);

    //basically while not end of file, keep grabbing the users from the file
    while(getline(csvFile, csvPos))
    {
        // to control when to stop pushing users onto list
        bool notFinished = true;
        //use the find the comma and use the comma to separate the users
        size_t separateFollowersFromFollowing = csvPos.find(',');
        // make into sub strings then we need to convert to int's so we can do the median and average
        userFollower = stoi(csvPos.substr(0, separateFollowersFromFollowing));
        userFollowing = stoi(csvPos.substr(separateFollowersFromFollowing + 1, csvPos.find("\n")));

        //size is now needed
        size_t size = theGraph.size();


        //keep going until you find no more users are found
        for(size_t i = 0; i < size && notFinished; ++i)
        {
            //if the user in the line is the first in the list.
            if(userFollower == theGraph[i].front())
            {
                //This adds to the end of the vector and sets notFinished to false to break the loop.
                theGraph[i].push_back(userFollowing);
                notFinished = false;
            }
        }

        //if the follower user value in line is not in the list.
        if(notFinished)
        {
            //adds to end of the vector.
            theGraph.push_back(theList);
            size_t i = theGraph.size() - 1;
            // added to the end
            theGraph[i].push_back(userFollower);
            theGraph[i].push_back(userFollowing);
        }
    }
}

//***********************************************************************************************
//                         user sort quick sort for the vectors
//
//***********************************************************************************************
//pre: menu function must exist to beable to load in the .csv data, vectors must exist to be able to be sorted
//post: Now we can quick sort the vectors
void userSort(std::vector<std::pair<int, int>> &users, int small, int big)
{
    //if the lowerValue is less than the higherValue.
    if(small < big)
    {
        //This selects the higherValue follower user count as pivot point.
        int pivot = users[big].second;
        int i = (small - 1);

        //iterate from lower to higher.
        for(int j = small; j <= big - 1; j++)
        {
            //if the follower user count and index j is greater than the pivot.
            if(users[j].second >= pivot)
            {
                //This increments temp variable by one.
                i++;

                //This swaps the two Nodes at indices i and j;
                int temp = users[i].first;
                users[i].first = users[j].first;
                users[j].first = temp;

                //This swaps the two follower user counts at indices i and j;
                int temp2 = users[i].second;
                users[i].second = users[j].second;
                users[j].second = temp2;
            }
        }

        //This swaps the two Nodes at indices i+1 and the higher;
        int temp3 = users[i + 1].first;
        users[i + 1].first = users[big].first;
        users[big].first = temp3;

        //This swaps the two follower counts at indices i+1 and the higher;
        int temp4 = users[i + 1].second;
        users[i + 1].second = users[big].second;
        users[big].second = temp4;

        //This declares and initializes a variable to i+1.
        int num = i + 1;

        //recursively calls itself until sorted.
        userSort(users, small, num - 1);
        userSort(users, num + 1, big);
    }
}

//***********************************************************************************************
//                         generate users
//
//***********************************************************************************************
//pre: menu function must exist to beable to load in the .csv data
//post: Now we have a vector to hold the vertices for the graph
void generateUsers(std::vector<std::list<int>> &theGraph, std::vector<std::pair<int, int>> &users)
{
    //iterates through the graph and adds the unique Nodes as pairs with zero followers.
    for(size_t i = 0; i < theGraph.size(); i++)
    {
        users.push_back(std::make_pair(theGraph[i].front(), 0));
    }

    //This iterates through the graph.
    for(size_t i = 0; i < theGraph.size(); i++)
    {
        //This creates an iterator that starts at the beginning of the list in the graph at index i.
        list<int>::iterator j = ++theGraph[i].begin();

        //This iterates through the list in the graph at index i.
        for(j; j != theGraph[i].end(); ++j)
        {
            //control when vertice found.
            bool found = false;

            //iterate the vector of pairs .
            for(size_t k = 0; k < users.size() && !found; ++k)
            {
                //if found .
                if(users[k].first == *j)
                {
                    //count the found vertices.
                    found = true;
                    ++users[k].second;
                }
            }
        }
    }

    //sort the vec of pairs by the amount of follower users.
    userSort(users, 0, (int)theGraph.size() - 1);
}

//***********************************************************************************************
//                          indexOfUser
//
//***********************************************************************************************
//pre: menu function must exist to beable to load in the .csv data, vectors and loadData() and  generateUsers() must exist for indexing
//post: Now our vertices are indexed and if not found then returned not found
int indexOfUser(std::vector<std::list<int>>& theGraph, int user)
{
    //iterate the graph.
    for(size_t i = 0; i < theGraph.size(); ++i)
    {
        //if the first value in list in graph is same to requested Node.
        if(theGraph[i].front() == user)
        {
            return i;
        }
    }

    //if not found the return not found.
    return -1;
}

//***********************************************************************************************
//                         BreadthFirstSearch
//
//***********************************************************************************************
//pre: menu function must exist, vectors and loadData() and  generateUsers() so we can search
//post: We can now search through the graph  using this unique search
bool BreadthFirstSearch(std::vector<std::list<int>> &theGraph, int beg, int ending, size_t size, std::vector<int> &prev, std::vector<int> &counter)
{
    // list to hold the values in queue
    list<int> theQueue;
    //vector to hold all the vertices that have been marked
    vector<bool> marked;
    marked.resize(size);

    //initialize all values in the vectors.
    for(size_t i = 0; i < size; ++i)
    {
        marked[i] = false;
        counter[i] = INT_MAX;
        prev[i] = -1;
    }

    //marked.
    marked[beg] = true;
    //counter counts the paths
    counter[beg] = 0;
    theQueue.push_back(beg);

    //while Queue not empty.
    while(!theQueue.empty())
    {
        //first val in Queue.
        int firstIn = theQueue.front();
        theQueue.pop_front();

        //iterate the graph
        for(auto iter = theGraph[firstIn].begin(); iter != theGraph[firstIn].end(); ++iter)
        {
            //index of the graph where iterator is currently
            int index = indexOfUser(theGraph, *iter);

            //if index exists and not been visited.
            if(index != -1 && marked[index] == false)
            {
                //updates the values.
                marked[index] = true;
                counter[index] = counter[firstIn] + 1;
                prev[index] = firstIn;
                theQueue.push_back(index);

                //true if found.
                if(*iter == theGraph[ending].front())
                {
                    return true;
                }
            }
        }
    }

    //if not found.
    return false;
}

//***********************************************************************************************
//                         find Shortest Path
//
//***********************************************************************************************
//pre: menu function must exist to beable to load in the .csv data, vectors and loadData() and  generateUsers() and the BreadthFirstSearch() needs to exist to search for vertices in the graph
//post: We can now find the shortest path between two user requested vertices and display them
void findShortestPath(std::vector<std::list<int>> &theGraph, int beg, int ending, size_t size)
{
    // vec for previous  and path count.
    std::vector<int> prev;
    std::vector<int> counter;
    prev.resize(size);
    counter.resize(size);

    //if path of the vertices cannot be found.
    if(BreadthFirstSearch(theGraph, beg, ending, size, prev, counter) == false)
    {
        cout << theGraph[beg].front() << " and " << theGraph[ending].front() << " are not connected." << endl;
        return;
    }

    //vec to next path
    std::vector<int> pathToNext;
    int iter = ending;
    pathToNext.push_back(iter);

    //shortest space between vertices.
    while(prev[iter] != -1)
    {
        pathToNext.push_back(prev[iter]);
        iter = prev[iter];
    }

    //print shortest path of the requested vertices
    cout << "Shortest path between " << theGraph[beg].front() << " and " << theGraph[ending].front() << " is: " << counter[ending] << endl;
    cout << endl;
    // print the path
    for(int i = pathToNext.size() - 1; i >= 0; --i)
    {
        if(i != 0)
        {
            cout << " " << theGraph[pathToNext[i]].front() << "-> ";
        }
        else
        {
            cout << " " << theGraph[pathToNext[i]].front() << endl;
        }
    }
}

//***********************************************************************************************
//                         find Top Ten
//
//***********************************************************************************************
//pre: menu function must exist to beable to load in the .csv data, vectors and loadData() and  generateUsers() and the BreadthFirstSearch() needs to exist to search for vertices in the graph
//post: We can now display the top ten connected vertices (users) ( followers) in the graph
void findTopTen(std::vector<std::pair<int, int>> &users)
{
    cout << "The Top Ten Most Connected Users are:\n" << endl;
    //print top ten
    for (size_t iter = 0; iter < 10; ++iter)
    {
        cout << "Ranking: " << iter + 1 << " place is ";
        cout << " user: " << users[iter].first << " with "
        << users[iter].second << " connections\n" << endl;
    }
}
//***********************************************************************************************
//                         findMedian
//
//***********************************************************************************************
//pre: menu function must exist to beable to load in the .csv data, vectors and loadData() and  generateUsers() and the BreadthFirstSearch() needs to exist to search for vertices in the graph
//post: We can now  calculate and display the median of connections of the vertices in the graph
void findMedian(std::vector<std::pair<int, int>> &users)
{
    // print median
    cout << "The Median number of connections of users is: "
    << users[(users.size() - 1) / 2].second << " connections" << endl;
}

//***********************************************************************************************
//                         find Average
//
//***********************************************************************************************
//pre: menu function must exist to beable to load in the .csv data, vectors and loadData() and  generateUsers() and the BreadthFirstSearch() needs to exist to search for vertices in the graph
//post: We can now  calculate and display the average
void findAverage(std::vector<std::pair<int, int>> &users)
{
    //our average.
    size_t average = 0;

    //iterate vector of pairs add the sum of all the follower users count.
    for(size_t i = 0; i < users.size(); ++i)
    {
        average += users[i].second;
    }

    //find average and print
    cout << "The Average number of connections of users is: " << average / users.size() << " connections" << endl;
}
//***********************************************************************************************
//                         shortest Path Util
//
//***********************************************************************************************
//pre: menu function must exist to beable to load in the .csv data, vectors and loadData() and  generateUsers() and the BreadthFirstSearch() needs to exist to search for vertices in the graph
//post: We can now request and validate the user input for the shortest path option on the menu, then we will all the findShortestPath() to calculate the shortest path and display it
void shortestPathUtil(std::vector<std::list<int>> &theGraph)
{
    // source and destination and their index
    int sourceIndex, destIndex = 0;
    int source, dest;
    cout << "*******************************************************************" << endl;
    cout << "* A good couple to try for testing purposes is 9236 with 176616   *" << endl;
    cout << "*                        and                                      *" << endl;
    cout << "*                   1567 with 319779                              *" << endl;
    cout << "*******************************************************************\n" << endl;
    // input from user and the validation
    do
    {

        do{
                cout << "User1: ";
                cin >> source;
                cout << endl;
                if(cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cout << " * Please enter a user number *" << endl;
                }
           }while(cin.fail());
        //find index
        sourceIndex = indexOfUser(theGraph, source);

        //if not found
        if(sourceIndex == -1)
        {
            cout << " * User not found *\n" << endl;
        }
    }
    while(sourceIndex == -1);

    // input from user and the validation
    do
    {

        do{
                cout << "User2: ";
                cin >> dest;
                cout << endl;
                if(cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cout << " * Please enter a user number *" << endl;
                }
           }while(cin.fail());

        //find index
        destIndex = indexOfUser(theGraph, dest);

        //if not found
        if(destIndex == -1)
        {
            cout << " * User not found *\n" << endl;
        }

        // if same user in input for both
        else if(destIndex == sourceIndex)
        {
            cout << " * Please enter two different users *\n" << endl;
            destIndex = -1;
        }
    }
    while(destIndex == -1);

    //find the shortest path by calling the shortest path function
    if(sourceIndex != -1 && destIndex != -1)
    {
        findShortestPath(theGraph, sourceIndex, destIndex, theGraph.size());
    }
}
