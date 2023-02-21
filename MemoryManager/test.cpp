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