#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <cassert>
#include <QDebug>


int CompareStr(const char* s1, const char* s2);
int SimilarityStr(const char* s1, const char* s2);

struct DBRecordKey
{
    char* name;
    unsigned char nmod;

    DBRecordKey();
    DBRecordKey(const DBRecordKey& key);
    DBRecordKey(const char* name);
    DBRecordKey(const char* name, unsigned char nmod);
    ~DBRecordKey();

    void CopyName(const char* source);
    int Compare(const DBRecordKey& other) const;
    int CompareName(const DBRecordKey& other) const;
    int CompareName(const char* otherName) const;

    DBRecordKey& operator=(const DBRecordKey& other)
    {
        if (&other == this)
            return *this;
        delete name;
        CopyName(other.name);
        nmod = other.nmod;
        return *this;
    }

    friend bool operator<(const DBRecordKey& L, const DBRecordKey& R)
    {
        return L.Compare(R) < 0;
    }

    friend bool operator==(const DBRecordKey& L, const DBRecordKey& R)
    {
        return L.Compare(R) == 0;
    }
};

union DBRecordData
{
    float vector[3];
    struct Coordinates
    {
        float x;
        float y;
        float z;
    } coordinates;
};

template <typename TDBRecordData> class Database;

template <typename TDBRecordData>
struct DBRecord
{
public:
    TDBRecordData data;

    DBRecord();
    DBRecord(const char* name);
    DBRecord(const DBRecord& rec);
    void Clear();

    inline DBRecordKey Key() const { return DBRecordKey(key); }
    char* Name() const;
    inline int Nmod() const { return key.nmod; }
    inline bool IsValid() const { return isValid; }
    inline size_t Address() const { return address; }
private:
    DBRecordKey key;
    bool isValid;
    size_t address;
    size_t sortedIndex;

    friend class Database<TDBRecordData>;
};

template <typename TDBRecordData>
class Database
{
private:
    DBRecord<TDBRecordData>* table;
    DBRecord<TDBRecordData>** sorted;
    size_t counter;
    size_t appendPos;

    void sortIndices(size_t first, size_t last);
    bool insert(const DBRecord<TDBRecordData>& rec, size_t address);
    void insertSorted(size_t index, DBRecord<TDBRecordData>* rec);
    void removeSorted(size_t index);
    size_t findSortedIndex(const DBRecordKey& key, size_t first, size_t last);

public:
    static const unsigned long MaxRecords = 100;
    static const size_t InvalidAddress = size_t(-1);

    Database();
    ~Database();
    void Clear();
    void rebuildIndices();

    inline size_t GetCounter() { return counter; }
    inline DBRecord<TDBRecordData>* GetRecord(size_t index) { return sorted[index]; }

    // Direct address operations
    size_t Append(const char* name, const TDBRecordData data);
    DBRecord<TDBRecordData>* Select(size_t address);
    bool Delete(size_t address);

    // Access by key
    DBRecord<TDBRecordData>* Select(const DBRecordKey& key);
    DBRecord<TDBRecordData>* SelectFirst(const char* name);
    DBRecord<TDBRecordData>* SelectNext(const DBRecordKey& key);
    bool Delete(const DBRecordKey& key);

    // Search similar name
    DBRecord<TDBRecordData>* Search(const char* name);
    DBRecord<TDBRecordData>* SearchNext(const DBRecord<TDBRecordData>* rec, int step = 1);
};

template class Database<DBRecordData>;
template struct DBRecord<DBRecordData>;

#endif // DATABASE_H
