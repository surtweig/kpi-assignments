#include "Allocator.h"
#include <cassert>
#include "../massert.h"

Allocator::Allocator(size_t pageSize)
{
    // Actual page size will be nearest bigger multiple of ALLOCATOR_ALIGNMENT
    this->pageSize = alignedSize(pageSize);
    assert(this->pageSize >= 2*ALLOCATOR_BLOCK_HEADER_SIZE);
    
    initPages();
}

Allocator::~Allocator()
{
    destroyPages();
}

void* Allocator::mem_alloc(size_t size)
{
    size = alignedSize(size);

    if (size == 0)
        return nullptr;
    // upper limit for data size
    if (size > pageSize - 2 * ALLOCATOR_BLOCK_HEADER_SIZE)
        return nullptr;

    for (int i = 0; i < ALLOCATOR_MAX_PAGES; ++i)
    {
        void* addr = mem_alloc_page(i, size);
        if (addr)
            return addr;
    }

    // retry with new page
    size_t newPage = allocatePage();
    if (newPage < ALLOCATOR_MAX_PAGES)
        return mem_alloc_page(newPage, size);

    return nullptr;
}

void* Allocator::mem_realloc(void* addr, size_t size)
{
    size = alignedSize(size);
    if (size == 0)
    {
        //mem_free(addr);
        return nullptr;
    }
    if (size > pageSize - 2 * ALLOCATOR_BLOCK_HEADER_SIZE)
        return nullptr;

    bool free;
    size_t nextOffset;
    size_t prevOffset;
    readBlockHeader(addr, free, nextOffset, prevOffset);
    size_t currentSize = nextOffset - ALLOCATOR_BLOCK_HEADER_SIZE;
    
    if (currentSize == size) // New size is the same
        return addr;
    else if (currentSize > size) // Shrink
    {
        size_t freedSize = currentSize - size;
        if (freedSize <= ALLOCATOR_BLOCK_HEADER_SIZE) // Ignore shrink request if freed space couldn't contain minimal block
            return nullptr;

        void* nextBlock = getNextBlock(addr);
        size_t newNextOffset = nextOffset - freedSize;
        
        setBlockNextOffset(addr, newNextOffset); // Shrinking given block
        
        void* freedBlock = (void*)((size_t)addr + newNextOffset);
        writeBlockHeader(freedBlock, true, freedSize, newNextOffset); // Creating new block from freed memory
        
        setBlockPrevOffset(nextBlock, freedSize); // Notifying next block that it has new neighbour
        if (getBlockState(nextBlock)) 
            mergeAdjacentBlocks(freedBlock, nextBlock); // Merging new and next block if the next block is free
        
        return addr;
    }
    else // Extend
    {
        void* nextBlock = getNextBlock(addr);
        bool nextFree;
        size_t nextNextOffset;
        size_t nextPrevOffset;
        readBlockHeader(nextBlock, nextFree, nextNextOffset, nextPrevOffset);

        void* prevBlock = getPrevBlock(addr);
        bool prevFree = false;
        size_t prevNextOffset;
        size_t prevPrevOffset;
        if (prevBlock)
            readBlockHeader(prevBlock, prevFree, prevNextOffset, prevPrevOffset);

        void* blockAfterNext = getNextBlock(nextBlock);

        if (nextFree && nextNextOffset >= size - currentSize) // Next block is free and has enough space
        {
            size_t newNextOffset = size + ALLOCATOR_BLOCK_HEADER_SIZE;
            size_t newNextNextOffset = nextNextOffset - size + currentSize;
            if (newNextNextOffset <= ALLOCATOR_BLOCK_HEADER_SIZE)
            {
                newNextOffset += newNextNextOffset;
                newNextNextOffset = 0;
                if (blockAfterNext)
                    setBlockPrevOffset(blockAfterNext, newNextOffset);
            }
            else
            {
                writeBlockHeader((void*)((size_t)addr + newNextOffset), true, newNextNextOffset, newNextOffset);
                if (blockAfterNext)
                    setBlockPrevOffset(blockAfterNext, newNextNextOffset);
            }
            setBlockNextOffset(addr, newNextOffset);
            return addr;
        }
        else if (prevFree && prevNextOffset >= size - currentSize) // Prev block is free and has enough space
        {
            size_t newNextOffset = size + ALLOCATOR_BLOCK_HEADER_SIZE;
            memmove(prevBlock, addr, currentSize);
            writeBlockHeader(prevBlock, false, newNextOffset, prevPrevOffset);
            void* newBlock = (void*)((size_t)prevBlock + newNextOffset);
            size_t newBlockNextOffset = prevNextOffset + currentSize + ALLOCATOR_BLOCK_HEADER_SIZE - newNextOffset;
            writeBlockHeader(newBlock, true, newBlockNextOffset, newNextOffset);
            setBlockPrevOffset(nextBlock, newBlockNextOffset);
            return prevBlock;
        }
        else if (prevFree && nextFree) // Both neighbours are free, but neither have enough space by themselves
        {
            if (prevNextOffset + currentSize + nextNextOffset >= size)
            {
                size_t newNextOffset = size + ALLOCATOR_BLOCK_HEADER_SIZE;
                memmove(prevBlock, addr, currentSize);
                
                size_t newNextNextOffset = (size_t)blockAfterNext - (size_t)prevBlock - newNextOffset;
                if (newNextNextOffset <= ALLOCATOR_BLOCK_HEADER_SIZE)
                {
                    newNextNextOffset = 0;
                    newNextOffset += newNextNextOffset;
                    setBlockPrevOffset(blockAfterNext, newNextOffset);
                }
                else
                {
                    writeBlockHeader((void*)((size_t)prevBlock + newNextOffset), true, newNextNextOffset, newNextOffset);
                    setBlockPrevOffset(blockAfterNext, newNextNextOffset);
                }

                writeBlockHeader(prevBlock, false, newNextOffset, prevPrevOffset);
                return prevBlock;
            }
        }
        
        // Moving in neighborhood didn't work, falling back to simple reallocation
        void* newAddr = mem_alloc(size);
        if (newAddr)
        {
            memmove(newAddr, addr, size);
            mem_free(addr);
            return newAddr;
        }
        else
            return nullptr; // Fail
    }
}

