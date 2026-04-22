// arena.hpp
#pragma once
#include <vector>
#include <stdexcept>
#include <utility> // for std::forward

class ArenaAllocator {
private:
    std::vector<char*> chunks; 
    size_t chunkSize;          
    size_t currentOffset;      

    void allocateNewChunk(); 

public:
    ArenaAllocator(size_t chunk_size = 65536);
    ~ArenaAllocator();

    void* allocate(size_t bytes); // We will write the logic for this in the .cpp!

    // Template stays in the header!
    template<typename T, typename... Args>
    T* create(Args&&... args) {
        void* memory = allocate(sizeof(T));
        return new (memory) T(std::forward<Args>(args)...);
    }
};