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

bool operator<(SetElement left, SetElement right)
{
    int i = 0;

    while (i < 80 && left.data[i] != 0 && right.data[i] != 0 && left.data[i] == right.data[i])
        i++;

    return left.data[i] < right.data[i];
}

bool operator<=(SetElement left, SetElement right)
{
    int i = 0;

    while (i < 80 && left.data[i] != 0 && right.data[i] != 0 && left.data[i] == right.data[i])
        i++;

    return left.data[i] <= right.data[i];
}

bool operator==(SetElement left, SetElement right)
{
    int i = 0;

    while (i < 80 && left.data[i] != 0 && right.data[i] != 0 && left.data[i] == right.data[i])
        i++;

    return left.data[i] == right.data[i];
}

bool operator>(SetElement left, SetElement right)
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

MySet::MySet() : list(nullptr), cardinality(0){}

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

int MySet::Cardinality()const
{
    return cardinality;
}

int main()
{
    MySet a;
    a.AddElement("e3");
    a.AddElement("b");
    a.AddElement("aa");
    a.AddElement("asdfas");
    a.AddElement("fddgfsh");
    a.DeleteElement("aa");
    a.DeleteElement("b");
    a.DeleteElement("e3");
    a.DeleteElement("asdfas");
    a.DeleteElement("fddgfsh");
    a.AddElement("e3");
    a.AddElement("b");
    a.AddElement("aa");
    a.AddElement("asdfas");
    a.AddElement("fddgfsh");

    cout << a;
}
