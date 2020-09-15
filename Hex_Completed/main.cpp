#include <iostream>
#include<vector>
#include<random>
#include<chrono>
#include<algorithm>
using namespace std;


/*****************
Author: Aryendra Singh
Date: 24th June 2020
Implementation of a Hex Board using graphs
Here the graph used has nodes numbered from 0 to n^2-1 where n is the size of the Hex board
The user enters coordinates Ranging from (0,0) to (n-1,n-1)
To implement path finding we will use Breadth First search instead of Dijkstra's Algorithm
It is optimal for our case of undirected unweighted graph(the weight is 1 or 0), i.e
either the edge exists or it does not exist
******************/

class Graph
{
public:
    Graph():n(0),graph(NULL)
    {
        //initializer constructor
    }
    //////////////////////////////////////////////////////////
    Graph(int sizeHex)
    {
        //constructor initializes a graph with n nodes and no edges added.
        //to add edges use the addEdge function
        n=sizeHex;
        graph=new bool*[n*n];
        for(int i=0; i<n*n; i++)
        {
            graph[i]=new bool[n*n];
        }
        for(int i=0; i<n*n; i++)
        {
            for(int j=0; j<n*n; j++)
            {
                graph[i][j]=false;
            }
        }
        c=new char[n*n];//Char of correct size declared
        //In our game of Hex: 'X' denotes Player 1, 'O' denotes player 2 and '.' denotes empty space
        for(int i=0; i<n*n; i++)
        {
            c[i]='.';
        }
        //Now we will form the requisite edge connections use setEdge
        //Starting with the 4 corners
        addEdge(0,1);
        addEdge(0,n);//NW corner
        addEdge(n*n-1,n*n-2);
        addEdge(n*n-1,n*n-1-n);//SE corner
        addEdge(n-1,n-2);
        addEdge(n-1,2*n-1);
        addEdge(n-1,2*n-2); //NE Corner
        addEdge(n*n-n,n*n-n+1);
        addEdge(n*n-n,n*n-2*n);
        addEdge(n*n-n,n*n-2*n+1);//SW Corner
        //Corner Edges done, Now remaining Border tiles with 4 edges each
        //First the north Border with node numbers [1,n-2]
        for(int i=1; i<=n-2; i++)
        {
            addEdge(i,i-1);
            addEdge(i,i+1);
            addEdge(i,i+n);
            addEdge(i,i+n-1);
        }
        //South Border with node numbers [n*n-n+1,n*n-2]
        for(int i=n*n-n+1; i<=n*n-2; i++)
        {
            addEdge(i,i-1);
            addEdge(i,i+1);
            addEdge(i,i-n);
            addEdge(i,i-n+1);
        }
        //East Border [n,n*n-2n] in steps of n
        for(int i=2*n-1; i<=n*n-n-1; i=i+n)
        {
            addEdge(i,i-n);
            addEdge(i,i+n);
            addEdge(i,i-1);
            addEdge(i,i+n-1);
        }
        //West Border [2*n-1,n*n-n-1] in steps of n

        for(int i=n; i<=n*n-2*n; i=i+n)
        {
            addEdge(i,i-n);
            addEdge(i,i+n);
            addEdge(i,i+1);
            addEdge(i,i-n+1);
        }
        int z=0;//z=x*n+y, the requisite node number
        //Now the Central nodes
        for(int x=1; x<=n-2; x++)
        {
            for(int y=1; y<=n-2; y++)
            {
                z=x*n+y;
                addEdge(z,z+1);
                addEdge(z,z-1);
                addEdge(z,z+n);
                addEdge(z,z-n);
                addEdge(z,z-n+1);
                addEdge(z,z+n-1);

            }
        }
        //All edges have been connected
    }
    /////////////////////////////////////
    void addEdge(int node1,int node2)//Adds edges assuming the cost to be 1,useful in our Hex program
    {
        graph[node1][node2]=graph[node2][node1]=true;
    }
    /////////////////////////////////////
    void setColor(int x,char d)//x is the position and d is the color
    {
        c[x]=d;
    }
    int getn()
    {
        return n;//returns the value of n
    }// a getter function
    /////////////////////////////////////
    bool** getgraph()
    {
        return graph;
    }//getter function for 2D graph
    /////////////////////////////////////
    bool BFS(int s,int g);//returns true if there is a path from node s to g
    //
    void prnHex();
    /////////////////////////////////////
    void playHex();
    ////////////////////////////////////
    void playHexAi();
    int ai_move(char t);//provides the node number in which the ai should move next
private:
    int n;//size of the Hex, like 7 or 11, the number of nodes will be n^2
    bool** graph;//cost array from node to node, will have 121*121 representation, necessary for edge formation
    char* c;//stores the color of the nodes Will have size 121
};
//////////////////////////////////////
void Graph::prnHex()
{
    int k=0;
    cout<<"\n";
    for(int i=0; i<2*n-1; i++) //Number of lines is 2n-1
    {
        for(int j=0; j<i; j++) //requisite number of leading spaces
        {
            cout<<" ";
        }
        if(i%2==0)//Prints .-.-.-.-.-
        {
            for(int j=0; j<n-1; j++)
            {
                cout<<c[k++]<<" - ";
            }
            cout<<c[k++]<<endl;
        }
        else
        {
            for(int j=0; j<n-1; j++)
            {
                cout<<"\\ / ";
            }
            cout<<"\\"<<endl;
        }

    }
}

