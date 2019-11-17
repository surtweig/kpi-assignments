#include "database.h"

int CompareStr(const char* s1, const char* s2)
{
    int i = 0;
    while (true)
    {
        int c1 = toupper(s1[i]);
        int c2 = toupper(s2[i]);
        if (c1 == c2)
        {
            c1 = s1[i];
            c2 = s2[i];
        }
        if (c1 != c2 || s1[i] == 0)
            return c1-c2;
        ++i;
    }
}

int SimilarityStr(const char* s1, const char* s2)
{
    int i = 0;
    while (true)
    {
        int c1 = toupper(s1[i]);
        int c2 = toupper(s2[i]);
        if (c1 != c2 || s1[i] == 0)
            break;
        ++i;
    }
    return i;
}

DBRecordKey::DBRecordKey()
{
    name = nullptr;
    nmod = 0;
}

DBRecordKey::DBRecordKey(const DBRecordKey& key)
{
    name = nullptr;
    CopyName(key.name);
    nmod = key.nmod;
}

DBRecordKey::DBRecordKey(const char* name)
{
    this->name = nullptr;
    CopyName(name);
    nmod = 0;
}

DBRecordKey::DBRecordKey(const char* name, unsigned char nmod)
{
    this->name = nullptr;
    CopyName(name);
    this->nmod = nmod;
}

void DBRecordKey::CopyName(const char* source)
{
    delete name;
    if (source)
    {
        name = new char[strlen(source)];//(char*)malloc(strlen(source));
        strcpy(name, source);
    }
    else
        name = nullptr;
}

int DBRecordKey::Compare(const DBRecordKey& other) const
{
    int namecmp = CompareStr(name, other.name);
    if (namecmp == 0)
        return nmod - other.nmod;
    else
        return namecmp;
}

DBRecordKey::~DBRecordKey()
{
    delete name;
}

int DBRecordKey::CompareName(const DBRecordKey& other) const
{
    return CompareStr(name, other.name);
}

int DBRecordKey::CompareName(const char* otherName) const
{
    return CompareStr(name, otherName);
}

template <typename TDBRecordData>
DBRecord<TDBRecordData>::DBRecord()
{
    Clear();
}

template <typename TDBRecordData>
DBRecord<TDBRecordData>::DBRecord(const char* name)
{
    Clear();
    key.CopyName(name);
    isValid = true;
}

template <typename TDBRecordData>
DBRecord<TDBRecordData>::DBRecord(const DBRecord<TDBRecordData>& rec)
{
    Clear();
    key.CopyName(rec.key.name);
    data = rec.data;
    isValid = true;
}

template <typename TDBRecordData>
void DBRecord<TDBRecordData>::Clear()
{
    delete key.name;
    key.name = nullptr;
    key.nmod = 0;
    isValid = false;
    address = Database<TDBRecordData>::InvalidAddress;
    sortedIndex = Database<TDBRecordData>::InvalidAddress;
}

template <typename TDBRecordData>
char* DBRecord<TDBRecordData>::Name() const
{
    char* name = new char[strlen(key.name)];//(char*)malloc(strlen(key.name));
    strcpy(name, key.name);
    return name;
}

template <typename TDBRecordData>
Database<TDBRecordData>::Database()
{
    table = new DBRecord<TDBRecordData>[MaxRecords];//(DBRecord*)malloc(sizeof(DBRecord)*MaxRecords);
    sorted = new DBRecord<TDBRecordData>*[MaxRecords];//(DBRecord**)malloc(sizeof(DBRecord*)*MaxRecords);
    Clear();
}

template <typename TDBRecordData>
void Database<TDBRecordData>::Clear()
{
    for (size_t i = 0; i < MaxRecords; i++)
    {
        table[i].Clear();
        sorted[i] = nullptr;
    }
    counter = 0;
    appendPos = 0;
}

template <typename TDBRecordData>
Database<TDBRecordData>::~Database()
{
    delete[] table;
    delete[] sorted;
}

template <typename TDBRecordData>
void Database<TDBRecordData>::insertSorted(size_t index, DBRecord<TDBRecordData>* rec)
{
    DBRecord<TDBRecordData>* prev = rec;
    DBRecord<TDBRecordData>* swap;
    for (size_t i = index; i < counter+1; i++)
    {
        swap = sorted[i];
        sorted[i] = prev;
        sorted[i]->sortedIndex = i;
        prev = swap;
    }
}

