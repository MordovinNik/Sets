#include "Set.h"

#define INVALID_SYMBOL 1
#define TOO_LONG_INPUT 2
#define SUCH_ELEMENT_ALREADY_EXISTS 3
#define NO_SUCH_ELEMENT_IN_SET 4
#define SUCH_SET_ALREADY_EXISTS 5

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
    out << '\"' << element.data << '\"';
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

SetElement *SetElement::Next()
{
    return next;
}

SetElement *SetElement::Prev()
{
    return prev;
}

void SetElement::SetNext(SetElement *ptr)
{
    next = ptr;
}

void SetElement::SetPrev(SetElement *ptr)
{
    prev = ptr;
}

///////////////////////////////////////////////////////////////////////////////

MySet::MySet(const std::string &setName) : list(nullptr), cardinality(0), name(setName), next(this), prev(this) {}

MySet::MySet(const MySet &src)
{
    cardinality = 0;
    list = nullptr;
    SetElement *tmp = src.list;
    name = src.name;
    next = prev = this;

    if (src.list != nullptr)
    {
        while (tmp->next != src.list)
        {
            AddElement(*tmp);
            tmp = tmp->next;
        }
        AddElement(*tmp);
    }
}

MySet::~MySet()
{
    if (list == nullptr)
        return;

    SetElement *tmp = list;

    while (tmp->next != list)
    {
        tmp = tmp->next;
        delete tmp->prev;
    }

    delete tmp;
}

bool MySet::operator<(const MySet &src)
{
    return this->name < src.name;
}

bool MySet::operator>(const MySet &src)
{
    return this->name > src.name;
}

bool MySet::operator==(const MySet &src)
{
    return this->name == src.name;
}

int MySet::AddElement(const char *data)
{
    SetElement *newElement = new SetElement;
    int err = newElement->SetData(data);

    if (err)
    {
        delete newElement;
        return err;
    }

    if (list == nullptr)
    {
        cardinality = 1;
        list = newElement;
        newElement->next = newElement->prev = newElement;
        return 0;
    }

    SetElement *tmp = list->prev;

    do
    {
        if (*newElement == *tmp->Next())
        {
            delete newElement;
            return SUCH_ELEMENT_ALREADY_EXISTS;
        }

        if (*newElement < *tmp->Next())
            break;
        tmp = tmp->Next();

    } while (tmp->Next() != list);

    if (tmp->Next() == list && *newElement < *list)
        list = newElement;

    newElement->SetPrev(tmp);
    newElement->SetNext(tmp->Next());
    tmp->Next()->SetPrev(newElement);
    tmp->SetNext(newElement);
    cardinality++;

    return 0;
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
        out << *tmp << '\n';
        tmp = tmp->Next();
    }

    out << *tmp << '\n';

    return out;
}

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

void MySet::SetName(const std::string &SetName)
{
    name = SetName;
}

MySet Unite(const MySet &Set1, const MySet &Set2, const std::string &setName)
{
    if (!Set2.cardinality)
    {
        MySet newSet(Set1);
        newSet.SetName(setName);
        return newSet;
    }

    if (!Set1.cardinality)
    {
        MySet newSet(Set2);
        newSet.SetName(setName);
        return newSet;
    }

    MySet newSet(Set1);
    newSet.SetName(setName);

    SetElement *tmp = Set2.list;

    while (tmp->Next() != Set2.list)
    {
        newSet.pushBack(*tmp);
        tmp = tmp->Next();
    }

    newSet.pushBack(*tmp);

    return newSet;
}

