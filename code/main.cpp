#include "Set.cpp"
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <map>

using namespace std;

list<string> split(const string &str)
{
    list<string> elems;
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
    map<string, int> mapping = {{"create", CM_CREATE}, {"add", CM_ADD}, {"del", CM_DEL}, {"unite", CM_UNITE}, {"intersect", CM_INTERSECT}, {"diff", CM_DIFF}, {"symDiff", CM_SYMDIFF}, {"checkInclusion", CM_CHECK_INCLUSION}, {"print", CM_PRINT}, {"exit", CM_EXIT}};
    cout << "start:" << endl;
    string input, tmp;
    SetsList lst;

    while (true)
    {
        getline(cin, input);
        stringstream ss;
        ss.str(input);
        getline(ss, tmp, ' ');

        if (mapping.find(tmp) == mapping.end())
        {
            cout << "unknown command" << endl;
            continue;
        }

        switch (mapping[tmp])
        {
        case CM_CREATE:
        {
            if (!getline(ss, tmp, ' '))
            {
                cout << "too few arguments" << endl;
                break;
            }
            MySet set(tmp);

            if (getline(ss, tmp, ' '))
            {
                cout << "too much arguments" << endl;
                break;
            }

            if (lst.AddSet(set))
            {
                cout << "such set already exists" << endl;
                break;
            }
            cout<<endl;
            break;
        }

        case CM_ADD:
        {
            string setName, element;
            if (!getline(ss, setName, ' ') || !getline(ss, element))
            {
                cout << "too few arguments" << endl;
                break;
            }
            MySet *set = lst.FindSet(setName);

            if (set == nullptr)
            {
                cout << "no such set in list" << endl;
                break;
            }

            SetElement elem;
            if (elem.SetData(element.data()))
            {
                cout << "incorrect element data" << endl;
                break;
            }

            if (set->AddElement(elem))
            {
                cout << "such element already exists in this set" << endl;
                break;
            }
            cout<< endl;
            break;
        }

        case CM_DEL:
        {
            string setName, element;
            if (!getline(ss, setName, ' '))
            {
                cout << "too few arguments" << endl;
                break;
            }
            MySet *set = lst.FindSet(setName);

            if (set == nullptr)
            {
                cout << "no such set in list" << endl;
                break;
            }

            if (getline(ss, element, ' '))
            {
                if (!set->FindElement(element.data()))
                {
                    cout << "no such element in set" << endl;
                    break;
                }
                set->DeleteElement(element.data());
                break;
            }
            lst.DeleteSet(setName);
            cout<< endl;
            break;
        }

        case CM_PRINT:
        {
            string setName;
            if (getline(ss, setName, ' '))
            {
                MySet *set = lst.FindSet(setName);

                if (set == nullptr)
                {
                    cout << "no such set in list" << endl;
                    break;
                }
                cout << (*set) << endl;
                break;
            }
            cout << lst << endl;
            break;
        }

        case CM_UNITE:
        {
            string leftSetName, rightSetName, resultSetName;
            if (!getline(ss, leftSetName, ' ') || !getline(ss, rightSetName, ' '))
            {
                cout << "too few arguments" << endl;
                break;
            }

            MySet *leftSet = lst.FindSet(leftSetName), *rightSet = lst.FindSet(rightSetName);

            if (leftSet == nullptr || rightSet == nullptr)
            {
                cout << "no such set in list" << endl;
                break;
            }

            if (getline(ss, resultSetName, ' '))
            {
                lst.DeleteSet(resultSetName);
                MySet newSet = Unite(*leftSet, *rightSet, resultSetName);
                lst.AddSet(newSet);
                break;
            }
            MySet newSet = Unite(*leftSet, *rightSet, "noname(");
            cout << newSet << endl;
            break;
        }

        case CM_INTERSECT:
        {
            string leftSetName, rightSetName, resultSetName;
            if (!getline(ss, leftSetName, ' ') || !getline(ss, rightSetName, ' '))
            {
                cout << "too few arguments" << endl;
                break;
            }

            MySet *leftSet = lst.FindSet(leftSetName), *rightSet = lst.FindSet(rightSetName);

            if (leftSet == nullptr || rightSet == nullptr)
            {
                cout << "no such set in list" << endl;
                break;
            }

            if (getline(ss, resultSetName, ' '))
            {
                lst.DeleteSet(resultSetName);
                MySet newSet = Intersection(*leftSet, *rightSet, resultSetName);
                lst.AddSet(newSet);
                break;
            }
            MySet newSet = Intersection(*leftSet, *rightSet, "noname(");
            cout << newSet << endl;
            break;
        }

        case CM_DIFF:
        {
            string leftSetName, rightSetName, resultSetName;
            if (!getline(ss, leftSetName, ' ') || !getline(ss, rightSetName, ' '))
            {
                cout << "too few arguments" << endl;
                break;
            }

            MySet *leftSet = lst.FindSet(leftSetName), *rightSet = lst.FindSet(rightSetName);

            if (leftSet == nullptr || rightSet == nullptr)
            {
                cout << "no such set in list" << endl;
                break;
            }

            if (getline(ss, resultSetName, ' '))
            {
                lst.DeleteSet(resultSetName);
                MySet newSet = Difference(*leftSet, *rightSet, resultSetName);
                lst.AddSet(newSet);
                break;
            }
            MySet newSet = Difference(*leftSet, *rightSet, "noname(");
            cout << newSet << endl;
            break;
        }

        case CM_SYMDIFF:
        {
            string leftSetName, rightSetName, resultSetName;
            if (!getline(ss, leftSetName, ' ') || !getline(ss, rightSetName, ' '))
            {
                cout << "too few arguments" << endl;
                break;
            }

            MySet *leftSet = lst.FindSet(leftSetName), *rightSet = lst.FindSet(rightSetName);

            if (leftSet == nullptr || rightSet == nullptr)
            {
                cout << "no such set in list" << endl;
                break;
            }

            if (getline(ss, resultSetName, ' '))
            {
                lst.DeleteSet(resultSetName);
                MySet newSet = SymDiff(*leftSet, *rightSet, resultSetName);
                lst.AddSet(newSet);
                break;
            }
            MySet newSet = SymDiff(*leftSet, *rightSet, "noname(");
            cout << newSet << endl;
            break;
        }

        case CM_CHECK_INCLUSION:
        {
            string leftSetName, rightName;

            if (!getline(ss, leftSetName, ' ') || !getline(ss, rightName, ' '))
            {
                cout << "too few arguments" << endl;
                break;
            }

            MySet *leftSet = lst.FindSet(leftSetName);

            if (leftSet == nullptr)
            {
                cout << "no such set in list" << endl;
                break;
            }

            if (rightName[0] == '\"' && rightName[rightName.length() - 1] == '\"')
            {
                cout << !!leftSet->FindElement((rightName.substr(1, rightName.length() - 2)).data()) << endl;
                break;
            }

            MySet *rightSet = lst.FindSet(rightName);

            if (rightSet == nullptr)
            {
                cout << "no such set in list" << endl;
                break;
            }

            cout << leftSet->CheckInclusion(*rightSet) << endl;
            break;
        }
        
        case CM_EXIT:
        {
            cout<<"turning off...";
            return 0;
        }
        }
    }
}

int main()
{
    SetsInterface();
}
