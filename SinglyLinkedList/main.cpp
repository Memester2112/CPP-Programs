#include <iostream>
#include<vector>
using namespace std;


class slistelem
{
public:
    slistelem():data(0),next(0)
    {
        //initializer constructor
    }
    slistelem(int n,slistelem* ptr):data(n),next(ptr)
    {
        //initializer constructor with parameters
    }

    int data;
    slistelem* next;
};

class slist
{
public:

    slist():head(0)
    {
        //initializer constructor
    }
    slist(vector<int> dat,int n)
    {//copy constructor: takes a vector and converts it to a slist
         slistelem* temp=new slistelem();
         temp->data=dat[0];
         temp->next=NULL;
         head= temp;
        for(int i=1;i<n;i++)
        {
          head =new slistelem(dat[i],head);
        }

    }

    void release()
    {
        //h is the head
        slistelem* temp=head;
        while(head!=NULL)
        {
            temp=head;
            delete temp;
            head=head->next;
        }
    }
    void prepend(int n);
    ~slist()
    {
        release();//auxiliary method
    }
    slistelem* getHead()
    {
        return head;
    }

private:

    slistelem* head;


};



//This type of list is LIFO
//Continuously changing the head,that is the beginning of the list
void slist::prepend(int key)
{
    head=new slistelem(key,head);
}

int main()
{
    slist list1;
    list1.prepend(8);
    list1.prepend(9);
    for(int i=0;i<=50;i++)
    {
        list1.prepend(i*i);
    }

    slistelem* temp=list1.getHead();
    cout<<"LIST 1 :"<<endl;
    while(temp!=NULL)
    {
        cout<<temp->data<<",";
        temp=temp->next;
    }
    vector<int> d;//={1,3,7,9,11};
    d.push_back(1);
    d.push_back(3);
    d.push_back(5);
    d.push_back(7);
    d.push_back(9);

    slist list2(d,5);
    cout <<endl<< "LIST 2 :" << endl;
    temp=list2.getHead();
    while(temp!=NULL)
    {
        cout<<temp->data<<",";
        temp=temp->next;
    }

    cout<<"\nVector content :\n";
    for(int i=0;i<d.size();i++)
    {
        cout<<d[i]<<endl;
    }

    return 0;
}
