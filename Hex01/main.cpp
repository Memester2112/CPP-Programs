#include <iostream>
#include<vector>
using namespace std;
const double PINF=999999999;

/*****************
Author: Aryendra Singh
Date: 16th June 2020
Implementation of a Hex Board using graphs
Here the graph used has nodes numbered from 0 to n^2-1 where n is the size of the Hex board
The user enters coordinates Ranging from (0,0) to (n-1,n-1)
To implement path finding we will have to have a heap class to form a minheap
And use Dijkstra's Algorithm:
******************/

typedef struct path//path is used as a return type in Dijkstra's shortest path(here the BFSW method)
{
    int* parent;//parent links to go back from g to s
    double pathLength;//total cost length of the path
    int length;//number of nodes including s and g
} path;

//...........................
typedef struct info
{
    double value;
    int nodeno;//used for delete min operation in heap
} info;
//##########################
//CLASS:HEAP:-MINHEAP implementation
///////////////////////////
class heap
{
public:
    heap():arr(0),node(0)
    {
        //initializer constructor
    }

    ////////////////////////
    void heapify();
    ////////////////////////
    heap(double* a,int* b,int n)//An array with n values is sent
    //a is the array with the values used to fill arr
    //b is the array with the node numbers used to fill node
    //arr[0] is used to store the number of heap elements
    {
        arr.push_back(0);//current size
        node.push_back(0);//stores nothing
        for(int i=1; i<=n; i++)
        {
            arr.push_back(a[i-1]);
            node.push_back(b[i-1]);
            arr[0]=arr[0]+1;//increasing size
            heapify();
        }
    }
    ///////////////////////
    info delMin();
    ///////////////////////
    void addNode(double val,int n);//val is the cost and node number
    ///////////////////////
    void reOrder(int n,double val);// n is the node number
    ///////////////////////
    vector<double>& getHeap()
    {
        return arr;
    }
    //////////////////////
private:
    vector<double> arr;//stores the value of the node in question
    vector<int> node;//Stores the id number of the node in question

};
/////////////////////////////
void heap::addNode(double val,int n)//val goes in arr and n goes in node
{
    if(arr.size()==0)
    {
        arr.push_back(0);//The size of heap is initialized
        node.push_back(0);//0th Index doesn't store anything
    }
    arr[0]=arr[0]+1;
    arr.push_back(val);
    node.push_back(n);
    heapify();
}
////////////////////////////
void heap::reOrder(int n,double val)//n is the node No.,val is the new value of this node n
{
    //reOrder only works when the value is replaced by a smaller value
    //In our PQ requirement we use it for Djikstra's Implementation
    //Thus we can rest easy
    int k;//array index of node n
    for(int i=1; i<=static_cast<int>(arr[0]); i++)
    {
        if(node[i]==n)
        {
            k=i;
            break;
        }
    }
    arr[k]=val;//value changed,now needs to be rearranged
    int temp,p,c;//p=parent node,c=current node
    double tempd;
    c=k;
    p=c/2;
    while(p>=1)
    {
        if(arr[c]<arr[p])
        {
            tempd=arr[c];
            arr[c]=arr[p];
            arr[p]=tempd;
            //having swapped arr we need to swap node also
            temp=node[c];
            node[c]=node[p];
            node[p]=temp;

            c=p;
            p=c/2;
        }
        else
        {
            break;
        }
    }

}
///////////////////////////
info heap::delMin()
{
    int temp;
    double tempd;
    info x;
    x.value=arr[1];//value to be returned
    x.nodeno=node[1];//node number to be returned

    arr[1]=arr[static_cast<int>(arr[0])];//Empty space now filled with the last value which will trickle down
    node[1]=node[static_cast<int>(arr[0])];
    arr[0]=arr[0]-1;//size of vector decreased.
    arr.pop_back();//size actually decreased
    node.pop_back();

    int lc,rc,wc,c;//left child and right child,wc=which child is smaller,c=current node
    c=1;
    lc=2;
    rc=3;
    while(lc<=static_cast<int>(arr[0]))//at least a left child exists
    {
        wc=lc;
        if(rc<=static_cast<int>(arr[0]))//right child also exists
        {
            if(arr[rc]<arr[lc])
            {
                wc=rc;
            }
        }
        if(arr[c]>arr[wc])
        {
            tempd=arr[c];
            arr[c]=arr[wc];
            arr[wc]=tempd;
            //having swapped arr we need to so the same operations on node[]
            temp=node[c];
            node[c]=node[wc];
            node[wc]=temp;

            c=wc;
            lc=2*c;
            rc=lc+1;
        }
        else
            break;

    }

    return x;
}

