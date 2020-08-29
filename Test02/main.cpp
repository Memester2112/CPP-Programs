#include <iostream>
#include<vector>
using namespace std;

void MyFun(vector<int> v)//call by value
{
    cout<<"MyFun(Call by Value)"<<endl;
    cout<<v[0]<<endl;
    return;
}

void MyFun2(vector<int>& v)//call by reference
{
    cout<<"MyFun2(Call by Reference)"<<endl;
    cout<<v[0]<<endl;

    return;
}

int main()
{
    vector<int> a;//without specifying size
    vector<int> b(3);//with size
    a.push_back(23);
    b[0]=49;
    MyFun(a);
    MyFun(b);
    MyFun2(a);
    MyFun2(b);
    cout << "Hello world!" << endl;
    return 0;
}
