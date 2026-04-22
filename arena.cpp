// arena.cpp
#include "arena.hpp"

// The Helper
void ArenaAllocator::allocateNewChunk() {
    chunks.push_back(new char[chunkSize]);
    currentOffset = 0;
}

// The Constructor
ArenaAllocator::ArenaAllocator(size_t chunk_size) {
    chunkSize = chunk_size;
    allocateNewChunk();
}

// The Destructor
ArenaAllocator::~ArenaAllocator() {
    for (char* chunk : chunks) {
        delete[] chunk;
    }
}

// --- YOUR MISSION ---
void* ArenaAllocator::allocate(size_t bytes) {
    // Write your Paged Allocator logic here!
    if(currentOffset + bytes > chunkSize){
        allocateNewChunk();
    }
    char* bp = chunks.back();
    char* mem_adr = bp + currentOffset;
    currentOffset += bytes;
    return mem_adr;
}