#include "Set.cpp"
#include <vector>
#include <sstream>
#include <map>

using namespace std;

vector<string> split(const string &str)
{
    vector<string> elems;
    stringstream ss;
    ss.str(str);
    string item;

    while (getline(ss, item, ' '))
        elems.push_back(item);

    return elems;
}

int SetsInterface()
{
    enum commands
    {
        CM_CREATE,
        CM_ADD,
        CM_DEL,
        CM_UNITE,
        CM_INTERSECT,
        CM_DIFF,
        CM_SYMDIFF,
        CM_CHECK_INCLUSION,
        CM_PRINT,
        CM_EXIT
    };

    map<string, int> mapping = {{"create",CM_CREATE},{"add",CM_ADD},{"del",CM_DEL},{"unite",CM_UNITE},
    {"intersect",CM_INTERSECT},{"diff",CM_DIFF},{"symDiff",CM_SYMDIFF},{"checkInclusion",CM_CHECK_INCLUSION},
    {"print",CM_PRINT},{"exit",CM_EXIT}};

    cout << "start:" << endl;

    string input, tmp;

    while (true)
    {
        getline(cin, input);

        vector<string> line = split(input);

        tmp = line.front();

        switch (mapping[tmp])
        {
        case CM_CREATE:
            
            break;
        
        default:
            break;
        }
    }
}

int main()
{
    SetsInterface();
    MySet a("a"), b("b");

    a.AddElement("aa");
    a.AddElement("bb");
    a.AddElement("zz");
    a.AddElement("dd");
    a.AddElement("ee");

    cout << a;

    b.AddElement("xx");
    b.AddElement("bb");
    b.AddElement("bb");
    b.AddElement("xx");
    b.AddElement("ee");

    cout << b << endl;
}
