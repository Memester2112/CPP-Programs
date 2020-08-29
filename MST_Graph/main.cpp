#include <iostream>
#include<vector>
#include<fstream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<iomanip>
/********************************************
Author- Aryendra Singh
Date- 6th June 2020
Minimum spanning tree via Prim's Method
Note that this does not use Fibonacci Heap structures

********************************************/

/*****************************************
Class nodeelem replaces the use of a struct
It is used to create an adjacency list representation of the graph
[cost|node no]->[cost|node no]...->NULL
[cost|node no]->[cost|node no]...->NULL
[cost|node no]->[cost|node no]...->NULL
[cost|node no]->[cost|node no]...->NULL
[cost|node no]->[cost|node no]...->NULL
A vector of type nodeelem is used in Graph to create adjacency list representation
struct info is used as return type for the MST method
NOTE : NODES ARE NUMBERED FROM 0 TO N-1, NOT 1 TO N
*****************************************/
////////////////////////////////////////
using namespace std;
const int PINF=99999999;
///////////////////////////////////////
typedef struct info
{
    int* n1;//edges are defined as: FROM node n1 TO node n2
    int* n2;//The array size will be n-1 where n is the number of nodes
    double mst_cost;//Total cost of the mst
}info;
//////////////////////////////////////////////
class nodeelem//node element
{
public:
    //constructor
    nodeelem():nodeno(0),cost(PINF),next(NULL)
    {
        //initializer class
    }
    //parameterized constructor
    nodeelem(int nodenumber,double cost):nodeno(nodenumber),cost(cost),next(NULL)
    {
        //initializer constructor
    }

    //getter functions
    int getNodeNo()
    {
        return nodeno;
    }
    double getCost()
    {
        return cost;
    }
    nodeelem* getNext()
    {
        return next;
    }

    //mutator functions
    void setNodeNo(int n)
    {
        nodeno=n;
        return;
    }
    void setCost(double n)
    {
        cost=n;
        return;
    }
    void setNext(nodeelem* n)
    {
        next=n;
    }

    //destructor
    ~nodeelem()
    {

    }

private:
    int nodeno;//Arbitrary number assigned to the node
    double cost;//From the parent node in the list to this node: cost
    nodeelem* next;//to point to the next in row
};
//////////////////////////////////////////
class Graph
{
public:
    Graph():n(0),graph(NULL)
    {
        //initializer constructor
    }
    //////////////////////////////////////////////////////////
    Graph(int sizegraph)
    {
        //constructor initializes a graph with n nodes and no edges added.
        //to add edges use the addEdge function
        n=sizegraph;
        graph=new double*[n];
        for(int i=0;i<n;i++)
        {
            graph[i]=new double[n];
        }
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                graph[i][j]=0;
            }
        }
    }
    //////////////////////////////////////////////
    void addEdge(int node1,int node2,double cost)
    {
        graph[node1][node2]=graph[node2][node1]=cost;
    }
    ///////////////////////////////////
    Graph(string pathname)//constructor that takes in file name and creates graph
    {
        ifstream fin;
        fin.open(pathname.c_str());
        cout<<pathname.c_str()<<endl;
        int node1,node2,cost;
        if(fin.is_open())
        {
            fin>>n;
            cout<<"Size of graph = "<<n<<endl;
            graph=new double*[n];
            for(int i=0; i<n; i++)
            {
                graph[i]=new double[n];
            }
            for(int i=0; i<n; i++)
            {
                for(int j=0; j<n; j++)
                {
                    graph[i][j]=0;
                }
            }
            while(!fin.eof())
            {
                fin>>node1>>node2>>cost;
                cout<<"("<<node1<<","<<node2<<") :"<<cost<<endl;
                graph[node1][node2]=graph[node2][node1]=static_cast<double>(cost);
            }
        }
        else
        {
            cout<<"Could not open\n";

        }
        fin.close();
    }
    ///////////////////////////////////
    int getn()
    {
        return n;//returns the value of n
    }// a getter function
    ///////////////////////////////////
    double** getgraph()
    {
        return graph;
    }//getter function for 2D graph
    /////////////////////////////////////
    double** RGG(int n,double p,double cost1,double cost2);//n is the number of nodes in the graph
    ///////////////////////////////////
    void prngraph();
    /////////////////////////////////////
    void createAdj();//creates the adjacency list
    ///////////////////////////////////
    void adjPrint();//prints the adjacency list
    ///////////////////////////////////
    info MST();
