#include "Set.h"
#include <cstring>

#define INVALID_SYMBOL 1
#define TOO_LONG_INPUT 2
#define SUCH_ELEMENT_ALREADY_EXISTS 3
#define NO_SUCH_ELEMENT_IN_SET 4

using namespace std;

SetElement::SetElement()
{
    data[0] = 0;
    prev = next = nullptr;
}

SetElement::SetElement(const SetElement &element)
{
    memcpy(data, element.data, strlen(element.data) + 1);

    next = nullptr;
    prev = nullptr;
}

SetElement::~SetElement()
{
    next = prev = nullptr;
    data[0] = 0;
}

std::ostream &operator<<(std::ostream &out, const SetElement &element)
{
    out << element.data;
    return out;
}

int SetElement::SetData(const char *dat)
{
    int i = 0;

    while (dat[i] != 0 && i < 81)
    {
        if (dat[i] > 31 && dat[i] < 127 && dat[i] != '\"')
            i++;
        else
            return INVALID_SYMBOL;
    }

    if (i == 81)
        return TOO_LONG_INPUT;

    memcpy(data, dat, i);
    data[i] = 0;

    return 0;
}

const char *SetElement::Data() const
{
    return data;
}

bool operator<(const SetElement &left, const SetElement &right)
{
    int i = 0;

    while (i < 80 && left.data[i] != 0 && right.data[i] != 0 && left.data[i] == right.data[i])
        i++;

    return left.data[i] < right.data[i];
}

bool operator<=(const SetElement &left, const SetElement &right)
{
    int i = 0;

    while (i < 80 && left.data[i] != 0 && right.data[i] != 0 && left.data[i] == right.data[i])
        i++;

    return left.data[i] <= right.data[i];
}

bool operator==(const SetElement &left, const SetElement &right)
{
    int i = 0;

    if (&left == &right)
        return true;

    while (i < 80 && left.data[i] != 0 && right.data[i] != 0 && left.data[i] == right.data[i])
        i++;

    return left.data[i] == right.data[i];
}

bool operator>(const SetElement &left, const SetElement &right)
{
    int i = 0;

    while (i < 80 && left.data[i] != 0 && right.data[i] != 0 && left.data[i] == right.data[i])
        i++;

    return left.data[i] > right.data[i];
}

int MySet::AddElement(const char *data)
{
    SetElement **tmp = &list;
    SetElement *newElement = new SetElement;

    int err = newElement->SetData(data);

    if (err)
        return err;

    if (list == nullptr)
    {
        newElement->next = newElement->prev = newElement;
        list = newElement;
        cardinality = 1;
        return 0;
    }

    while ((*tmp)->next != list && (*newElement) > (**tmp))
    {
        if ((*newElement) < (*(*tmp)->next))
            break;
        tmp = &(*tmp)->next;
    }

    if ((*newElement) == (**tmp))
        return SUCH_ELEMENT_ALREADY_EXISTS;

    if ((*newElement) < (**tmp))
    {
        newElement->next = (*tmp);
        newElement->prev = (*tmp)->prev;
        (*tmp)->prev = newElement;
        newElement->prev->next = newElement;

        if (*tmp == list)
            *tmp = newElement;
    }
    else
    {
        newElement->next = (*tmp)->next;
        newElement->prev = (*tmp);
        (*tmp)->next = newElement;
        newElement->next->prev = newElement;
    }

    cardinality++;
    return 0;
}

SetElement *SetElement::Next()
{
    return next;
}

std::ostream &operator<<(std::ostream &out, const MySet &set)
{
    if (set.list == nullptr)
    {
        out << "empty!\n";
        return out;
    }

    SetElement *tmp = set.list;
    while (tmp->Next() != set.list)
    {
        out << *tmp << ' ';
        tmp = tmp->Next();
    }

    out << *tmp << '\n';

    return out;
}

MySet::MySet() : list(nullptr), cardinality(0) {}

SetElement *MySet::FindElement(const char *value) const
{
    SetElement *tmp = list;

    SetElement newElement;
    newElement.SetData(value);

    if (list == nullptr)
        return nullptr;

    while (tmp->next != list && !(*tmp == newElement))
        tmp = tmp->next;

    if (*tmp == newElement)
        return tmp;

    return nullptr;
}

