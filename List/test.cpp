//
// Created by Mr. John on 2/4/2023.
//
#include "list.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include <string>

using namespace lab618;

template<class T>
std::string printList(CSingleLinkedList<T>& list) {
    std::string trace = "";
    for (auto it = list.begin(); it.isValid(); ++it) {
        if (it != list.begin())
            trace += "->";
        trace += std::to_string(it.getData().val);
    }
    return trace;
}

template<class T>
std::string printList(CDualLinkedList<T>& list) {
    std::string trace = "";
    for (auto it = list.begin(); it.isValid(); ++it) {
        if (it != list.begin())
            trace += "<->";
        trace += std::to_string(it.getData().val);
    }
    return trace;
}

template<class T>
struct Package {
    T val;
    Package& operator=(T& v) {
        val = v;
        return *this;
    }
};

TEST_CASE("ForwardList") {
    const int SIZE = 100;
    Package<int> p[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        p[i] = i;
    }
    SUBCASE("PushBack") {
        CSingleLinkedList<Package<int>> list;
        std::string res = "";
        for (Package<int>& pack : p) {
            list.pushBack(pack);
            res += std::to_string(pack.val);
            CHECK(res == printList(list));
            res += "->";
        }
    }
    SUBCASE("PushFront") {
        CSingleLinkedList<Package<int>> list;
        std::string res = "";
        for (Package<int>& pack : p) {
            list.pushFront(pack);
            res = std::to_string(pack.val) + res;
            CHECK(res == printList(list));
            res = "->" + res;
        }
    }
    SUBCASE("PopFront") {
        CSingleLinkedList<Package<int>> list;
        std::string res = "";
        for (Package<int>& pack : p) {
            list.pushBack(pack);
        }
        for (Package<int>& pack : p) {
            CHECK(list.popFront().val == pack.val);
        }
    }

    SUBCASE("Mix-back-front-push-pop") {
        CSingleLinkedList<Package<int>> list;
        std::string res = "";
        for (size_t i = 0; i < SIZE; ++i) {
            if ((i & 1) == 0)
                list.pushBack(p[i]);
            else
                list.pushFront(p[i]);
        }
        for (int i = SIZE - 1 - (((SIZE & 1) == 0) ? 0 : 1); i >= 0; i -= 2) {
            CHECK(list.popFront().val == p[i].val);
        }
        for (int i = 0; i < SIZE; i += 2) {
            CHECK(list.popFront().val == p[i].val);
        }
    }

    SUBCASE("Size") {
        CSingleLinkedList<Package<int>> list;
        size_t cnt = 0;
        for (Package<int>& pack : p) {
            list.pushBack(pack);
            cnt++;
            CHECK(list.getSize() == cnt);
        }
    }
    SUBCASE("Clear") {
        CSingleLinkedList<Package<int>> list;
        for (Package<int>& pack : p) {
            list.pushBack(pack);
        }
        list.clear();
        CHECK(list.getSize() == 0);
    }
    SUBCASE("Erase") {
        CSingleLinkedList<Package<int>> list;
        for (Package<int>& pack : p) {
            list.pushBack(pack);
        }
        for (auto it = list.begin(); it.isValid(); ++it, ++it) {
            list.erase(it);
        }
        for (int i = 1; i < SIZE; i += 2) {
            CHECK(list.popFront().val == p[i].val);
        }
    }
    SUBCASE("Iterator") {
        CSingleLinkedList<Package<int>> list;
        for (Package<int>& pack : p) {
            list.pushBack(pack);
        }
        int i = 0;
        for (auto it = list.begin(); it.isValid(); ++it, ++i) {
            CHECK(it.getData().val == p[i].val);
        }
    }
}

TEST_CASE("BiDirectionalList") {
    const int SIZE = 100;
    Package<int> p[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        p[i] = i;
    }
    SUBCASE("PushBack") {
        CDualLinkedList<Package<int>> list;
        std::string res = "";
        for (Package<int>& pack : p) {
            list.pushBack(pack);
            res += std::to_string(pack.val);
            CHECK(res == printList(list));
            res += "<->";
        }
    }
    SUBCASE("PushFront") {
        CDualLinkedList<Package<int>> list;
        std::string res = "";
        for (Package<int>& pack : p) {
            list.pushFront(pack);
            res = std::to_string(pack.val) + res;
            CHECK(res == printList(list));
            res = "<->" + res;
        }
    }
    SUBCASE("PopFront") {
        CDualLinkedList<Package<int>> list;
        for (Package<int>& pack : p) {
            list.pushBack(pack);
        }
        for (Package<int>& pack : p) {
            CHECK(list.popFront().val == pack.val);
        }
    }

    SUBCASE("PopBack") {
        CDualLinkedList<Package<int>> list;
        std::string res = "";
        for (Package<int>& pack : p) {
            list.pushFront(pack);
        }
        for (Package<int>& pack : p) {
            CHECK(list.popBack().val == pack.val);
        }
    }

    SUBCASE("Mix-back-front-push-pop") {
        CDualLinkedList<Package<int>> list;
        std::string res = "";
        for (size_t i = 0; i < SIZE; ++i) {
            if ((i & 1) == 0)
                list.pushBack(p[i]);
            else
                list.pushFront(p[i]);
        }

        for (int i = SIZE - 1; i >= 0; --i) {
            if ((i & 1) == 0)
                CHECK(list.popBack().val == p[i].val);
            else
                CHECK(list.popFront().val == p[i].val);
        }
    }

    SUBCASE("Size") {
        CDualLinkedList<Package<int>> list;
        size_t cnt = 0;
        for (Package<int>& pack : p) {
            list.pushBack(pack);
            cnt++;
            CHECK(list.getSize() == cnt);
        }
        for (Package<int>& pack : p) {
            list.pushFront(pack);
            cnt++;
            CHECK(list.getSize() == cnt);
        }
    }
    SUBCASE("Clear") {
        CDualLinkedList<Package<int>> list;
        for (Package<int>& pack : p) {
            list.pushBack(pack);
        }
        list.clear();
        CHECK(list.getSize() == 0);
    }
    SUBCASE("Erase") {
        CDualLinkedList<Package<int>> list;
        for (Package<int>& pack : p) {
            list.pushBack(pack);
        }
        for (auto it = list.begin(); it.isValid(); ++it, ++it) {
            list.erase(it);
        }
        for (auto it = list.end(); it.isValid(); --it, --it) {
            list.eraseAndNext(it);
        }
        for (int i = (SIZE % 4 == 0 || SIZE % 4 == 1) ? 1 : 3; i < SIZE; i += 4) {
            CHECK(list.popFront().val == p[i].val);
        }
    }
    SUBCASE("IteratorForward") {
        CDualLinkedList<Package<int>> list;
        for (Package<int>& pack : p) {
            list.pushBack(pack);
        }
        int i = 0;
        for (auto it = list.begin(); it.isValid(); ++it, ++i) {
            CHECK(it.getData().val == p[i].val);
        }
    }
    SUBCASE("IteratorBackward") {
        CDualLinkedList<Package<int>> list;
        for (Package<int>& pack : p) {
            list.pushFront(pack);
        }
        int i = 0;
        for (auto it = list.end(); it.isValid(); --it, ++i) {
            CHECK(it.getData().val == p[i].val);
        }
    }
}