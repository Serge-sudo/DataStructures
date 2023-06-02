#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "mm.h"
#include <vector>
#include <string>

struct item {
    int num;
    std::string str;
    time_t time;
};

TEST_CASE("newObject") {
    lab618::CMemoryManager<item> mm(10, true);
    std::vector<item *> objs;
    for (int i = 0; i < 2000; ++i) {
        item *obj = mm.newObject();
        obj->num = i;
        obj->time = i * 1000;
        obj->str = std::to_string(i);
        objs.push_back(obj);
    }
    for (int i = 0; i < 2000; ++i) {
        CHECK(objs[i]->num == i);
        CHECK(objs[i]->time == i * 1000);
        CHECK(objs[i]->str == std::to_string(i));
    }
}

TEST_CASE("deleteObject") {
    SUBCASE("OnDestructor") {
        lab618::CMemoryManager<item> mm(10, true);
        std::vector<item *> objs;
        for (int i = 0; i < 2000; ++i) {
            item *obj = mm.newObject();
            obj->num = i;
            obj->time = i * 1000;
            obj->str = std::to_string(i);
            objs.push_back(obj);
        }
        for (int i = 0; i < 2000; ++i) {
            CHECK(objs[i]->num == i);
            CHECK(objs[i]->time == i * 1000);
            CHECK(objs[i]->str == std::to_string(i));
            mm.deleteObject(objs[i]);
        }
    }
    SUBCASE("NotOnDestructor") {
        lab618::CMemoryManager<item> mm(10, false);
        std::vector<item *> objs;
        for (int i = 0; i < 2000; ++i) {
            item *obj = mm.newObject();
            obj->num = i;
            obj->time = i * 1000;
            obj->str = std::to_string(i);
            objs.push_back(obj);
        }
        for (int i = 0; i < 2000; ++i) {
            CHECK(objs[i]->num == i);
            CHECK(objs[i]->time == i * 1000);
            CHECK(objs[i]->str == std::to_string(i));
            mm.deleteObject(objs[i]);
        }
    }

}

TEST_CASE("Clear") {
    SUBCASE("OnDestructor") {
        lab618::CMemoryManager<item> mm(10, true);
        std::vector<item *> objs;
        for (int i = 0; i < 2000; ++i) {
            item *obj = mm.newObject();
            obj->num = i;
            obj->time = i * 1000;
            obj->str = std::to_string(i);
            objs.push_back(obj);
        }
        for (int i = 0; i < 10; ++i) {
            CHECK(objs[i]->num == i);
            CHECK(objs[i]->time == i * 1000);
            CHECK(objs[i]->str == std::to_string(i));
            mm.deleteObject(objs[i]);
        }
        CHECK_NOTHROW(mm.clear());
    }

    SUBCASE("NotOnDestructor") {
        lab618::CMemoryManager<item> mm(10, false);
        std::vector<item *> objs;
        for (int i = 0; i < 2000; ++i) {
            item *obj = mm.newObject();
            obj->num = i;
            obj->time = i * 1000;
            obj->str = std::to_string(i);
            objs.push_back(obj);
        }
        for (int i = 0; i < 10; ++i) {
            CHECK(objs[i]->num == i);
            CHECK(objs[i]->time == i * 1000);
            CHECK(objs[i]->str == std::to_string(i));
            mm.deleteObject(objs[i]);
        }
        CHECK_THROWS_AS(mm.clear(), lab618::CMemoryManager<item>::CException);
    }
}

TEST_CASE("SIMPLE_TEST") {
    SUBCASE("TEST1") {
        lab618::CMemoryManager<item> mm(10, false);
        std::vector<item *> objs;
        for (int i = 0; i < 2000; ++i) {
            item *obj = mm.newObject();
            obj->num = i;
            obj->time = i * 1000;
            obj->str = std::to_string(i);
            objs.push_back(obj);
        }
        for (int i = 0; i < 1000; ++i) {
            mm.deleteObject(objs[i]);
        }
        for (int i = 2000; i < 3000; ++i) {
            item *obj = mm.newObject();
            obj->num = i;
            obj->time = i * 1000;
            obj->str = std::to_string(i);
            objs.push_back(obj);
        }
        for (int i = 1000; i < 3000; ++i) {
            CHECK(objs[i]->num == i);
            CHECK(objs[i]->time == i * 1000);
            CHECK(objs[i]->str == std::to_string(i));
            mm.deleteObject(objs[i]);
        }
    }

    SUBCASE("TEST2") {
        lab618::CMemoryManager<item> mm(10, true);
        std::vector<item *> objs;
        for (int i = 0; i < 2000; ++i) {
            item *obj = mm.newObject();
            obj->num = i;
            obj->time = i * 1000;
            obj->str = std::to_string(i);
            objs.push_back(obj);
        }
        for (int i = 2000; i < 3000; ++i) {
            item *obj = mm.newObject();
            obj->num = i;
            obj->time = i * 1000;
            obj->str = std::to_string(i);
            objs.push_back(obj);
        }
        for (int i = 1000; i < 3000; ++i) {
            CHECK(objs[i]->num == i);
            CHECK(objs[i]->time == i * 1000);
            CHECK(objs[i]->str == std::to_string(i));
        }
    }
}

TEST_CASE("EFFECTIVENESS") {
    SUBCASE("Allocation and deallocation - auto remove disabled") {
        std::cout << "-----------------Without auto remove----------------" << std::endl;
        std::size_t size = 1000000;

        lab618::CMemoryManager<item> mm(100000, false);

        std::vector<item *> objs;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; ++i) {
            objs.push_back(new item);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Default memory manager allocation: " << duration.count() << " microseconds\n";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; ++i) {
            item *ptr = objs.back();
            objs.pop_back();
            delete ptr;
        }
        end = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Default memory manager deallocation: " << duration.count() << " microseconds\n";

        objs.shrink_to_fit();
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; ++i) {
            objs.push_back(mm.newObject());
        }
        end = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Custom memory manager allocation: " << duration.count() << " microseconds\n";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; ++i) {
            item *ptr = objs.back();
            objs.pop_back();
            mm.deleteObject(ptr);
        }
        end = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Custom memory manager deallocation: " << duration.count() << " microseconds\n";

    }
    SUBCASE("Allocation and deallocation auto remove enabled") {
        std::cout << "-----------------With auto remove----------------" << std::endl;
        std::size_t size = 1000000;

        lab618::CMemoryManager<item> mm(100000, true);

        std::vector<item *> objs;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; ++i) {
            objs.push_back(new item);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Default memory manager allocation: " << duration.count() << " microseconds\n";

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; ++i) {
            item *ptr = objs.back();
            objs.pop_back();
            delete ptr;
        }
        end = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Default memory manager deallocation: " << duration.count() << " microseconds\n";

        objs.shrink_to_fit();

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; ++i) {
            objs.push_back(mm.newObject());
        }
        end = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Custom memory manager allocation: " << duration.count() << " microseconds\n";

        start = std::chrono::high_resolution_clock::now();
        objs.clear();
        mm.~CMemoryManager();
        end = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Custom memory manager deallocation: " << duration.count() << " microseconds\n";

    }
}
