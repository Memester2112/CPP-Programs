#include <iostream>

using namespace std;

int main()
{
    int b=5;
    int& a=b;
    cout<<"&b= "<<&b<<endl;
    cout<<"&a= "<<&a<<endl;
    int* ptr_a=nullptr;
    ptr_a=&b;
    cout<<"ptr_a = "<<ptr_a<<endl;
    cout<<"*ptr_a = "<<*ptr_a<<endl;

    return 0;
}
