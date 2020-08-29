#include <iostream>
#include<fstream>
#include<string>
using namespace std;
int main()
{
    string pathname;
    cout<<"Enter full path name to the file:\n";
    cout<<"Example Use :D:\\\\C++ Code\\\\File_Test\\\\sample_data.txt for ";
    cout<<"D:\\C++ Code\\File_Test\\sample_data.txt\n";
    getline(cin,pathname);
    ifstream fin;
    //fin.open("D:\\C++ Code\\File_Test\\sample_data.txt");
    fin.open(pathname.c_str());
    cout<<pathname.c_str()<<endl;
    int n;
    if(fin.is_open())
    {

        fin>>n;
        cout<<"Size of graph = "<<n<<endl;
    }
    else
        cout<<"Could not open\n";

    int node1,node2,cost;
    char ch;
    while(!fin.eof())
    {
        fin>>node1>>node2>>cost;
        cout<<"("<<node1<<","<<node2<<") :"<<cost<<endl;
    }
    fin.close();

    return 0;
}
//    fstream new_file;
//    new_file.open("new_file.txt",ios::out);
//    if(!new_file)
//    {
//        cout<<"File creation failed";
//    }
//    else
//    {
//        cout<<"New file created";
//        new_file<<"Learning File handling\n123\n";    //Writing to file
//        new_file.close();
//    }
//    //////////////////////////////////////
//    cout<<"\n\n**********************\n\n";
//    fstream new_file1;
//
//    new_file1.open("new_file.txt",ios::in);
//    if(!new_file1)
//        cout<<"No such file";
//
//    else
//    {
//        char ch;
//        while (!new_file1.eof())
//        {
//            new_file1 >>ch;
//            cout << ch;
//            if(ch==' ')
//                cout<<' ';
//            if(ch=='\n')
//                cout<<'\n';
//        }
//        new_file1.close();
//    }
//
//int main()
//{
//    // Creation of ofstream class object
//    ofstream fout;
//
//    string line;
//
//    // by default ios::out mode, automatically deletes
//    // the content of file. To append the content, open in ios:app
//    // fout.open("sample.txt", ios::app)
//    fout.open("sample.txt");
//    line="5\n1 2 3\n4 5 6\n";
//    // Execute a loop If file successfully opened
//
//    // Read a Line from standard input
//
//    // Write line in file
//    fout << line << endl;
//
//    // Close the File
//    fout.close();
//
//    // Creation of ifstream class object to read the file
//    ifstream fin;
//
//    // by default open mode = ios::in mode
//    fin.open("sample.txt");
//
//    // Execute a loop until EOF (End of File)
//    while (fin)
//    {
//
//        // Read a Line from File
//        getline(fin, line);
//
//        // Print line in Console
//        cout << line << endl;
//    }
//
//    // Close the file
//    fin.close();
//
//    return 0;
//}
//