void Allocator::mem_free(void* addr)
{
    size_t memAddr = (size_t)addr;
    for (int i = 0; i < ALLOCATOR_MAX_PAGES; ++i)
    {
        if (pages[i])
        {
            if (memAddr >= (size_t)pages[i] && memAddr < (size_t)pages[i] + pageSize)
            {
                void* block = pages[i];
                while (block)
                {
                    if (addr == block)
                    {
                        setBlockState(block, true);
                        void* prevBlock = getPrevBlock(block);
                        void* nextBlock = getNextBlock(block);
                        bool nextFree = getBlockState(nextBlock);
                        if (prevBlock)
                        {
                            bool prevFree = getBlockState(prevBlock);
                            if (prevFree && nextFree)
                                mergeBlocks(prevBlock, nextBlock);
                            else if (prevFree)
                                mergeAdjacentBlocks(prevBlock, block);
                            else if (nextFree)
                                mergeAdjacentBlocks(block, nextBlock);
                        }
                        else if (nextFree)
                            mergeAdjacentBlocks(block, nextBlock);

                        return;
                    }
                    block = getNextBlock(block);
                    assert((size_t)block - (size_t)pages[i] < pageSize);
                }
            }
        }
    }
}

void Allocator::mem_dump(std::ostream& log)
{
    for (int i = 0; i < ALLOCATOR_MAX_PAGES; ++i)
    {
        if (pages[i])
        {
            log << "Page " << std::dec << i << " at " << std::hex << (size_t)(pages[i]) << ":\n";
            void* block = pages[i];
            size_t blockCounter = 0;
            while (block)
            {
                assert((size_t)block - (size_t)pages[i] < pageSize);
                bool free;
                size_t nextOffset;
                size_t prevOffset;
                size_t size;
                readBlockHeader(block, free, nextOffset, prevOffset);
                if (nextOffset > 0)
                    size = nextOffset - ALLOCATOR_BLOCK_HEADER_SIZE;
                else
                    size = 0;

                log << "    Block " << std::dec << blockCounter << " at " << std::hex << (size_t)block
                    << ": [ " << (free ? "Free" : "Reserved") << std::dec << ", Next=" << nextOffset << ", Prev=" << prevOffset << " ] "
                    << std::dec << size << " Bytes.\n";
                block = getNextBlock(block);
                ++blockCounter;
            }
            log << "\n";
        }
    }
}

void Allocator::initPages()
{
    for (int i = 0; i < ALLOCATOR_MAX_PAGES; ++i)
        pages[i] = nullptr;

    allocatePage();
}

void Allocator::destroyPages()
{
    for (int i = 0; i < ALLOCATOR_MAX_PAGES; ++i)
        deallocatePage(i);
}

