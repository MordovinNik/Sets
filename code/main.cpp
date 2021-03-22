#include "Set.h"
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <map>

using namespace std;

MySet Unite(const MySet& Set1, const MySet& Set2, const std::string& setName);
MySet Intersection(const MySet& Set1, const MySet& Set2, const std::string& setName);
MySet Difference(const MySet& Set1, const MySet& Set2, const std::string& setName);
MySet SymDiff(const MySet& Set1, const MySet& Set2, const std::string& setName);

void DeleteLeftRightSpaces(string &str)
{
    int i = 0, j = str.size() - 1;
    while (isspace(str[i]) && i < str.length())
        i++;

    while (isspace(str[j]) && j > 0)
        j--;

    if (i <= j)
        str = str.substr(i, j - i + 1);
    else
        str.clear();
}

void DeleteDoubleSpaces(string &str)
{
    int size = str.size();
    bool del = true, wasSpace = false;
    string newstr;
    for (int i = 0; i < size; i++)
    {
        if (str[i] == '\"')
            del = false;

        if (isspace(str[i]) && str[i] != '\n' && del)
        {
            if (wasSpace)
                continue;
            else
                wasSpace = true;
        }
        else if (wasSpace)
            wasSpace = false;
        newstr.push_back(str[i]);
    }
    str = newstr;
}

int checkLocale(string &str)
{
    for (auto i : str)
    {
        if (i < 0 || i > 127)
            return false;
    }
    return true;
}

