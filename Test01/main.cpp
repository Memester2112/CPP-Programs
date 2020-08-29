//#include <iostream>
//#include<complex>
//using namespace std;
//
//template<class T>
//inline void myswap(T& a,T& b)
//{
//    T temp=a;
//    a=b;
//    b=temp;
//}
//template<class X>
//inline X mysum(X* a,int n)
//{
//    X sum=0;
//    for(int i=0; i<n; i++)
//        sum+=a[i];
//
//    return(sum);
//}
//
//
//int main()
//{
//    cout << "Hello world!" << endl;
//    complex<double> r(1.2,1.2), s(2.4,2.4);
//    cout<< "inputs" << r <<" and "<< s<< endl;
//    myswap(r,s);
//    cout<<"Outputs" << r<<" and "<< s <<endl;
//
//    double a[]= {1.0,2.0,3.5,5.0,4.0};
//    double k=mysum(a,5);
//    cout<<"sum = "<<k<<endl;
//    return 0;
//}

//

/* Convert this program to C++

* change to C++ io

* change to one line comments

* change defines of constants to const

* change array to vector<>

* inline any short function

*/

#include <iostream>
#include<vector>
using namespace std;
const int N=40;

void sum(int& p,int n,vector<int> d)
{
    int i;
    p=0;
    for(i=0;i<n;++i)
    {
        p+=d[i];
    }
}

int main()
{
    int i;
    int accum = 0;
    vector<int> data;
    for(i = 0; i < N; ++i)
    {
        data.push_back(i);
    }

    sum(accum, N, data);
    cout<<"sum is "<<accum<<endl;
    return 0;
}
