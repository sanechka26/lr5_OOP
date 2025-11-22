#pragma once
#include <memory_resource>
#include <map>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <iostream>

class heap_memory_resource : public std::pmr::memory_resource {
private:
    std::map<void*, std::size_t> allocated_blocks;
    
    std::multimap<std::size_t, void*> free_blocks;

    std::size_t total_allocated = 0;

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        auto range = free_blocks.equal_range(bytes);
        for (auto it = range.first; it != range.second; ++it) {
            void* ptr = it->second;
            if (reinterpret_cast<std::uintptr_t>(ptr) % alignment == 0) {
                free_blocks.erase(it);
                allocated_blocks[ptr] = bytes;
                return ptr;
            }
        }

        void* ptr = ::operator new(bytes, std::align_val_t(alignment));
        allocated_blocks[ptr] = bytes;
        total_allocated += bytes;
        return ptr;
    }

    void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override {
        if (!ptr) return;

        auto it = allocated_blocks.find(ptr);
        if (it == allocated_blocks.end()) {
            std::cerr << "[ERROR] Freeing unknown block\n";
            return;
        }

        allocated_blocks.erase(it);
        free_blocks.emplace(bytes, ptr);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

public:
    ~heap_memory_resource() override {
        for (const auto& [size, ptr] : free_blocks) {
            ::operator delete(ptr, std::align_val_t(alignof(std::max_align_t)));
        }
        free_blocks.clear();

        if (!allocated_blocks.empty()) {
            std::cerr << "[ERROR] Memory leaks detected!\n";
            for (const auto& [ptr, size] : allocated_blocks) {
                std::cerr << "  Leaked block at " << ptr << ", size: " << size << " bytes\n";
            }
        }

        std::cout << "[POOL STATS] Total allocated: " << total_allocated << " bytes\n";
        std::cout << "[POOL STATS] Free blocks remaining: " << free_blocks.size() << "\n";
    }

    std::size_t get_allocated_count() const { return allocated_blocks.size(); }
    std::size_t get_free_count() const { return free_blocks.size(); }

    std::size_t get_total_allocated() const {
        return total_allocated;
    }
};