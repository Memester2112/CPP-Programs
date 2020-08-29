#include <iostream>
#include<vector>
using namespace std;
//Use of class within a class
const int PINF=99999999;
class nodeelem
{
public:
    //constructor
    nodeelem():nodeno(0),cost(PINF),next(0)
    {
        //initializer class
    }
    //parameterized constructor
    nodeelem(int nodenumber,int cost):nodeno(nodenumber),cost(cost),next(0)
    {
        //initializer constructor
    }

    //getter functions
    int getNodeNo()
    {
        return nodeno;
    }
    int getCost()
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
    void setCost(int n)
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
    int cost;//From the parent node in the list to this node: cost
    nodeelem* next;//to point to the next in row
};

int main()
{
    cout<<"Enter number of nodes in the graph"<<endl;
    int n;
    cin>>n;

    vector<nodeelem> graph(n);
    for(int i=0;i<n;i++)
    {
        graph[i].setNodeNo(i);
        graph[i].setCost(0);
        graph[i].setNext(NULL);
    }

    cout << "Graph:" << endl;
    for(int i=0;i<n;i++)
    {
        nodeelem* temp;
        temp=&graph[i];
        while(temp!=NULL)
        {
            cout<<"["<<(*temp).getNodeNo()<<"]"<<"("<<(*temp).getCost()<<") -> ";
            temp=(*temp).getNext();
        }
        cout<<endl;

    }
    return 0;
}
