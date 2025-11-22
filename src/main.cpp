#include "../include/singly_linked_list.h"
#include "../include/heap_memory_resource.h"
#include <iostream>
#include <string>


void print_pool_stats(const heap_memory_resource& resource, const char* stage) {
    std::cout << "\n[POOL STATE] " << stage << ":\n";
    std::cout << "  Active blocks: " << resource.get_allocated_count() 
              << " | Free blocks: " << resource.get_free_count()
              << " | Total allocated: " << resource.get_total_allocated() << " bytes\n";
}

int main() {
    heap_memory_resource memory_resource;

    {
        singly_linked_list<int> list1(&memory_resource);
        std::cout << "\n[STEP 1] Creating first list (3 elements)...\n";
        list1.push_back(10);
        list1.push_back(20);
        list1.push_back(30);
        
        print_pool_stats(memory_resource, "After first list allocation");
        std::cout << "List1 content: ";
        list1.print();
    }

    print_pool_stats(memory_resource, "After first list destruction");
    std::cout << "[KEY POINT] Memory NOT freed to OS! Blocks moved to free pool.\n";
    
    {
        singly_linked_list<int> list2(&memory_resource);
        std::cout << "\n[STEP 2] Creating second list (3 elements)...\n";
        list2.push_back(100);
        list2.push_back(200);
        list2.push_back(300);
        
        print_pool_stats(memory_resource, "After second list allocation");
        std::cout << "List2 content: ";
        list2.print();
    }
    
    print_pool_stats(memory_resource, "Final state before resource destruction");
    std::cout << "\n[CONCLUSION] No new memory allocated for second list!\n";
    std::cout << "Total allocated from OS: " << memory_resource.get_total_allocated() 
              << " bytes (same as after first allocation)\n";
    
    std::cout << "\n Program finished. All blocks will be freed to OS in destructor.\n";
    return 0;
}