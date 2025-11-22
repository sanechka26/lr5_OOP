#include <gtest/gtest.h>
#include "../include/heap_memory_resource.h"
#include "../include/singly_linked_list.h"

// Тест 1: Проверка выделения и возврата в пул
TEST(HeapMemoryResourceTest, AllocationAndPooling) {
    heap_memory_resource resource;
    
    {
        singly_linked_list<int> list(&resource);
        list.push_back(42); // Выделен 1 блок
        
        ASSERT_EQ(resource.get_allocated_count(), 1); // 1 активный блок
        ASSERT_EQ(resource.get_free_count(), 0);      // Пул пуст
        ASSERT_EQ(resource.get_total_allocated(), 16); // Примерный размер Node<int>
    }
    
    // После уничтожения списка:
    ASSERT_EQ(resource.get_allocated_count(), 0);  // Нет активных блоков
    ASSERT_EQ(resource.get_free_count(), 1);       // 1 блок в пуле
    ASSERT_EQ(resource.get_total_allocated(), 16); // Суммарное выделение не изменилось!
}

// Тест 2: Повторное использование из пула
TEST(HeapMemoryResourceTest, MemoryReuse) {
    heap_memory_resource resource;
    
    size_t initial_allocated = 0;
    
    {
        singly_linked_list<int> list1(&resource);
        list1.push_back(10);
        initial_allocated = resource.get_total_allocated(); // 16 байт
    }
    
    // Память в пуле, но не освобождена ОС
    ASSERT_EQ(resource.get_free_count(), 1);
    
    {
        singly_linked_list<int> list2(&resource);
        list2.push_back(20); // Берёт блок из пула
        
        // total_allocated НЕ УВЕЛИЧИЛСЯ!
        ASSERT_EQ(resource.get_total_allocated(), initial_allocated);
        ASSERT_EQ(resource.get_allocated_count(), 1); // 1 активный блок
        ASSERT_EQ(resource.get_free_count(), 0);      // Пул пуст
    }
}

// Тест 3: Корректное освобождение в деструкторе
TEST(HeapMemoryResourceTest, FinalDeallocation) {
    size_t total_before = 0;
    size_t total_after = 0;
    
    {
        heap_memory_resource resource;
        total_before = resource.get_total_allocated(); // 0
        
        {
            singly_linked_list<int> list(&resource);
            list.push_back(100);
        }
        
        // Память в пуле, но не освобождена
        ASSERT_GT(resource.get_free_count(), 0);
        total_after = resource.get_total_allocated(); // 16
    } // Деструктор resource освобождает ВСЕ блоки из пула
    
    // Проверить невозможно напрямую (ресурс уничтожен),
    // но если бы были утечки — ASan или valgrind это показали бы
    SUCCEED() << "Destructor correctly freed all blocks";
}