void heap::heapify()
{
    int p,c,temp;//p is parent node and c is current node
    double tempd;
    c=static_cast<int>(arr[0]);
    p=c/2;
    while(p>=1)
    {
        if(arr[c]<arr[p])
        {
            //cout<<"Swapped"<<endl;
            tempd=arr[c];
            arr[c]=arr[p];
            arr[p]=tempd;
            //having swapped arr we need to swap node also
            temp=node[c];
            node[c]=node[p];
            node[p]=temp;

            c=p;
            p=c/2;
        }
        else
            break;
    }
}

//Methods for HEAP over now
//##########################


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
        graph=new double*[n*n];
        for(int i=0; i<n*n; i++)
        {
            graph[i]=new double[n*n];
        }
        for(int i=0; i<n*n; i++)
        {
            for(int j=0; j<n*n; j++)
            {
                graph[i][j]=0;
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
        addEdge(0,1);addEdge(0,n);//NW corner
        addEdge(n*n-1,n*n-2);addEdge(n*n-1,n*n-n-1);//SE corner
        addEdge(n-1,n-2);addEdge(n-1,2*n-1);addEdge(n-1,2*n-2); //NE Corner
        addEdge(n*n-n,n*n-n+1);addEdge(n*n-n,n*n-2*n);addEdge(n*n-n,n*n-2*n+1);//SW Corner
        //Corner Edges done, Now remaining Border tiles with 4 edges each
        //First the north Border with node numbers [1,n-2]
        for(int i=1;i<=n-2;i++)
        {
            addEdge(i,i-1);
            addEdge(i,i+1);
            addEdge(i,i+n);
            addEdge(i,i+n-1);
        }
        //South Border with node numbers [n*n-n+1,n*n-2]
        for(int i=n*n-n+1;i<=n*n-2;i++)
        {
            addEdge(i,i-1);
            addEdge(i,i+1);
            addEdge(i,i-n);
            addEdge(i,i-n+1);
        }
        //East Border [n,n*n-2n] in steps of n
        for(int i=n;i<=n*n-2*n;i=i+n)
        {
            addEdge(i,i-n);
            addEdge(i,i+n);
            addEdge(i,i-1);
            addEdge(i,i+n-1);
        }
        //West Border [2*n-1,n*n-n-1] in steps of n
        for(int i=2*n-1;i<=n*n-n-1;i=i+n)
        {
            addEdge(i,i-n);
            addEdge(i,i+n);
            addEdge(i,i+1);
            addEdge(i,i-n+1);
        }
        int z=0;//z=x*n+y, the requisite node number
        //Now the Central nodes
        for(int x=1;x<=n-2;x++)
        {
            for(int y=1;y<=n-2;y++)
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
        graph[node1][node2]=graph[node2][node1]=1.0;
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
    double** getgraph()
    {
        return graph;
    }//getter function for 2D graph
    /////////////////////////////////////
    path BFSW(int s,int g,char z);
    /////////////////////////////////////
    void prnHex();
    /////////////////////////////////////
    void playHex();
private:
    int n;//size of the Hex, like 7 or 11, the number of nodes will be n^2
    double** graph;//cost array from node to node, will have
    char* c;//stores the color of the nodes Will have size n
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
    int N[n];int S[n]; int E[n]; int W[n];
    /**To keep track of the X's in the top and bottom row N,S are used
    To do the same for O's in the left and right columns E,W are used
    If an X is found in the top row, then we check all possible paths from
    all top X's to all bottom X's. If we find one then X has one
    Likewise for O's path from
    **/
    for(int i=0;i<n;i++)
    {
        N[i]=S[i]=W[i]=E[i]=0;//initiall
    }
    path mypath;
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
        if(x==n-1&&z=='X')
            S[y]=1;
        if(y==0&&z=='O')
            W[x]=1;
        if(y==n-1&&z=='O')
            E[x]=1;
        //After each turn, will have to check for paths from Top to Bottom
        for(int i=0;i<n;i++)//goes over values of N
        {
            if(N[i])//top tile is X
            {
                for(int j=0;j<n;j++)
                {
                    if(S[j])//bottom tile is also X, then possibility of a path exists
                    {
                        mypath = BFSW(i,(n-1)*n+j,z);
                        if(mypath.pathLength!=PINF)
                        {
                            prnHex();
                            cout<<"\nX WINS!\n";
                            cout<<"PathLength = "<<mypath.pathLength;
                            return;
                        }
                    }
                }
            }
        }
        //After each turn, will have to check for paths from Left to Right
        for(int i=0;i<n;i++)//goes over values of W
        {
            if(W[i])//left tile is O
            {
                for(int j=0;j<n;j++)
                {
                    if(E[j])//Right tile is also O, then possibility of a path exists
                    {
                        mypath = BFSW(i*n,j*n+n-1,z);
                        if(mypath.pathLength!=PINF)
                        {
                            prnHex();
                            cout<<"\nO WINS!\n";
                            cout<<"PathLength = "<<mypath.pathLength;
                            return;
                        }
                    }
                }
            }
        }

        k++;
        prnHex();

        cout<<"Enter next coordinates:\n";
        cin>>x>>y;
    }
    cout<<"Game ended\n";
}
///////////////////////////////////////////////////
path Graph::BFSW(int s,int g,char z)//s is the starting node and g is the goal node,z is the type of node we
{                                   //we want in our path, either X or O
    int n1=n*n;//Number of nodes
    //n is the Size of Hex board
    int inQueue[n1];//maintains if the node is currently in the queue or not
    double cost[n1];
    int visited[n1];
    //initializing cost array and inQueue array and visited array
    //mypath is the return variable;
    path mypath;
    mypath.length=0;
    mypath.parent=new int[n1];
    mypath.pathLength=0;

    for(int i=0; i<n1; i++)
    {
        cost[i]=PINF;
        inQueue[i]=0;
        visited[i]=0;
        mypath.parent[i]=-1;
    }
    //minheap used as a priority queue
    heap pq;//priority queue
    cost[s]=0;
    pq.addNode(0,s);//add as (cost,node no)
    inQueue[s]=1;
    info j;//for delMin operation
    int terminate=1;//terminate = 1 if the process terminates due to empty queue
    while(pq.getHeap()[0]>0)
    {
        j=pq.delMin();
        visited[j.nodeno]=1;
        inQueue[j.nodeno]=0;
        if(j.nodeno==g)
        {
            mypath.pathLength=cost[j.nodeno];
            terminate=0;
            break;
        }

        for(int i=0; i<n1; i++) //finding all successors of j with same color
        {
            if(i!=j.nodeno&&graph[j.nodeno][i]!=0&&c[i]==z)//edge exists to a node with color 'z', either X or O
            {
                if(visited[i]==0)
                {
                    if(cost[i]>cost[j.nodeno]+graph[j.nodeno][i])
                    {
                        cost[i]=cost[j.nodeno]+graph[j.nodeno][i];
                        mypath.parent[i]=j.nodeno;
                        if(inQueue[i]==0)
                        {
                            pq.addNode(cost[i],i);//node not present in the queue is inserted
                            inQueue[i]=1;//now the node is in the queue
                        }
                        else//node already present in the queue,has to be reordered
                        {
                            pq.reOrder(i,cost[i]);
                        }
                    }
                }
            }
        }//for ends
    }//while ends

    if(pq.getHeap()[0]==0&&terminate)//The queue has terminated due to being empty
    {
        mypath.pathLength=PINF;
        //indicates no solution
    }
    return mypath;

}//returns the length of this shortest path from s to g
//returns the path from s to g as s-v1-v2...-g
////////////////////////////////////////////////////////////////

int main()
{
    cout << "Enter Board Size:" << endl;
    int n;
    cin>>n;
    Graph G(n);
    G.prnHex();
    G.playHex();
    return 0;
}