private:
    int n;//size of graph
    double** graph;//cost array from node to node
    vector<nodeelem> adjgraph;//adjacency list representation
};
//////////////////////////////////////////////
info Graph::MST()
{
    //initialize the return type first
    info mypath;
    mypath.n1=new int[n-1];
    mypath.n2=new int[n-1];
    mypath.mst_cost=0;

    int p[n];//p is a look up for the nodes which have already been included in the mst
    //p[i]=0 if the node[i] is not in the mst and p[i]=1 if it is
    int q[n];//q is the look table which sequentially goes on storing the nodes being added
    // to the mst. Helps in determining the edges that need to be checked per iteration

    //initializing p and q
    for(int i=0;i<n;i++)
    {
        p[i]=0;
        q[i]=-1;
    }
    //start with node 0
    mypath.n1[0]=0;
    p[0]=1;//node 0 has been included in the mst
    q[0]=0;//node 0 is the first node in the mst
    nodeelem* temp=NULL;
    int j=0;
    double minc=PINF;//current minimum cost edge, will be updated
    for(int i=0;i<n-1;i++)//process is repeated for n-1 times to get n-1 edges
    {
        j=0;
        while(q[j]!=-1)//goes over the edges of the nodes already in the mst
        {
            temp=&adjgraph[q[j]];//pointer to one of the chains that need to be checked.
            temp=(*temp).getNext();//So that a node already in the mst is not checked
            //now for the node q[j](already in the mst), examine all its edges
            while(temp!=NULL)//goes till the end of the list,all edges examined
            {
                if( p[(*temp).getNodeNo()]==0 )//means that the node to which the edge is going to is not already in the mst
                {
                    if((*temp).getCost()<minc)//update
                    {
                      mypath.n1[i]=q[j];//FROM q[j], will be updated as a new minimum is found
                      mypath.n2[i]=(*temp).getNodeNo();//TO q[j]->next, will be updated as new minimum are found
                      minc=(*temp).getCost();
                    }
                }
                temp=(*temp).getNext();
            }
            j++;
        }
        //After this a minima of the edges going out from the current mst has been found.
        //The node added is mypath.n2[i]; We now reset minc; add mypath.n2[i] to q; update mst_cost; update p;
        mypath.mst_cost+=minc;
        q[j]=mypath.n2[i];
        p[mypath.n2[i]]=1;
        minc=PINF;
    }

    return mypath;
}
//////////////////////////////////////////////
void Graph::createAdj()
{
    adjgraph.resize(n);
    for(int i=0; i<n; i++)
    {
        adjgraph[i].setNodeNo(i);
        adjgraph[i].setCost(0);
        adjgraph[i].setNext(NULL);
    }
    //adding more nodes now
    for(int i=0; i<n; i++)
    {
        nodeelem* curr=NULL;
        curr=&adjgraph[i];//pointer to a nodeelem object
        for(int j=0; j<n; j++) //
        {
            if(graph[i][j]!=0)//making connections from i TO j
            {
                nodeelem* temp=new nodeelem;//create pointer to a new empty node
                (*temp).setNodeNo(j);//.seNode etc work on the object themselves,not on pointers to the object
                (*temp).setCost(graph[i][j]);//thus the use of (*temp)
                (*temp).setNext(NULL);
                (*curr).setNext(temp);
                curr=temp;
            }
        }
    }

}
/////////////////////////////////////////////////
void Graph::adjPrint()
{
    cout << "\nAdjoint Graph:" << endl;
    for(int i=0; i<n; i++)
    {
        nodeelem* temp;
        temp=&adjgraph[i];
        while(temp!=NULL)
        {
            cout<<"["<<(*temp).getNodeNo()<<"]"<<"("<<(*temp).getCost()<<") -> ";
            temp=(*temp).getNext();
        }
        cout<<endl;
    }
}
double** Graph::RGG(int n,double p,double cost1,double cost2)//n is the number of nodes in the graph
{
    //p is the probability that an edge exists
    //(cost1-cost2) is the range of costs
    int limit=static_cast<int>(100*p);
    int k;
    double cost;
    double** graph;
    srand(time(0));
    graph=new double*[n];//columns defined
    for(int i=0; i<n; i++)
    {
        graph[i]=new double[n];//rows initialized
    }
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(i==j)
                graph[i][j]=0;
            else
            {
                k=rand()%100+1;//gives number between 1 and 100
                cost=cost1+((cost2-cost1)*static_cast<double>(rand()))/32767.0;//gives a number in the range cost1-cost2
                if(k<limit)//edge exists
                {
                    graph[i][j]=graph[j][i]=cost;
                }
                else//edge doesn't exist
                    graph[i][j]=graph[j][i]=0;
            }

        }
    }

    return graph;
}
/////////////////////////////////////////////
void Graph::prngraph()
{
    cout<<"\nGRAPH:\n";
    cout<<setprecision(2)<<fixed;
    for(int i=0; i<n; i++)
    {
        cout<<setw(6)<<setfill(' ')<<i;
    }
    cout<<endl;
    for(int i=0; i<n; i++)
    {
        cout<<setprecision(2)<<fixed;
        cout<<setw(2)<<setfill('0')<<i<<"::";
        for(int j=0; j<n; j++)
        {
            cout<<setw(5)<<setfill('0')<<graph[i][j]<<" ";
        }
        cout<<"\n";
    }
}
/////////////////////////////////////////

int main()
{
    string pathname;
    cout<<"Enter full path name to the file:\n";
    cout<<"Example Use :D:\\\\C++ Code\\\\File_Test\\\\sample_data.txt for ";
    cout<<"D:\\C++ Code\\File_Test\\sample_data.txt\n";
    getline(cin,pathname);
    Graph G(pathname);
    G.prngraph();
    G.createAdj();
    G.adjPrint();
    info mypath=G.MST();
    cout<<"\nMinimum Spanning Tree is given as\n:";
    cout<<"Edges included\n";
    int a,b;
    double** graph=G.getgraph();

    for(int i=0;i<G.getn()-1;i++)
    {
        a=mypath.n1[i];
        b=mypath.n2[i];
        cout<<"("<<a<<","<<b<<"): "<<graph[a][b]<<"\n";
    }
    cout<<"Cost of the MST : "<<mypath.mst_cost;

    return 0;
}