template <typename TDBRecordData>
void Database<TDBRecordData>::removeSorted(size_t index)
{
    for (size_t i = index; i < counter; i++)
    {
        sorted[i] = sorted[i+1];
        if (sorted[i])
            sorted[i]->sortedIndex = i;
    }
}

template <typename TDBRecordData>
size_t Database<TDBRecordData>::findSortedIndex(const DBRecordKey& key, size_t first, size_t last)
{
    qDebug() << "findSortedIndex:" << key.name << "#" << key.nmod << "through" << first << "-" << last;
    QString s = "   sorted["+QString::number(first)+":"+QString::number(last)+"] = ";
    for (size_t i = first; i <= last; ++i)
    {
        s += QString::number(i);
        s += ":";
        s += sorted[i]->key.name;
        s += "#";
        s += QString::number(sorted[i]->key.nmod);
        s += " ";
    }
    qDebug().noquote() << s;

    if (first > last || last >= counter)
    {
        qDebug() << "   InvalidAddress";
        return InvalidAddress;
    }

    if (sorted[first]->key == key)
    {
        qDebug() << "   returning first index (" << first << ") exact match";
        return first;
    }

    if (sorted[last]->key == key)
    {
        qDebug() << "   returning last index (" << last << ") exact match";
        return last;
    }

    if (last-first <= 0)
    {
        if (key < sorted[last]->Key())
        {
            qDebug() << "   returning last(first) index (" << last << ") key < last";
            return last;
        }
        if (sorted[first]->Key() < key)
        {
            qDebug() << "   returning first(last) index (" << first << ") first < key";
            return first;
        }
    }

    size_t mid = (first+last)/2 + 1;
    if (sorted[mid-1]->key < key)
    {
        return findSortedIndex(key, mid, last);
    }
    else
    {
        return findSortedIndex(key, first, mid-1);
    }
}

//           i               i+1
//                   j
//
//           mid = (i+i+1)>>1 = i

template <typename TDBRecordData>
void Database<TDBRecordData>::rebuildIndices()
{
    counter = 0;
    for (size_t i = 0; i < MaxRecords; i++)
        if (table[i].IsValid())
            sorted[counter++] = &table[i];

    for (size_t i = counter; i < MaxRecords; i++)
        sorted[i] = nullptr;

    sortIndices(0, counter-1);
}

template <typename TDBRecordData>
void Database<TDBRecordData>::sortIndices(size_t first, size_t last)
{
    if (first < last)
    {
        size_t count = last-first+1;
        if (count == 2)
        {
            if (sorted[last]->key < sorted[first]->key)
            {
                DBRecord<TDBRecordData>* t = sorted[last];
                sorted[last] = sorted[first];
                sorted[last]->sortedIndex = last;
                sorted[first] = t;
                sorted[first]->sortedIndex = first;
            }
        }
        else
        {
            DBRecord<TDBRecordData>* pivot = sorted[first + (last-first)/2];
            size_t i = first;
            size_t j = last;
            while (true)
            {
                while (sorted[i]->key < pivot->key)
                    ++i;
                while (pivot->key < sorted[j]->key)
                    --j;
                if (i >= j)
                    break;
                DBRecord<TDBRecordData>* t = sorted[i];
                sorted[i] = sorted[j];
                sorted[i]->sortedIndex = i;
                sorted[j] = t;
                sorted[j]->sortedIndex = j;
            }

            sortIndices(first, j);
            sortIndices(j+1, last);
        }
    }
}

template <typename TDBRecordData>
DBRecord<TDBRecordData>* Database<TDBRecordData>::Select(size_t address)
{
    if (address >= MaxRecords)
        return nullptr;
    return &table[address];
}

template <typename TDBRecordData>
bool Database<TDBRecordData>::insert(const DBRecord<TDBRecordData>& rec, size_t address)
{
    if (address >= MaxRecords)
        return false;
    if (counter == MaxRecords)
        return false;
    size_t pos = address;
    DBRecord<TDBRecordData> prev = rec;
    DBRecord<TDBRecordData> swap;
    while (table[pos].IsValid())
    {
        swap = table[pos];
        table[pos] = prev;
        prev = swap;
        pos++;
        if (pos >= MaxRecords)
            break;
    }
    counter++;
    return true;
}

