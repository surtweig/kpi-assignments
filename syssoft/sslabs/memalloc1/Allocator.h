#pragma once
#include <iostream>
#define ALLOCATOR_MAX_PAGES 16
#define ALLOCATOR_ALIGNMENT sizeof(uint32_t)
#define ALLOCATOR_BLOCK_HEADER_SIZE 2*ALLOCATOR_ALIGNMENT
#define ALLOCATOR_BLOCK_HEADER_FREE_FLAG_MASK (uint32_t)1
#define ALLOCATOR_BLOCK_HEADER_OFFSET_MASK (uint32_t)~1

class Allocator
{
public:
    Allocator(size_t pageSize);
    ~Allocator();

    void* mem_alloc(size_t size);
    void* mem_realloc(void* addr, size_t size);
    void mem_free(void* addr);

    // Explicitly prohibit copying or assigning an Allocator
    Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete;

    void mem_dump(std::ostream& log);

private:
    size_t pageSize;
    void* pages[ALLOCATOR_MAX_PAGES];

    static size_t alignedSize(size_t size) { return ((size - 1) / ALLOCATOR_ALIGNMENT + 1) * ALLOCATOR_ALIGNMENT; }

    void initPages();
    void destroyPages();

    size_t allocatePage();
    void deallocatePage(size_t index);

    // Tries to allocate a block on the page[index], returns pointer or nullptr if fails
    void* mem_alloc_page(size_t index, size_t size);

    // Reads block header, returns pointer to its data
    void* readBlockHeader(void* addr, bool& free, size_t& nextOffset, size_t& prevOffset);
    void writeBlockHeader(void* addr, bool free, size_t nextOffset, size_t prevOffset);

    void* getNextBlock(void* addr);
    void* getPrevBlock(void* addr);
    
    void setBlockState(void* addr, bool free);
    bool getBlockState(void* addr);

    void setBlockPrevOffset(void* addr, size_t prevOffset);
    void setBlockNextOffset(void* addr, size_t nextOffset);
    
    // Splits a block into head and tail, changes head size to newSize, returns pointer to tail
    void* splitBlock(void* addr, size_t newSize); 
    
    // Merges a sequence of blocks from firstBlock to lastBlock inclusively
    void mergeBlocks(void* firstBlock, void* lastBlock);
    
    // Merges two adjacent blocks
    void mergeAdjacentBlocks(void* firstBlock, void* secondBlock);
};

