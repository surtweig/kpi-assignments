#include "BuddyAllocator.h"
#include "../massert.h"
#include <cassert>

BuddyAllocator::BuddyAllocator(size_t levels, size_t minBlock)
{
    assert(levels > 0);
    assert(minBlock > 0);
    this->maxLevel = levels-1;
    this->minBlock = minBlock;
    totalSize = (1 << (levels - 1)) * minBlock;
    
    size_t tocSize = 0;
    for (size_t i = 0; i < levels; ++i)
        tocSize += 1 << i;

    toc.resize(tocSize, false);

    buffer = malloc(totalSize);
}

BuddyAllocator::~BuddyAllocator()
{
    free(buffer);
}

void* BuddyAllocator::mem_alloc(size_t size)
{
    size_t ceilLevel = getCeilLevel(size);
    for (BlockMetadata block = getFirstBlockLevel(ceilLevel); block.data != nullptr; block = getNextBlockLevel(block))
    {
        if (isFree(block))
        {
            if (acquire(block))
            {
                return block.data;
            }
        }
    }
    return nullptr;
}

void* BuddyAllocator::mem_realloc(void* addr, size_t size)
{
    return nullptr;
}

void BuddyAllocator::mem_free(void* addr)
{
    BlockMetadata block = getBlockAtAddr(addr);
    if (block.data)
    {
        if (!isFree(block))
        {
            release(block);
        }
    }
}

void BuddyAllocator::mem_dump(ostream& log)
{
    for (size_t level = 0; level <= maxLevel; ++level)
    {
        log << "Level " << dec << level << " blocks " << getLevelBlockSize(level) << " bytes : ";
        BlockMetadata block = getFirstBlockLevel(level);
        while (block.data)
        {
            log << "[" << dec << block.tocIndex << (isFree(block) ? " (-) " : " (+) ")
                << "at 0x" << hex << (size_t)block.data - (size_t)buffer << "] ";
                
            block = getNextBlockLevel(block);
        }
        log << endl;
    }
}

BuddyAllocator::BlockMetadata BuddyAllocator::getBlockAtAddr(void* addr)
{
    size_t pos = ((size_t)addr - (size_t)buffer);

    for (size_t level = 0; level <= maxLevel; ++level)
    {
        size_t blockSize = getLevelBlockSize(level);
        if (pos % blockSize == 0)
        {
            BlockMetadata block = BlockMetadata(
                (1 << level) - 1 + pos / blockSize,
                level,
                addr
            );
            return block;
        }
    }

    return BlockMetadata();
}

size_t BuddyAllocator::getLevelBlockSize(size_t level)
{
    return minBlock * (1 << (maxLevel - level));
}

size_t BuddyAllocator::getCeilLevel(size_t reqSize)
{
    assert(reqSize > 0);
    size_t minBlockCount = (reqSize - 1) / minBlock + 1;
    if (minBlockCount == 1)
        return maxLevel;

    size_t levelMinBlockCount = 1;
    for (size_t i = maxLevel; i > 0; --i)
    {
        levelMinBlockCount *= 2;
        if (levelMinBlockCount >= minBlockCount)
        {
            return i - 1;
        }
    }
    return InvalidIndex;
}

BuddyAllocator::BlockMetadata BuddyAllocator::getRootBlock()
{
    return BlockMetadata(0, 0, buffer);
}

BuddyAllocator::BlockMetadata BuddyAllocator::getRightBuddy(const BlockMetadata& block)
{
    assert(block.data);

    if (block.tocIndex % 2 == 1)
    {
        return BlockMetadata(
            block.tocIndex + 1,
            block.level,
            (void*)((size_t)block.data + getLevelBlockSize(block.level)));
    }
    else
        return BlockMetadata();
    /*
        |-0----------------------------|
        |-1------------||-2------------|
        |-3----||-4----||-5----||-6----|
        |-7||-8||-9||10||11||12||13||14|

        |0-------------------------------------------------------------|
        |1-----------------------------||2-----------------------------|
        |3-------------||4-------------||5-------------||6-------------|
        |7-----||8-----||9-----||10----||11----||12----||13----||14----|
        |15||16||17||18||19||20||21||22||23||24||25||26||27||28||29||30|
    */
}

BuddyAllocator::BlockMetadata BuddyAllocator::getLeftBuddy(const BlockMetadata& block)
{
    assert(block.data);

    if (block.tocIndex % 2 == 0 && (block.level > 0))
    {
        return BlockMetadata(
            block.tocIndex - 1,
            block.level,
            (void*)((size_t)block.data - getLevelBlockSize(block.level))
        );
    }
    else
        return BlockMetadata();
}