template <typename TDBRecordData>
size_t Database<TDBRecordData>::Append(const char* name, const TDBRecordData data)
{
    if (counter == MaxRecords)
        return InvalidAddress;

    size_t start = appendPos;
    while (table[counter].IsValid())
    {
        appendPos++;
        if (appendPos >= MaxRecords)
            appendPos = 0;
        if (appendPos == start)
            break;
    }

    if (table[appendPos].IsValid())
        return InvalidAddress;
    else
    {
        unsigned char n = 0;
        if (counter > 0)
        {
            DBRecord<TDBRecordData>* collision = SelectFirst(name);
            while (collision != nullptr)
            {
                if (collision->key.nmod != n)
                    break;
                collision = SelectNext(collision->key);
                n++;
                if (n == 0 && collision == nullptr)
                    return InvalidAddress;
            }
        }
        table[appendPos] = DBRecord<TDBRecordData>(name);
        table[appendPos].key.nmod = n;
        table[appendPos].data = data;
        table[appendPos].address = appendPos;

        if (counter > 0)
        {
            size_t insertSortedPos = findSortedIndex(table[appendPos].key, 0, counter-1);

            if (table[appendPos].key < sorted[insertSortedPos]->key)
                insertSorted(insertSortedPos, &table[appendPos]);
            else
                insertSorted(insertSortedPos+1, &table[appendPos]);
        }
        else
            insertSorted(0, &table[appendPos]);

        counter++;
        appendPos++;
        //rebuildIndices();
        return appendPos-1;
    }
}

template <typename TDBRecordData>
bool Database<TDBRecordData>::Delete(size_t address)
{
    if (address >= MaxRecords)
        return false;
    bool wasValid = table[address].IsValid();
    if (wasValid)
    {
        removeSorted(findSortedIndex(table[address].key, 0, counter-1));
        --counter;
    }
    table[address].Clear();
    return wasValid;
}

template <typename TDBRecordData>
DBRecord<TDBRecordData>* Database<TDBRecordData>::Select(const DBRecordKey& key)
{
    size_t index = findSortedIndex(key, 0, counter-1);
    if (index < counter)
    {
        if (key == sorted[index]->Key())
            return sorted[index];
        else
            return nullptr;
    }
    else
        return nullptr;
}

template <typename TDBRecordData>
DBRecord<TDBRecordData>* Database<TDBRecordData>::SelectFirst(const char* name)
{
    DBRecordKey key;
    key.CopyName(name);
    key.nmod = 0;
    size_t index = findSortedIndex(key, 0, counter-1);
    if (index < counter)
    {
        if (key.CompareName(sorted[index]->Key()) == 0)
            return sorted[index];
        else
            return nullptr;
    }
    else
        return nullptr;
}

template <typename TDBRecordData>
DBRecord<TDBRecordData>* Database<TDBRecordData>::SelectNext(const DBRecordKey& key)
{
    DBRecordKey keynext = DBRecordKey(key.name, key.nmod+1);
    if (keynext.nmod != 0)
        return Select(keynext);
    else
        return nullptr;
}

template <typename TDBRecordData>
bool Database<TDBRecordData>::Delete(const DBRecordKey& key)
{
    size_t index = findSortedIndex(key, 0, counter-1);
    if (index == InvalidAddress)
        return false;
    if (sorted[index]->Key() == key)
    {
        sorted[index]->Clear();
        removeSorted(index);
        --counter;
        return true;
    }
    else
        return false;
}

template <typename TDBRecordData>
DBRecord<TDBRecordData>* Database<TDBRecordData>::Search(const char* name)
{
    DBRecordKey key(name);
    size_t index = findSortedIndex(key, 0, counter-1);
    if (index == InvalidAddress)
        return nullptr;
    if (SimilarityStr(sorted[index]->Key().name, name) > 0)
    {
        return sorted[index];
    }
    else
    {
        --index;
        if (index == InvalidAddress)
            return nullptr;
        if (SimilarityStr(sorted[index]->Key().name, name) > 0)
        {
            return sorted[index];
        }
        else
            return nullptr;
    }
}

template <typename TDBRecordData>
DBRecord<TDBRecordData>* Database<TDBRecordData>::SearchNext(const DBRecord<TDBRecordData>* rec, int step)
{
    if (rec == nullptr)
        return nullptr;

    size_t nextIndex = rec->sortedIndex+step;
    if (nextIndex >= counter)
        return nullptr;

    if (sorted[nextIndex] == nullptr)
        return nullptr;

    if (SimilarityStr(sorted[nextIndex]->Key().name, rec->Key().name) > 0)
    {
        return sorted[nextIndex];
    }
    else
        return nullptr;
}