int MySet::DeleteElement(const char *value)
{
    SetElement *tmp = FindElement(value);

    if (tmp == nullptr)
        return 0;

    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;

    if (cardinality == 1)
    {
        cardinality = 0;
        delete list;
        list = nullptr;
        return 0;
    }

    if (tmp == list)
        list = tmp->next;

    cardinality--;
    delete tmp;
    return 0;
}

int MySet::Cardinality() const
{
    return cardinality;
}

int MySet::AddElement(SetElement element)
{
    return AddElement(element.Data());
}

MySet::MySet(const MySet &src)
{
    cardinality = 0;
    list = nullptr;
    SetElement *tmp = src.list;

    while (tmp->next != src.list)
    {
        AddElement(*tmp);
        tmp = tmp->next;
    }
    AddElement(*tmp);
}

MySet &Unite(const MySet &Set1, const MySet &Set2)
{
    MySet *newSet = new MySet(Set1);
    SetElement *tmp = Set2.list;

    while (tmp->Next() != Set2.list)
    {
        newSet->AddElement(*tmp);
        tmp = tmp->Next();
    }

    newSet->AddElement(*tmp);

    return *newSet;
}

MySet &Intersection(const MySet &Set1, const MySet &Set2)
{
    MySet *newSet = new MySet;
    SetElement *element1 = Set1.list, *element2 = Set2.list;

    while (true)
    {
        if (*element1 < *element2)
        {
            element1 = element1->Next();
        }
        else if (*element1 == *element2)
        {
            newSet->AddElement(*element2);
            element1 = element1->Next();
            element2 = element2->Next();
        }
        else
        {
            element2 = element2->Next();
        }
        if (element1 == Set1.list || element2 == Set2.list)
            break;
    }
    return *newSet;
}

MySet &Difference(const MySet &Set1, const MySet &Set2)
{
    MySet *newSet = new MySet;
    SetElement *element1 = Set1.list, *element2 = Set2.list;

    while (true)
    {
        if (*element1 < *element2)
        {
            newSet->AddElement(*element1);
            element1 = element1->Next();
        }
        else if (*element1 == *element2)
        {
            element1 = element1->Next();
            element2 = element2->Next();
        }
        else
        {
            element2 = element2->Next();
        }

        if (element1 == Set1.list)
            break;

        if (element2 == Set2.list)
        {
            while (element1 != Set1.list)
            {
                newSet->AddElement(*element1);
                element1 = element1->Next();
            }
            break;
        }
    }
    return *newSet;
}

MySet &SymDiff(const MySet &Set1, const MySet &Set2)
{
    MySet *newSet = new MySet;
    SetElement *element1 = Set1.list, *element2 = Set2.list;

    while (true)
    {
        if (*element1 < *element2)
        {
            newSet->AddElement(*element1);
            element1 = element1->Next();
        }
        else if (*element1 == *element2)
        {
            element1 = element1->Next();
            element2 = element2->Next();
        }
        else
        {
            newSet->AddElement(*element2);
            element2 = element2->Next();
        }

        if (element1 == Set1.list)
        {
            while (element2 != Set2.list)
            {
                newSet->AddElement(*element2);
                element2 = element2->Next();
            }
            break;
        }

        if (element2 == Set2.list)
        {
            while (element1 != Set1.list)
            {
                newSet->AddElement(*element1);
                element1 = element1->Next();
            }
            break;
        }
    }
    return *newSet;
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
    b.AddElement("gg");
    b.AddElement("ff");
    b.AddElement("uu");
    cout << b.Cardinality();

    cout << Difference(b, a);

    //cout << a;
    // a.DeleteElement("aa");
    // a.DeleteElement("b");
    // a.DeleteElement("e3");
    // a.DeleteElement("asdfas");
    // a.DeleteElement("fddgfsh");
    // cout << a.AddElement("e3");
    // cout << a.AddElement("b");
    // cout << a.AddElement("aa");
    // cout << a.AddElement("asdfas");
    // cout << a.AddElement("fddgfsh");

    //cout << a;
}