size_t Allocator::allocatePage()
{
    size_t freePageIndex = ALLOCATOR_MAX_PAGES;
    for (size_t i = 0; i < ALLOCATOR_MAX_PAGES; ++i)
        if (pages[i] == nullptr)
        {
            freePageIndex = i;
            break;
        }

    if (freePageIndex < ALLOCATOR_MAX_PAGES)
    {
        pages[freePageIndex] = malloc(pageSize);

        // formatting new page with default first and stop blocks
        if (pages[freePageIndex])
        {
            // first block
            size_t offset = pageSize - ALLOCATOR_BLOCK_HEADER_SIZE; // stop block header is at the end of the page
            writeBlockHeader(pages[freePageIndex], true, offset, 0);

            // stop block
            void* stopBlock = (void*)((size_t)pages[freePageIndex] + offset);
            writeBlockHeader(stopBlock, false, 0, offset);

            /* // test
            void* newBlock = splitBlock(pages[freePageIndex], 512);
            mergeBlocks(pages[freePageIndex], newBlock);
            */
        }
        else
            freePageIndex = ALLOCATOR_MAX_PAGES;
    }
    return freePageIndex;
}

void Allocator::deallocatePage(size_t index)
{
    assert(index < ALLOCATOR_MAX_PAGES);

    if (pages[index] != nullptr)
        free(pages[index]);

    pages[index] = nullptr;
}

void* Allocator::mem_alloc_page(size_t index, size_t size)
{
    assert(size > 0);
    assert(size < pageSize - 2 * ALLOCATOR_BLOCK_HEADER_SIZE);
    if (pages[index])
    {
        void* block = pages[index];
        void* candidateBlock = block;
        size_t candidateBlockSize = 0;
        while (block)
        {
            assert((size_t)block - (size_t)pages[index] < pageSize);

            bool free;
            size_t nextOffset;
            size_t prevOffset;
            readBlockHeader(block, free, nextOffset, prevOffset);

            if (free)
            {
                size_t blockSize = nextOffset - ALLOCATOR_BLOCK_HEADER_SIZE;
                if (blockSize >= size &&
                    (candidateBlockSize > blockSize || candidateBlockSize == 0))
                {
                    candidateBlock = block;
                    candidateBlockSize = blockSize;
                }
            }

            block = getNextBlock(block);
        }

        if (candidateBlock && candidateBlockSize > 0)
        {
            void* prevBlock = getPrevBlock(candidateBlock);
            void* nextBlock = getNextBlock(candidateBlock);

            bool prevBlockFree = false;
            if (prevBlock)
                prevBlockFree = getBlockState(prevBlock);
            bool nextBlockFree = getBlockState(nextBlock);

            if (nextBlockFree || !prevBlockFree)
            {
                void* remainderBlock = splitBlock(candidateBlock, size);
                if (remainderBlock && nextBlockFree)
                    mergeAdjacentBlocks(remainderBlock, nextBlock);
                
                // reserve block
                setBlockState(candidateBlock, false);
                return candidateBlock;
            }
            else
            {
                void* splittedBlock = splitBlock(candidateBlock, candidateBlockSize-size);
                void* remainderBlock = candidateBlock;
                if (splittedBlock != remainderBlock)
                    mergeAdjacentBlocks(prevBlock, remainderBlock);
                
                // reserve block
                setBlockState(splittedBlock, false);
                return splittedBlock;
            }
        }
    }
    return nullptr;
}

void* Allocator::readBlockHeader(void* addr, bool& free, size_t& nextOffset, size_t& prevOffset)
{
    uint32_t hWordLow = *(uint32_t*)addr;
    uint32_t hWordHigh = *(uint32_t*)((size_t)addr + sizeof(uint32_t));

    free = (bool)(hWordLow & ALLOCATOR_BLOCK_HEADER_FREE_FLAG_MASK);
    nextOffset = hWordLow & ALLOCATOR_BLOCK_HEADER_OFFSET_MASK;
    prevOffset = hWordHigh & ALLOCATOR_BLOCK_HEADER_OFFSET_MASK;

    return (void*)((size_t)addr + ALLOCATOR_BLOCK_HEADER_SIZE);
}

void* Allocator::getNextBlock(void* addr)
{
    uint32_t hWordLow = *(uint32_t*)addr;
    uint32_t nextOffset = hWordLow & ALLOCATOR_BLOCK_HEADER_OFFSET_MASK;
    if (nextOffset > 0)
        return (void*)((size_t)addr + nextOffset);
    else
        return nullptr;
}

void* Allocator::getPrevBlock(void* addr)
{
    uint32_t hWordHigh = ((uint32_t*)addr)[1];
    uint32_t prevOffset = hWordHigh & ALLOCATOR_BLOCK_HEADER_OFFSET_MASK;
    if (prevOffset > 0)
        return (void*)((size_t)addr - prevOffset);
    else
        return nullptr;
}

