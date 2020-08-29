#include <iostream>
#include<vector>
#include<iomanip>
#include<cstdlib>
using namespace std;
const int PINF=99999999;
typedef struct info
{
    double value;
    int nodeno;
}info;
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
         for(int i=1;i<=n;i++)
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
         for(int i=1;i<=static_cast<int>(arr[0]);i++)
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
    for(int i=1;i<=static_cast<int>(arr[0]);i++)
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

int main()
{
    double a[]={7,6,5,4,3,2,1};//values in the heap
    int b[]={10,20,30,40,50,60,70};//node numbers
    heap heap1(a,b,7);
    //vector<int>& v=heap1.getHeap();
    heap1.printHeap();
    vector<double> v=heap1.getHeap();
//    int s=v.size();//size of arr
//    cout<<endl<<"Ascending Order:\n";
//    for(int i=1;i<s;i++)
//    {
//        k=heap1.delMin();
//        cout<<k.value<<" from node number :"<<k.nodeno<<"\n";
//
//    }
    cout<<endl;
    heap1.reOrder(30,0);
    heap1.printHeap();
    cout<<"\nHEAP2:TEST BY addNode\n";
    heap heap2;
    for(int i=0;i<7;i++)
    {
        heap2.addNode(a[i],b[i]);

    }
    heap2.printHeap();
    vector<double> w=heap2.getHeap();
    int s=w.size();//size of arr
    cout<<endl<<"Ascending Order:\n";
    for(int i=1;i<s;i++)
    {
        info k=heap2.delMin();
        cout<<k.value<<" from node number :"<<k.nodeno<<"\n";

    }
    w=heap2.getHeap();
    cout<<"Size of heap after total deletion:"<<w[0];
//    for(vector<int>::iterator p=v.begin();p!=v.end();p++)
//    {
//                cout<< *p << endl;
//
//    }
//    cout<<"********************************************"<<endl;
//    vector<int> k(2);
//    k[0]=15;
//    k[1]=16;
//    k[2]=17;
//    for(vector<int>::iterator p=k.begin();p<k.end();p++)
//    {
//        cout<<*p<<endl;
//
//    }

    return 0;
}