void Graph::playHex()
{
    cout<<"Player 1 is X and Player 2 is O\n"
        <<"Player 1 (X) forms a connected path from top to bottom (North to South)\n"
        <<"Player 2 (O) forms a connected path from left to right\n";
    cout<<"Choose your player and then start playing\n";
    cout<<"You can enter your move as a tuple like :(x,y) as x y\n"
        <<"The range is from (0,0) to "<<"("<<n-1<<","<<n-1<<")\n"
        <<"Enter a move coordinate with X going first\n"
        <<"To stop entering moves enter x = -1\n";
    int k=0;//keep track of whose turn it is
    int x,y;
    char z;//Which one of X or O chance to play
    cin>>x>>y;
    int N[n];
    int S[n];
    int E[n];
    int W[n];
    /**To keep track of the X's in the top and bottom row N,S are used
    To do the same for O's in the left and right columns E,W are used
    If an X is found in the top row, then we check all possible paths from
    all top X's to all bottom X's. If we find one then X has one
    Likewise for O's path from
    **/
    for(int i=0; i<n; i++)
    {
        N[i]=0;
        S[i]=0;
        W[i]=0;
        E[i]=0;//initiate all
    }

    while(x!=-1)
    {
        while(x<0||x>n-1||y<0||y>n-1||c[x*n+y]!='.')
        {
            cout<<"Coordinates out of range or already occupied\n"
                <<"Enter coordinates between (0,0) and "<<"("<<n-1<<","<<n-1<<")\n";
            cin>>x>>y;
            if(x==-1)
                break;
        }
        if(x==-1)
            break;

        if(k%2==0)
            z='X';
        else
            z='O';
        c[x*n+y]=z;//Character updated
        ///////////Setting Border flags
        if(x==0&&z=='X')
            N[y]=1;
        if(x==(n-1)&&z=='X')
            S[y]=1;
        if(y==0&&z=='O')
            W[x]=1;
        if(y==(n-1)&&z=='O')
            E[x]=1;
        //After each turn, will have to check for paths from Top to Bottom
        if(z=='X')
        {
            for(int i=0; i<n; i++) //goes over values of N
            {
                if(N[i])//top tile is X
                {
                    for(int j=0; j<n; j++)
                    {
                        if(S[j])//bottom tile is also X, then possibility of a path exists
                        {
                            if(BFS(i,(n-1)*n+j))
                            {
                                prnHex();
                                cout<<"\nX WINS!\n";
                                return;
                            }
                        }
                    }
                }
            }
        }
        //After each turn, will have to check for paths from Left to Right
        else
        {
            for(int i=0; i<n; i++) //goes over values of W
            {
                if(W[i])//left tile is O
                {
                    for(int j=0; j<n; j++)
                    {
                        if(E[j])//Right tile is also O, then possibility of a path exists
                        {
                            if(BFS(i*n,j*n+n-1))
                            {
                                prnHex();
                                cout<<"\nO WINS!\n";
                                return;
                            }
                        }
                    }
                }
            }
        }

        k++;//keeping track whose turn it is, increment to give the turn to the other player
        prnHex();

        cout<<"Enter next coordinates:\n";
        cin>>x>>y;
    }
    cout<<"Game ended\n";
}
///////////////////////////////////////////////////
bool Graph::BFS(int s,int g)
{
    char z=c[s];//all nodes have to be matched to this color
    int n1=n*n;//n=11,n1=121
    //n1 is the actual number of nodes in the graph
    int visited[n1];
    int p=0,r=0;
    //pointer as to where we are in the queue,BFS uses a queue and since the max number of elements ever to be in a n1
    //we use a lazy implementation of the queue data structure: as an array of length n1, pointed at index = p
    //p points to the beginning of the queue and
    int que[n1];//apparently queue is a keyword, so que is used
    for(int i=0; i<n1; i++)
    {
        visited[i]=0;
        que[i]=0;
    }//initializing

    int j=0;

    que[r]=s;
    r++;//on adding an element to the front we keep the front ptr same and increment the back one;
    while(p!=r)//while queue is not empty
    {
        j=que[p];//Dequeueing
        p++;

        if(j==g)
            return true;
        if(visited[j]==0)
        {
            visited[j]=1;//marked as visited
            for(int i=0; i<n1; i++) //goes over all possible successors of the node j
            {
                if(graph[j][i]&&c[i]==z)//edge exists and the color matches
                {
                    que[r]=i;
                    r++;
                    //enqueue
                }
            }
        }
    }
    return false;
}
///////////////////////////////////////////////////////////

