#include "Set.cpp"

using namespace std;

int SetsInterface()
{
    
}

int main()
{
    MySet a, b;

    a.AddElement("aa");
    a.AddElement("bb");
    a.AddElement("zz");
    a.AddElement("dd");
    a.AddElement("ee");

    cout << a.Cardinality();

    b.AddElement("aa");
    b.AddElement("cc");
    b.AddElement("zz");
    b.AddElement("dd");
    b.AddElement("ee");

    cout << b.Cardinality() << endl;

    cout << b.CheckInclusion(a);
}
