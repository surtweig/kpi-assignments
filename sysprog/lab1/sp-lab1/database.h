#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct DBRecordKey
{
    char* name;
    unsigned char nmod;

    DBRecordKey();
    DBRecordKey(const DBRecordKey& key);

    void CopyName(const char* source);
    int Compare(const DBRecordKey& other) const;

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

struct DBRecord
{
public:
    DBRecordData data;

    DBRecord();
    DBRecord(const char* name);
    DBRecord(const DBRecord& rec);
    void Clear();

    inline DBRecordKey Key() { return DBRecordKey(key); }
    char* Name();
    inline int Nmod() { return key.nmod; }
    inline bool IsValid() { return isValid; }
    inline size_t Address() { return address; }
private:
    DBRecordKey key;
    bool isValid;
    size_t address;

    friend class Database;
};

int CompareStr(char* s1, char* s2);

class Database
{
private:
    DBRecord* table;
    DBRecord** sorted;
    size_t counter;
    size_t appendPos;

    void rebuildIndices();
    void sortIndices(size_t first, size_t last);
    inline size_t getRecordsCounter() { return counter; }
    bool insert(const DBRecord& rec, size_t address);
    void insertSorted(size_t index, DBRecord* rec);
    void removeSorted(size_t index);
    size_t findSortedIndex(const DBRecordKey& key, size_t first, size_t last);

public:
    static const unsigned long MaxRecords = 100;
    static const size_t InvalidAddress = size_t(-1);

    Database();
    ~Database();
    void Clear();

    // Direct address operations
    DBRecord* Select(size_t address);
    size_t Append(const DBRecord& rec);
    bool Delete(size_t address);

    // Access by key
    DBRecord* Select(const DBRecordKey& key);
    DBRecord* SelectFirst(char* name);
    DBRecord* SelectNext(const DBRecordKey& key);
    bool Delete(const DBRecordKey& key);
};

#endif // DATABASE_H