int Graph::ai_move(char t)//t is the AI's color,either X or O
{
    Graph G1(n);

    char z;//z is the human's color
    if(t=='X')
        z='O';
    else
        z='X';

    int bestMove=-1;
    //current board is in bool** graph
    //the stones information, whether X or O or . is in c
    //we will use this c to initialize our vector containing node numbers for which the moves need to be played
    //the already occupied places do not have to be replaced.
    vector<int> stones;
    int n1=n*n;
    for(int i=0; i<n1; i++)
    {
        stones.push_back(i);//vec of [0,1,2...120] if n=11
    }
    //This is to generate the random shuffled arrangement we need to make the board
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    auto rng = default_random_engine {seed};
    shuffle(begin(stones), end(stones), rng);

    int testCounter=0;

    int flag=0;

    //checking all legal moves, we store the number of runs for each starting point as a variable;
    int k=0;//keeps track of whose turn it is in the simulation
    const int runs=1000;
    int wins=0;//counter to keep track of number of wins
    int maxWins=-1;//keeps track of the maximum wins achieved in any run
    int xWins=0;//Counter of wins by X
    for(int p=0; p<n1; p++)//position p being evaluated as a move
    {
        if(c[p]=='.')//means that the space is empty and we can make a legal move:
        {
            xWins=0;
            wins=0;
            for(int q=0; q<runs; q++)//starts run to evaluate position p
            {
                for(int i=0; i<n1; i++)
                {
                    G1.c[i]=c[i];
                }//now we have a copy of the data we need to work on, we will have to copy this anew for every run of the simulation
                G1.c[p]=t;//possible move by the ai
                shuffle(begin(stones), end(stones), rng);//shuffled to fill up the board
                k=0;//human moves when k%2==0, ai otherwise
                //now to fill the board
                for(int r=0; r<n1; r++)
                {
                    if(c[stones[r]]=='.'&&stones[r]!=p)//the tile is empty and not the one we are evaluating(as that has been already filled)
                    {
                        if(k%2==0)
                            G1.c[stones[r]]=z;
                        else
                            G1.c[stones[r]]=t;
                        k++;
                    }
                }//board filled up

                //G1.prnHex();
                testCounter++;
                flag=0;

                //now determine who won
                //for X to have won there should be at least one path from some node[0,n-1] to one of the bottom nodes[n1-n,n1-1]
                for(int i=0; i<n; i++) //top row node numbers
                {
                    if(G1.c[i]=='X')
                    {
                        for(int j=n1-n; j<n1; j++) //bottom row node numbers
                        {
                            if(G1.c[j]=='X')//possible path may exist
                            {
                                if(G1.BFS(i,j))
                                {
                                    flag=1;//There might be many paths on the same board through which X wins, they have to be counted as one victory though
                                    break;
                                }
                            }
                        }
                    }
                    if(flag)
                        break;
                }
                if(flag)
                    xWins++;

            }//end of simulations for move 'p', iterated over 'runs' number of times
            if(t=='X')
                wins=xWins;
            else
                wins=runs-xWins;

            if(wins>maxWins)
            {
                maxWins=wins;
                bestMove=p;
            }
            //cout<<"maxWins = "<<maxWins<<endl;
        }//if block of checking if this is a valid move for the next turn

    }//for loop going over each node

    cout<<"\nNo. of boards checked for this move = "<<testCounter<<"\n";
    return bestMove;
}
///////////////////////////////////////////////////////////

