#include <iostream>

class SetElement
{
private:
    SetElement *prev, *next;
    char data[81];

public:
    SetElement();
    SetElement(const SetElement&);
    ~SetElement();
    int SetData(const char *dat);
    const char *Data() const;
    SetElement *Next();

    friend bool operator<(const SetElement &left, const SetElement &right);
    friend bool operator<=(const SetElement &left, const SetElement &right);
    friend bool operator>(const SetElement &left, const SetElement &right);
    friend bool operator==(const SetElement &left, const SetElement& right);
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
    MySet(const MySet&);
    int AddElement(const char *value);
    int AddElement(SetElement);
    SetElement *FindElement(const char *value) const;
    int DeleteElement(const char *value);
    int Cardinality()const;
    bool IsSubset(const MySet& src)const;

    friend std::ostream &operator<<(std::ostream &out, const MySet &set);
    friend MySet& Unite (const MySet& Set1,const MySet& Set2);
    friend MySet& Intersection(const MySet& Set1,const MySet& Set2);
    friend MySet& Difference(const MySet &Set1, const MySet &Set2);
    friend MySet &SymDiff(const MySet &Set1, const MySet &Set2);
};