#pragma once

#include <iostream>
#include <vector>

using namespace std;

class BuddyAllocator
{
public:
    BuddyAllocator(size_t levels, size_t minBlock);
    ~BuddyAllocator();

    void* mem_alloc(size_t size);
    void* mem_realloc(void* addr, size_t size);
    void mem_free(void* addr);

    // Explicitly prohibit copying or assigning an Allocator
    BuddyAllocator(const BuddyAllocator&) = delete;
    BuddyAllocator& operator=(const BuddyAllocator&) = delete;

    void mem_dump(std::ostream& log);

private:
    static const size_t InvalidIndex = (size_t)-1;

    struct BlockMetadata
    {
        size_t tocIndex;
        size_t level;
        void* data;

        BlockMetadata()
        {
            tocIndex = InvalidIndex;
            level = InvalidIndex;
            data = nullptr;
        }

        BlockMetadata(size_t tocIndex, size_t level, void* data)
        {
            this->tocIndex = tocIndex;
            this->level = level;
            this->data = data;
        }
    };

    size_t maxLevel;
    size_t minBlock;
    size_t totalSize;
    vector<bool> toc;
    void* buffer;

    size_t getLevelBlockSize(size_t level); // Size in bytes of a block at given level
    size_t getCeilLevel(size_t reqSize); // Minimal level which can contain reqSize bytes
    inline bool isFree(const BlockMetadata& block) { return !toc[block.tocIndex]; }
    
    BlockMetadata getRootBlock();
    BlockMetadata getRightBuddy(const BlockMetadata&);
    BlockMetadata getLeftBuddy(const BlockMetadata&);
    BlockMetadata getLeftChild(const BlockMetadata&);
    BlockMetadata getParent(const BlockMetadata&);
    BlockMetadata getFirstBlockLevel(size_t level);
    BlockMetadata getNextBlockLevel(const BlockMetadata&);
    bool acquire(const BlockMetadata&);
    bool release(const BlockMetadata&);

    bool split(const BlockMetadata&);
    bool merge(const BlockMetadata&);
};

