#include <iostream>
#include<vector>
#include<iomanip>
#include<cstdlib>
#include<ctime>
using namespace std;
const double PINF=999999999;

typedef struct path
{
    int* parent;//parent links to go back from g to s
    double pathLength;//total cost length of the path
    int length;//number of nodes including s and g
} path;
//...........................
typedef struct info
{
    double value;
    int nodeno;
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
    ////////////////////////

    ////////////////////////
    void printHeap()
    {
        cout<<setprecision(2)<<fixed;
        cout<<"HEAP  VALUE(NODE_NUMBER)"<<endl;
        for(int i=1; i<=static_cast<int>(arr[0]); i++)
        {
            cout<<arr[i]<<"("<<node[i]<<") ";
        }
        cout<<endl;
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

//RANDOM GRAPH GENERATOR: RGG
double** RGG(int n,double p,double cost1,double cost2)//n is the number of nodes in the graph
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

//Prints the 2d graph matrix
void prngraph(double** graph,int n)
{
    cout<<"GRAPH:\n";
    for(int i=0; i<n; i++)
    {
        cout<<setprecision(2)<<fixed;
        for(int j=0; j<n; j++)
        {
            cout<<setw(5)<<setfill('0')<<graph[i][j]<<" ";
        }
        cout<<"\n";
    }
}
//Methods for RANDOM GRAPH GENERATOR over
//////////////////////////////////////////////////////


path BFSW(int s,int g,double** graph,int n)//s is the starting node and g is the goal node
{
    //n is the number of nodes
    int inQueue[n];//maintains if the node is currently in the queue or not
    double cost[n];
    int visited[n];
    //initializing cost array and inQueue array and visited array
    //mypath is the return variable;
    path mypath;
    mypath.length=0;
    mypath.parent=new int[n];
    mypath.pathLength=0;

    for(int i=0; i<n; i++)
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

        for(int i=0; i<n; i++) //finding all successors of j
        {
            if(i!=j.nodeno&&graph[j.nodeno][i]!=0)//edge exists
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

int main()
{
    int n;
    double p,cost1,cost2;
    n=50;
    p=0.2;
    cost1=1;
    cost2=10;
    double** graph=RGG(n,p,cost1,cost2);
    //prngraph(graph,n);//Prints the connectivity matrix
    int s=0;//starting node
    double sum;//stores the sum of the paths from 1-2,1-3...1-50
    //note that the nodes are actually numbered from 0, so the paths are 0-1,0-2,...0-49
    int c=0;//counts the number of paths that do exist,used in calculating the average path length
    cout<<"P=0.2"<<endl;
    for(int g=1; g<=49; g++)
    {
        path mypath=BFSW(s,g,graph,n);
        //cout<<"Path From "<<s<<" to "<<g<<" = "<<mypath.pathLength<<endl;
        if(mypath.pathLength!=PINF)//Implies that a path exists
        {
            sum=sum+mypath.pathLength;
            c++;
        }
    }
    cout<<"Average distance: = "<<(sum/c)<<endl;

    p=0.4;
    graph=RGG(n,p,cost1,cost2);
    sum=0;c=0;
    cout<<"P=0.4"<<endl;
    for(int g=1; g<=49; g++)
    {
        path mypath=BFSW(s,g,graph,n);
        //cout<<"Path From "<<s<<" to "<<g<<" = "<<mypath.pathLength<<endl;
        if(mypath.pathLength!=PINF)//Implies that a path exists
        {
            sum=sum+mypath.pathLength;
            c++;
        }
    }
    cout<<"Average distance: = "<<(sum/c)<<endl;


    return 0;
}