void Allocator::writeBlockHeader(void* addr, bool free, size_t nextOffset, size_t prevOffset)
{
    uint32_t* mem = (uint32_t*)addr;
    assert(nextOffset % ALLOCATOR_ALIGNMENT == 0);
    assert(prevOffset % ALLOCATOR_ALIGNMENT == 0);
    mem[0] = (uint32_t)nextOffset & ALLOCATOR_BLOCK_HEADER_OFFSET_MASK;
    mem[1] = (uint32_t)prevOffset & ALLOCATOR_BLOCK_HEADER_OFFSET_MASK;
    setBlockState(addr, free);
}

void Allocator::setBlockState(void* addr, bool free)
{
    uint32_t hWordLow = *(uint32_t*)addr;
    if (free)
        hWordLow |= ALLOCATOR_BLOCK_HEADER_FREE_FLAG_MASK;
    else
        hWordLow &= (~ALLOCATOR_BLOCK_HEADER_FREE_FLAG_MASK);
    *(uint32_t*)addr = hWordLow;
}

bool Allocator::getBlockState(void* addr)
{
    uint32_t hWordLow = (*(uint32_t*)addr) & ALLOCATOR_BLOCK_HEADER_FREE_FLAG_MASK;
    return hWordLow > 0;
}

void Allocator::setBlockPrevOffset(void* addr, size_t prevOffset)
{
    uint32_t* mem = (uint32_t*)addr;
    assert(prevOffset % ALLOCATOR_ALIGNMENT == 0);
    mem[1] = (uint32_t)prevOffset & ALLOCATOR_BLOCK_HEADER_OFFSET_MASK;
}

void Allocator::setBlockNextOffset(void* addr, size_t nextOffset)
{
    uint32_t* mem = (uint32_t*)addr;
    assert(nextOffset % ALLOCATOR_ALIGNMENT == 0);
    mem[0] = (uint32_t)nextOffset & ALLOCATOR_BLOCK_HEADER_OFFSET_MASK;
}

void* Allocator::splitBlock(void* addr, size_t newSize)
{
    assert(newSize % ALLOCATOR_ALIGNMENT == 0);
    bool free;
    size_t nextOffset;
    size_t prevOffset;
    readBlockHeader(addr, free, nextOffset, prevOffset);
    massert(free, "Cannot split reserved block!");

    size_t currentSize = nextOffset - ALLOCATOR_BLOCK_HEADER_SIZE;
    if (currentSize == newSize) // splitting the same size is a null operation, but it is not an error
        return nullptr;
    if (newSize == 0) // splitting zero is equivalent of swapping head and tail
        return addr;
    massert(currentSize > newSize, "Splitted size cannot be bigger than current size!"); // that's an error

    size_t newOffset = newSize + ALLOCATOR_BLOCK_HEADER_SIZE;
    void* newBlock = (void*)((size_t)addr + newOffset);

    writeBlockHeader(newBlock, true, nextOffset - newOffset, newOffset);
    writeBlockHeader(addr, true, newOffset, prevOffset);

    return newBlock;
}

void Allocator::mergeBlocks(void* firstBlock, void* lastBlock)
{
    void* block = firstBlock;
    size_t newNextOffset = 0;
    size_t newPrevOffset = (size_t)-1;
    while (block)
    {
        bool free;
        size_t nextOffset;
        size_t prevOffset;
        readBlockHeader(block, free, nextOffset, prevOffset);
        if (!free)
            return; // we won't merge reserved blocks, but it's not an error
        //assert(free);
        assert(nextOffset > 0);
        assert((size_t)block - (size_t)firstBlock < pageSize);
        newNextOffset += nextOffset;
        if (newPrevOffset > pageSize)
            newPrevOffset = prevOffset;

        if (block == lastBlock)
            break;

        block = getNextBlock(block);
    }

    void* newNextBlock = getNextBlock(block);
    writeBlockHeader(firstBlock, true, newNextOffset, newPrevOffset);
    setBlockPrevOffset(newNextBlock, newNextOffset);
}

void Allocator::mergeAdjacentBlocks(void* firstBlock, void* secondBlock)
{
    bool firstFree;
    size_t firstNextOffset;
    size_t firstPrevOffset;
    readBlockHeader(firstBlock, firstFree, firstNextOffset, firstPrevOffset);

    bool secondFree;
    size_t secondNextOffset;
    size_t secondPrevOffset;
    readBlockHeader(secondBlock, secondFree, secondNextOffset, secondPrevOffset);

    assert(firstFree && secondFree);
    assert((size_t)firstBlock + firstNextOffset == (size_t)secondBlock);

    void* nextBlock = (void*)((size_t)secondBlock + secondNextOffset);
    size_t newNextOffset = firstNextOffset + secondNextOffset;
    writeBlockHeader(firstBlock, true, newNextOffset, firstPrevOffset);
    setBlockPrevOffset(nextBlock, newNextOffset);
}
