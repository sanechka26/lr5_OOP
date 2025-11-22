#include <gtest/gtest.h>
#include "../include/singly_linked_list.h"
#include "../include/heap_memory_resource.h"

TEST(SinglyLinkedListTest, PushBackAndGetFront) {
    heap_memory_resource resource;
    singly_linked_list<int> list(&resource);
    
    list.push_back(10);
    list.push_back(20);
    
    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.front(), 10);
    ASSERT_EQ(resource.get_allocated_count(), 2); // 2 активных блока
    ASSERT_EQ(resource.get_free_count(), 0);
}

TEST(SinglyLinkedListTest, ClearReturnsToPool) {
    heap_memory_resource resource;
    singly_linked_list<int> list(&resource);
    
    list.push_back(1);
    list.push_back(2);
    
    size_t before_clear = resource.get_total_allocated();
    list.clear(); // Блоки возвращены в пул
    
    ASSERT_EQ(resource.get_allocated_count(), 0); // Нет активных блоков
    ASSERT_EQ(resource.get_free_count(), 2);      // 2 блока в пуле
    ASSERT_EQ(resource.get_total_allocated(), before_clear); // Не изменилось!
}


TEST(SinglyLinkedListTest, Iteration) {
    heap_memory_resource resource;
    singly_linked_list<int> list(&resource);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    int expected[] = {1, 2, 3};
    int index = 0;
    for (int value : list) {
        ASSERT_EQ(value, expected[index]);
        index++;
    }
    ASSERT_EQ(index, 3);
}

TEST(SinglyLinkedListTest, Clear) {
    heap_memory_resource resource;
    singly_linked_list<int> list(&resource);
    
    list.push_back(1);
    list.push_back(2);
    list.clear();
    
    ASSERT_TRUE(list.empty());
    ASSERT_EQ(list.size(), 0);
}

TEST(SinglyLinkedListTest, Size) {
    heap_memory_resource resource;
    singly_linked_list<int> list(&resource);
    
    ASSERT_EQ(list.size(), 0);
    list.push_back(10);
    ASSERT_EQ(list.size(), 1);
    list.push_back(20);
    ASSERT_EQ(list.size(), 2);
}

TEST(SinglyLinkedListTest, ComplexType) {
    heap_memory_resource resource;
    singly_linked_list<std::string> list(&resource);
    
    list.push_back("Hello");
    list.push_back("World");
    
    auto it = list.begin();
    ASSERT_EQ(*it, "Hello");
    ++it;
    ASSERT_EQ(*it, "World");
}