void SetOperation(MySet (*func)(const MySet &, const MySet &, const std::string &), stringstream &ss, SetsList &lst)
{
    string leftSetName, rightSetName, resultSetName;
    if (!getline(ss, leftSetName, ' ') || !getline(ss, rightSetName, ' '))
    {
        cout << "too few arguments" << endl;
        return;
    }
    DeleteLeftRightSpaces(rightSetName);
    MySet *leftSet = lst.FindSet(leftSetName), *rightSet = lst.FindSet(rightSetName);

    if (leftSet == nullptr || rightSet == nullptr)
    {
        cout << "no such set in list" << endl;
        return;
    }

    if (getline(ss, resultSetName, ' '))
    {
        lst.DeleteSet(resultSetName);
        MySet newSet = func(*leftSet, *rightSet, resultSetName);
        lst.AddSet(newSet);
        return;
    }
    MySet newSet = func(*leftSet, *rightSet, "noname(");
    cout << newSet;
    return;
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
        CM_EXIT,
        CM_HELP,
        CM_CARDINALITY
    };

    map<string, int> mapping = {{"create", CM_CREATE}, {"add", CM_ADD}, {"del", CM_DEL}, {"unite", CM_UNITE}, {"intersect", CM_INTERSECT}, {"diff", CM_DIFF}, {"symDiff", CM_SYMDIFF}, {"checkInclusion", CM_CHECK_INCLUSION}, {"print", CM_PRINT}, {"exit", CM_EXIT}, {"help", CM_HELP},{"cardinality",CM_CARDINALITY}};
    cout << "start:" << endl;
    string input, tmp;
    SetsList lst;

    while (true)
    {
        cout << ">>";
        getline(cin, input);
        if (!checkLocale(input))
        {
            cout << "incorrect symbols\n";
            continue;
        }
        DeleteDoubleSpaces(input);
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
        case CM_HELP:
        {
            cout << "Имя множества не может начинаться с \" и не может содержать пробельных символов\n"
            "<> - обязательный параметр команды\n"
            "[] - необязательный параметр команды\n\n"

            "Список команд программы:\n"

            "  create <name> - создает пустое множество с именем name.\n\n"

            "  add <name> <\"element\"> - добавляет элемент string во множество с именем name.\n\n"

            "  del <name> [\"element\"] - удаляет множество с именем name.\n"
            "    В случае если указан параметр element, то удаляет его из множества с именеи name.\n\n"

            "  unite <leftSetName> <rightSetName> [resultSetName] - выводит на экран результат объединения множеств с именами leftSetName и rightSetName\n"
            "    или записывает результат во множество с именем resultSetName если оно указано.\n"
            "    В случае если множество с именем resultSetName не существует - создаст новое с таким именем.\n\n"

            "  intersect <leftSetName> <rightSetName> [resultSetName] - выводит на экран результат пересечения множеств с именами leftSetName и rightSetName\n"
            "    или записывает результат во множество с именем resultSetName если оно указано.\n"
            "    В случае если множество с именем resultSetName не существует - создаст новое с таким именем.\n\n"

            "  diff <leftSetName> <rightSetName> [resultSetName] - выводит на экран результат вычитания множества с именем rightSetName из множества с именем leftSetName\n"
            "    или записывает результат во множество с именем resultSetName если оно указано.\n"
            "    В случае если множество с именем resultSetName не существует - создаст новое с таким именем.\n\n"

            "  symDiff<leftSetName><rightSetName>[resultSetName] - выводит на экран результат симметрической разности множеств с именами leftSetName и rightSetName\n"
            "    или записывает результат во множество с именем resultSetName если оно указано.\n"
            "    В случае если множество с именем resultSetName не существует - создаст новое с таким именем.\n\n"

            "  checkInclusion<leftSetName> <rightSetName | \"element\"> - выводит на экран результат проверки на вхождение множества с именем rightSetName\n"
            "    или элемента element во множество с именем leftSetName.\n"
            "    Возвращаемые значения :\n"
            "      1 - множество с именем rightSetName или элемент element входит во множество с именем leftSetName\n"
            "      0 - множество с именем rightSetName или элемент element не входит во множество с именем leftSetName\n\n"

            "  print[name] - выводит на экран список имен всех множеств или список всех элементов множества с именем name\n\n"

            "  exit - выход из программы.\n";
            break;
        }

        case CM_CREATE:
        {
            if (!getline(ss, tmp, ' '))
            {
                cout << "too few arguments" << endl;
                break;
            }

            if (tmp[0] == '\"')
            {
                cout << "set name cannot start with \"\n";
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
            break;
        }

        case CM_CARDINALITY:
        {
            if (!getline(ss, tmp, ' '))
            {
                cout << "too few arguments" << endl;
                break;
            }

            MySet *set = lst.FindSet(tmp);

            if (set == nullptr)
            {
                cout << "no such set in list" << endl;
                break;
            }

            if (getline(ss, tmp, ' '))
            {
                cout << "too much arguments" << endl;
                break;
            }

            cout<<set->Cardinality()<<endl;

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

            DeleteLeftRightSpaces(element);

            if (element[0] == '\"' && element[element.length() - 1] == '\"')
            {
                element = element.substr(1, element.length() - 2);
            }
            else
            {
                cout << "incorrect element input format" << endl;
                break;
            }

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

                DeleteLeftRightSpaces(element);

                if (element[0] == '\"' && element[element.length() - 1] == '\"')
                {
                    element = element.substr(1, element.length() - 2);
                }
                else
                {
                    cout << "incorrect element input format" << endl;
                    break;
                }

                if (!set->FindElement(element.data()))
                {
                    cout << "no such element in set" << endl;
                    break;
                }
                set->DeleteElement(element.data());
                break;
            }
            lst.DeleteSet(setName);
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
                cout << (*set);
                break;
            }
            cout << lst;
            break;
        }
        case CM_UNITE:
        {
            SetOperation(Unite, ss, lst);
            break;
        }

        case CM_INTERSECT:
        {
            SetOperation(Intersection, ss, lst);
            break;
        }

        case CM_DIFF:
        {
            SetOperation(Difference, ss, lst);
            break;
        }

        case CM_SYMDIFF:
        {
            SetOperation(SymDiff, ss, lst);
            break;
        }

        case CM_CHECK_INCLUSION:
        {
            string leftSetName, rightName;

            if (!getline(ss, leftSetName, ' ') || !getline(ss, rightName, '\n'))
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
            DeleteLeftRightSpaces(rightName);
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
            cout << "turning off...\n";
            return 0;
        }
        }
    }
}

int main()
{
    SetsInterface();
}
