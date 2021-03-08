#include <iostream>
#include <cstring>

class SetElement
{
private:
    SetElement *prev, *next;
    char data[81];

public:
    SetElement();
    SetElement(const SetElement &);
    ~SetElement();
    int SetData(const char *dat);
    const char *Data() const;
    SetElement *Next();
    SetElement *Prev();
    void SetNext(SetElement *);
    void SetPrev(SetElement *);

    friend bool operator<(const SetElement &left, const SetElement &right);
    friend bool operator<=(const SetElement &left, const SetElement &right);
    friend bool operator>(const SetElement &left, const SetElement &right);
    friend bool operator==(const SetElement &left, const SetElement &right);
    friend std::ostream &operator<<(std::ostream &out, const SetElement &element);
    friend class MySet;
};

class MySet
{
private:
    SetElement *list;
    int cardinality;
    std::string name;
    MySet *next, *prev;

public:
    MySet(const std::string &setName);
    MySet(const MySet &);
    ~MySet();
    int AddElement(const char *value);
    int AddElement(SetElement);
    SetElement *FindElement(const char *value) const;
    int DeleteElement(const char *value);
    int Cardinality() const;
    bool IsSubset(const MySet &src) const;
    bool CheckInclusion(const MySet &src) const;
    void SetName(const std::string &SetName);
    bool operator<(const MySet &src);
    bool operator>(const MySet &src);
    bool operator==(const MySet &src);
    MySet *Next();
    MySet *Prev();
    void SetNext(MySet *);
    void SetPrev(MySet *);
    std::string Name();
    int pushBack(const SetElement& element);
    int pushFront(const SetElement& element);

    friend std::ostream &operator<<(std::ostream &out, const MySet &set);
    friend MySet Unite(const MySet &Set1, const MySet &Set2, const std::string &setName);
    friend MySet Intersection(const MySet &Set1, const MySet &Set2, const std::string &setName);
    friend MySet Difference(const MySet &Set1, const MySet &Set2, const std::string &setName);
    friend MySet SymDiff(const MySet &Set1, const MySet &Set2, const std::string &setName);
};

class SetsList
{
private:
    MySet *list;
    int counter;

public:
    SetsList();
    ~SetsList();
    int AddSet(const MySet &set);
    void DeleteSet(std::string);
    MySet *FindSet(std::string);

    friend std::ostream &operator<<(std::ostream &out, const SetsList &lst);
};