#include <iostream>

class SetElement
{
private:
    SetElement *prev, *next;
    char data[81];

public:
    SetElement();
    ~SetElement();
    int SetData(const char *dat);
    const char *Data() const;
    SetElement *Next();

    friend bool operator<(SetElement, SetElement);
    friend bool operator<=(SetElement,SetElement);
    friend bool operator>(SetElement, SetElement);
    friend bool operator==(SetElement, SetElement);
    friend std::ostream &operator<<(std::ostream &out, const SetElement &element);
    friend class MySet;
};

class MySet
{
private:
    SetElement *list;
    int cardinality;

public:
    MySet();
    int AddElement(const char *value);
    SetElement *FindElement(const char *value) const;
    int DeleteElement(const char *value);
    int Cardinality()const;

    friend std::ostream &operator<<(std::ostream &out, const MySet &set);
    friend MySet Unite (MySet Set1,MySet Set2);
};