void Graph::playHexAi()
{
    cout<<"Player 1 is X and Player 2 is O\n"
        <<"Player 1 (X) forms a connected path from top to bottom (North to South)\n"
        <<"Player 2 (O) forms a connected path from left to right\n";
    cout<<"Choose your player and then start playing\n";
    cout<<"You can enter your move as a tuple like :(x,y) as x y\n"
        <<"The range is from (0,0) to "<<"("<<n-1<<","<<n-1<<")\n"
        <<"Enter a move coordinate with X going first\n"
        <<"To stop entering moves enter x = -1, y= anything\n"
        <<"The Pie rule applies here: Suppose you choose to go first\n"
        <<"and make a very good move, then Player 2 can decide that they\n"
        <<"want to be Player 1 and thus Player 1 will become player 2\n";
    /*********************/
    int N[n];
    int S[n];
    int E[n];
    int W[n];
    /**To keep track of the X's in the top and bottom row N,S are used
    To do the same for O's in the left and right columns E,W are used
    If an X is found in the top row, then we check all possible paths from
    all top X's to all bottom X's. If we find one then X has one
    Likewise for O's path from
    **/
    for(int i=0; i<n; i++)
    {
        N[i]=0;
        S[i]=0;
        W[i]=0;
        E[i]=0;//initiate all
    }
    char t,z;//t will be the Ai's color and z will be the Human's color
    cout<<"Press X to be player 1 and O to be player 2\n";
    cin>>z;
    while(z!='X'&&z!='O')
    {
        cout<<"\nWrong Input: Enter X or O :\n";
        cin>>z;
    }
    int bestMove,x,y;
    if(z=='X')
        t='O';
    else
        t='X';
    if(z=='X')//AI has to evaluate whether to switch players or not
    {
        cout<<"Enter your move as X:, Remember the PIE rule\n";
        cin>>x>>y;
        while(x<0||x>n-1||y<0||y>n-1)
        {
            cout<<"Input out of range: Enter again :\n";
            cin>>x>>y;
        }
        c[x*n+y]=z;
        prnHex();
        cout<<"\nAI is now deciding whether or not to swap:\n";
        bestMove = ai_move(t);
        if(bestMove==(x*n+y)||(bestMove-1)==(x*n+y)||(bestMove+1)==(x*n+y)||(bestMove-n)==(x*n+y)||(bestMove+n)==(x*n+y)
           ||(bestMove+n+1)==(x*n+y)||(bestMove+n-1)==(x*n+y)||(bestMove-n+1)==(x*n+y)||(bestMove-n-1)==(x*n+y))
        //the human has made a very good move,AI decides to switch
        {
            t='X';//AI Chooses to become the first player
            z='O';//Human becomes the second player
            cout<<"\nAI HAS CHOSEN TO SWAP\nNOW YOU ARE PLAYER 2, PLAYING AS O\n"
                <<"Enter your move as Player 2 now:\n";
            cin>>x>>y;
            while(x<0||x>n-1||y<0||y>n-1||c[(x*n+y)]!='.')
            {
                cout<<"Input out of range or space already occupied: Enter again :\n";
                cin>>x>>y;
            }
            c[x*n+y]=z;
            prnHex();
        }
        else//if the AI does not choose to switch then
        {
            cout<<"\nAI has chosen NOT to switch\n";
            bestMove=ai_move(t);
            cout<<"\nAI Moved = "<<(bestMove/n)<<" "<<(bestMove%n);
            c[bestMove]=t;
            prnHex();
        }
    }

    else//human had chosen player 2,AI has to play first move
    {
        cout<<"AI is calculating its first move...\n";
        bestMove=ai_move(t);
        c[bestMove]=t;
        cout<<"\nAI Moved = "<<(bestMove/n)<<" "<<(bestMove%n);
        prnHex();
        cout<<"Would you like to be the first player?\nEnter 1 for yes, 0 for no\n";
        int e;
        cin>>e;
        if(e==1)
        {
            cout<<"Players Swapped\nAI will now play its turn as player 2\n";
            t='O';
            z='X';
            bestMove=ai_move(t);
            c[bestMove]=t;
            cout<<"\nAI Moved = "<<(bestMove/n)<<" "<<(bestMove%n);
            prnHex();
        }
        else
        {
            cout<<"\nYou have chosen not to switch\nPlay your turn as player 2\n";
            cin>>x>>y;
            while(x<0||x>n-1||y<0||y>n-1||c[(x*n+y)]!='.')
            {
                cout<<"Input out of range or space already occupied: Enter again :\n";
                cin>>x>>y;
            }
            c[x*n+y]=z;
            prnHex();

        }

    }
    //Now 2 turns have been played
    //we have not checked for N,E,W or S for the above 2 turns: before playing further we must do that:
    for(x=0;x<n;x++)
    {
        for(y=0;y<n;y++)
        {
            if(c[x*n+y]=='X')
            {
                if(x==0)
                    N[y]=1;
                if(x==(n-1))
                    S[y]=1;
            }
            else if(c[x*n+y]=='O')
            {
                if(y==0)
                    W[x]=1;
                if(y==(n-1))
                    E[x]=1;
            }
        }
    }

    //If at the end of this pie dependent turn, the player has X then its now his turn, otherwise the ai will go now

    char turn;
    int k=0;//when k is zero, its X's turn to move
    x=0;
    while(x!=-1)
    {
        if(x==-1)
            break;

        if(k%2==0)//X's turn to move
        {
            turn='X';
            if(z=='X')//the human is playing as X, then take input:
            {
                cout<<"\nEnter your move as X:\n";
                cin>>x>>y;
                while(x<0||x>n-1||y<0||y>n-1||c[x*n+y]!='.')
                {
                    cout<<"Coordinates out of range or already occupied\n"
                        <<"Enter coordinates between (0,0) and "<<"("<<n-1<<","<<n-1<<")\n";
                    cin>>x>>y;
                }
                c[x*n+y]='X';
            }
            else//ai is playing as X, feed in move automatically
            {
                cout<<"\nAI will now play,please wait for 20 seconds...\n";
                bestMove=ai_move(t);
                c[bestMove]='X';
                x=bestMove/n;
                y=bestMove%n;
                cout<<"\nAI Moved = "<<(x)<<" "<<(y);

            }
        }

        else//It is O's turn
        {
            turn='O';
            if(z=='O')//the human is playing as O, then take input:
            {
                cout<<"\nEnter your move as O:\n";
                cin>>x>>y;
                while(x<0||x>n-1||y<0||y>n-1||c[x*n+y]!='.')
                {
                    cout<<"Coordinates out of range or already occupied\n"
                        <<"Enter coordinates between (0,0) and "<<"("<<n-1<<","<<n-1<<")\n";
                    cin>>x>>y;
                }
                c[x*n+y]='O';
            }
            else//ai is playing as O, feed in move automatically
            {
                cout<<"\nAI will now play,please wait for 20 seconds...\n";
                bestMove=ai_move(t);
                c[bestMove]='O';
                x=bestMove/n;
                y=bestMove%n;
                cout<<"\nAI Moved = "<<(x)<<" "<<(y);
            }
        }
        prnHex();
        k++;//keeping track whose turn it is, increment to give the turn to the other player


        ///////////Setting Border flags
        if(x==0&&turn=='X')
            N[y]=1;
        if(x==(n-1)&&turn=='X')
            S[y]=1;
        if(y==0&&turn=='O')
            W[x]=1;
        if(y==(n-1)&&turn=='O')
            E[x]=1;
        //After each X turn, will have to check for paths from Top to Bottom
        if(turn=='X')
        {
            for(int i=0; i<n; i++) //goes over values of N
            {
                if(N[i])//top tile is X
                {
                    for(int j=0; j<n; j++)
                    {
                        if(S[j])//bottom tile is also X, then possibility of a path exists
                        {

                            if(BFS(i,(n-1)*n+j))
                            {
                                prnHex();
                                cout<<"\nX WINS!\n";
                                return;
                            }
                        }
                    }
                }
            }
        }
        //After each O turn, will have to check for paths from Left to Right
        else
        {
            for(int i=0; i<n; i++) //goes over values of W
            {
                if(W[i])//left tile is O
                {
                    for(int j=0; j<n; j++)
                    {
                        if(E[j])//Right tile is also O, then possibility of a path exists
                        {
                            if(BFS(i*n,j*n+n-1))
                            {
                                prnHex();
                                cout<<"\nO WINS!\n";
                                return;
                            }
                        }
                    }
                }
            }
        }



    }
    cout<<"Game ended\n";




}
int main()
{
    cout << "Enter Board Size:" << endl;
    int n;
    cin>>n;
    Graph G(n);
    G.prnHex();
    cout<<"Enter 1 to play Human vs Human or enter 0 to play Human vs AI\n";
    int choice;
    cin>>choice;
    while(choice!=1&&choice!=0)
    {
        cout<<"Wrong Input: Enter 1 to play Human vs Human or enter 0 to play Human vs AI\n";
        cin>>choice;
    }
    if(choice)
    {
        G.playHex();
    }
    else
    {
        G.playHexAi();
    }
    return 0;
}