BuddyAllocator::BlockMetadata BuddyAllocator::getLeftChild(const BlockMetadata& block)
{
    assert(block.data);

    if (block.level < maxLevel)
    {
        return BlockMetadata(
            block.tocIndex * 2 + 1,
            block.level + 1,
            block.data
        );
    }
    else
        return BlockMetadata();
}

BuddyAllocator::BlockMetadata BuddyAllocator::getParent(const BlockMetadata& block)
{
    assert(block.data);

    if (block.level > 0)
    {
        return BlockMetadata(
            (block.tocIndex - 1) / 2,
            block.level - 1,
            block.data
        );
    }
    else
        return BlockMetadata();
}

BuddyAllocator::BlockMetadata BuddyAllocator::getFirstBlockLevel(size_t level)
{
    if (level <= maxLevel)
    {
        return BlockMetadata(
            (1 << level) - 1,
            level,
            buffer
        );
    }
    return BlockMetadata();
}

BuddyAllocator::BlockMetadata BuddyAllocator::getNextBlockLevel(const BlockMetadata& block)
{
    size_t nextLevelFirstBlockTOCIndex = (1 << (block.level + 1)) - 1;
    if (block.tocIndex + 1 < nextLevelFirstBlockTOCIndex) // if it's not the last block on this level
    {
        return BlockMetadata(
            block.tocIndex + 1,
            block.level,
            (void*)((size_t)block.data + getLevelBlockSize(block.level))
        );
    }
    return BlockMetadata();
}

void BuddyAllocator::ascend(const BlockMetadata& block, bool val)
{
    assert(block.data);

    if (!val)
    {
        BlockMetadata leftChild = getLeftChild(block);
        BlockMetadata rightChild = getRightBuddy(leftChild);

        if (isFree(leftChild) && isFree(rightChild))
        {
            toc[block.tocIndex] = val;
            if (block.level > 0)
                ascend(getParent(block), val);
        }
    }
    else
    {
        toc[block.tocIndex] = val;
        if (block.level > 0)
            ascend(getParent(block), val);
    }

}

void BuddyAllocator::descend(const BlockMetadata& block, bool val)
{
    assert(block.data);

    toc[block.tocIndex] = val;
    if (block.level < maxLevel)
    {
        BlockMetadata leftChild = getLeftChild(block);
        BlockMetadata rightChild = getRightBuddy(leftChild);
        descend(leftChild, val);
        descend(rightChild, val);
    }
}


bool BuddyAllocator::acquire(const BlockMetadata& block)
{
    assert(block.data);

    // cannot aquire occupied block
    if (!isFree(block))
        return false;

    /*
    // acquire parent
    if (block.level > 0)
        acquire(getParent(block));

    BlockMetadata leftChild = getLeftChild(block);
    BlockMetadata rightChild = getRightBuddy(block);
    toc[block.tocIndex] = true;
    */
    ascend(block, true);
    descend(block, true);
    return true;
}

bool BuddyAllocator::release(const BlockMetadata& block)
{
    assert(block.data);

    if (isFree(block))
        return false;

    
    descend(block, false);
    return true;
    /*
    BlockMetadata leftChild = getLeftChild(block);
    BlockMetadata rightChild = getRightBuddy(leftChild);
    if (isFree(leftChild) && isFree(rightChild))
    {
        toc[block.tocIndex] = false;
        
        bool rlsParent = true;
        if (block.level > 0)
            rlsParent = release(getParent(block));
        
        if (!rlsParent)
            toc[block.tocIndex] = true; // fail, reverting original state
        
        return rlsParent;
    }
    return false;
    */
}


bool BuddyAllocator::split(const BlockMetadata& block)
{
    assert(block.data);

    if (block.level < maxLevel)
    {
        if (!toc[block.tocIndex])
        {
            BlockMetadata leftchild = getLeftChild(block);
            BlockMetadata rightchild = getRightBuddy(leftchild);
            toc[block.tocIndex] = true;
            toc[leftchild.tocIndex] = false;
            toc[rightchild.tocIndex] = false;
            return true;
        }
    }
    return false;
}

bool BuddyAllocator::merge(const BlockMetadata& block)
{
    assert(block.data);

    if (block.level < maxLevel)
    {
        if (toc[block.tocIndex])
        {
            BlockMetadata leftchild = getLeftChild(block);
            BlockMetadata rightchild = getRightBuddy(leftchild);
            toc[block.tocIndex] = false;
            merge(leftchild);
            merge(rightchild);
            return true;
        }
    }
    return false;
}