MySet Intersection(const MySet &Set1, const MySet &Set2, const std::string &setName)
{
    if (!Set2.cardinality)
    {
        MySet newSet(Set2);
        newSet.SetName(setName);
        return newSet;
    }

    if (!Set1.cardinality)
    {
        MySet newSet(Set1);
        newSet.SetName(setName);
        return newSet;
    }

    MySet newSet(setName);
    SetElement *element1 = Set1.list, *element2 = Set2.list;

    if (element1 == nullptr || element2 == nullptr)
        return newSet;
    while (true)
    {
        if (*element1 < *element2)
        {
            element1 = element1->Next();
        }
        else if (*element1 == *element2)
        {
            newSet.pushBack(*element2);
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
    return newSet;
}

MySet Difference(const MySet &Set1, const MySet &Set2, const std::string &setName)
{
    if (!Set1.cardinality || !Set2.cardinality)
    {
        MySet newSet(Set1);
        newSet.SetName(setName);
        return newSet;
    }
    MySet newSet(setName);
    SetElement *element1 = Set1.list, *element2 = Set2.list;

    while (true)
    {
        if (*element1 < *element2)
        {
            newSet.pushBack(*element1);
            element1 = element1->Next();
        }
        else
        {
            if (*element1 == *element2)
            {
                element1 = element1->Next();
                element2 = element2->Next();
            }
            else
            {
                element2 = element2->Next();
            }

            if (element2 == Set2.list)
            {
                while (element1 != Set1.list)
                {
                    newSet.pushBack(*element1);
                    element1 = element1->Next();
                }
                break;
            }
        }
        if (element1 == Set1.list)
            break;
    }
    return newSet;
}

MySet SymDiff(const MySet &Set1, const MySet &Set2, const std::string &setName)
{
    if (!Set1.cardinality)
    {
        MySet newSet(Set2);
        newSet.SetName(setName);
        return newSet;
    }

    if (!Set2.cardinality)
    {
        MySet newSet(Set1);
        newSet.SetName(setName);
        return newSet;
    }

    MySet newSet(setName);
    SetElement *element1 = Set1.list, *element2 = Set2.list;

    while (true)
    {
        if (*element1 < *element2)
        {
            newSet.pushBack(*element1);
            element1 = element1->Next();
        }
        else
        {
            if (*element1 == *element2)
            {
                element1 = element1->Next();
                element2 = element2->Next();
            }
            else
            {
                newSet.pushBack(*element2);
                element2 = element2->Next();
            }

            if (element2 == Set2.list)
            {
                while (element1 != Set1.list)
                {
                    newSet.pushBack(*element1);
                    element1 = element1->Next();
                }
                break;
            }
        }

        if (element1 == Set1.list)
        {
            while (element2 != Set2.list)
            {
                newSet.pushBack(*element2);
                element2 = element2->Next();
            }
            break;
        }
    }
    return newSet;
}

bool MySet::CheckInclusion(const MySet &src) const
{
    if (!src.cardinality)
        return true;
        
    if (!this->cardinality)
        return false;

    SetElement *element1 = src.list, *element2 = this->list;
    while (true)
    {
        if (*element1 == *element2)
        {
            element1 = element1->Next();
            element2 = element2->Next();

            if (element1 == src.list)
                return true;
        }
        else
        {
            element2 = element2->Next();
        }

        if (element2 == this->list)
            return false;
    }
}

MySet *MySet::Next()
{
    return next;
}

MySet *MySet::Prev()
{
    return prev;
}

void MySet::SetNext(MySet *ptr)
{
    next = ptr;
}

void MySet::SetPrev(MySet *ptr)
{
    prev = ptr;
}

std::string MySet::Name()
{
    return name;
}

int MySet::pushBack(const SetElement &element)
{
    SetElement *newElement = new SetElement(element);

    if (list == nullptr)
    {
        cardinality = 1;
        list = newElement;
        newElement->next = newElement->prev = newElement;
        return 0;
    }

    newElement->next = list;
    newElement->prev = list->prev;
    list->prev->next = newElement;
    list->prev = newElement;
    return 0;
}

int MySet::pushFront(const SetElement &element)
{
    pushBack(element);
    list = list->prev;
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

SetsList::SetsList() : list(nullptr), counter(0) {}

SetsList::~SetsList()
{
    if (list == nullptr)
        return;

    MySet *tmp = list;

    while (tmp->Next() != list)
    {
        tmp = tmp->Next();
        delete tmp->Prev();
    }
    delete tmp;
}

int SetsList::AddSet(const MySet &set)
{
    MySet *newSet = new MySet(set);

    if (list == nullptr)
    {
        list = newSet;
        newSet->SetNext(newSet);
        newSet->SetPrev(newSet);
        counter = 1;
        return 0;
    }

    MySet *tmp = list->Prev();

    do
    {
        if (*newSet == *tmp->Next())
        {
            delete newSet;
            return SUCH_SET_ALREADY_EXISTS;
        }

        if (*newSet < *tmp->Next())
            break;
        tmp = tmp->Next();

    } while (tmp->Next() != list);

    if (tmp->Next() == list && *newSet < *list)
        list = newSet;

    newSet->SetPrev(tmp);
    newSet->SetNext(tmp->Next());
    tmp->Next()->SetPrev(newSet);
    tmp->SetNext(newSet);
    counter++;

    return 0;
}

MySet *SetsList::FindSet(std::string name)
{
    MySet *tmp = list;
    if (list == nullptr)
        return nullptr;

    do
    {
        if (tmp->Name() == name)
            return tmp;
        tmp = tmp->Next();
    } while (tmp != list);

    return nullptr;
}

void SetsList::DeleteSet(std::string name)
{
    MySet *tmp = FindSet(name);

    if (tmp == nullptr)
        return;

    tmp->Next()->SetPrev(tmp->Prev());
    tmp->Prev()->SetNext(tmp->Next());

    if (counter == 1)
    {
        counter = 0;
        delete list;
        list = nullptr;
        return;
    }

    if (tmp == list)
        list = list->Next();

    counter--;
    delete tmp;
}

std::ostream &operator<<(std::ostream &out, const SetsList &lst)
{
    if (lst.list == nullptr)
    {
        out << "empty!\n";
        return out;
    }

    MySet *tmp = lst.list;
    while (tmp->Next() != lst.list)
    {
        out << tmp->Name() << std::endl;
        tmp = tmp->Next();
    }

    out << tmp->Name() << '\n';

    return out;
}