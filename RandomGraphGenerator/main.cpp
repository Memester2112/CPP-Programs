#include <iostream>
#include <vector>
#include<cstdlib>
#include<ctime>
#include<iomanip>
using namespace std;
const int PINF=99999999;
class nodeelem
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

void prngraph(double** graph,int n)
{
    cout<<"GRAPH:\n";
    for(int i=0; i<n; i++)
    {
        cout<<setprecision(2)<<fixed;
        for(int j=0; j<n; j++)
        {
            cout<<graph[i][j]<<" ";
        }
        cout<<"\n";
    }
}
//////////////////////////////////////////////////////

void adjPrint(vector<nodeelem> adjgraph,int n)
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
////////////////////////////////////////////////////
int main()
{
    cout << "Enter number of nodes in the graph:" << endl;
    int n;
    cin>>n;
    double p,cost1,cost2;
    cout<<"Enter value of p(0.2,0.3 etc)\n";
    cin>>p;
    cout<<"Enter a cost range (eg 1.0 12.0)\n";
    cin >> cost1 >> cost2;
    double** graph=RGG(n,p,cost1,cost2);
    prngraph(graph,n);
    ///////////////////////////////
    vector<nodeelem> adjgraph(n);
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
    adjPrint(adjgraph,n);
    return 0;
}

