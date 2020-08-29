#include <iostream>
#include<algorithm>
#include<random>
#include<ctime>
using namespace std;

int main()
{
    vector<int> cards;
    for(int i=0; i<10; i++)
        cards.push_back(i*10);
    for(auto i=0; i<10; i++)
    {
        cout<<cards[i]<<" ";
    }
    cout<<endl;
    auto rng = default_random_engine {};
    shuffle(begin(cards), end(cards), rng);

    for(auto i=0; i<10; i++)
    {
        cout<<cards[i]<<" ";
    }

    /**
    For deleting specific element from the middle
    first finds its position and make an iterator point to it and then erase it
    **/
    auto it=cards.begin();
    for(it=cards.begin(); it!=cards.end(); it++)
    {
        if(*it==70)
            break;
    }
    cards.erase(it);
    cout<<endl;
    for(auto i=cards.begin(); i!=cards.end(); i++)
    {
        cout<<*i<<" ";
    }
    for(int i=0; i<10; i++)
    {
        shuffle(begin(cards), end(cards), rng);
        cout<<endl;
        for(auto i=cards.begin(); i!=cards.end(); i++)
        {
            cout<<*i<<" ";
        }
    }

    cout<<endl;
}
