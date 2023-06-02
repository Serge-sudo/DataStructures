#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "avltree.h"
#include <cstring>

struct TestStruct {
    int value;
    std::string str;
};

int compareTestStruct(const TestStruct *pElement, const TestStruct *pElement2) {
    return pElement->value - pElement2->value;
}

TEST_CASE("UNIT TEST") {
    lab618::CAVLTree<TestStruct, compareTestStruct> tree(1000);
    std::vector<TestStruct> ts;
    for (int i = 0; i < 10000; i++) {
        ts.push_back(TestStruct{i, ""});
    }
    for (int i = 500; i < 10000; ++i) {
        CHECK(tree.add(&ts[i]));
    }
    for (int i = 0; i < 500; ++i) {
        CHECK(tree.add(&ts[i]));
    }
    std::vector<TestStruct> ts2 = ts;
    for (int i = 0; i < 10000; i++) {
        ts2[i].str = std::to_string(i);
    }
    for (int i = 0; i < 10000; ++i) {
        CHECK(tree.update(&ts2[i]));
    }
    for (int i = 0; i < 10000; ++i) {
        CHECK(tree.find(ts[i])->str == ts2[i].str);
    }
    for (int i = 500; i < 10000; ++i) {
        CHECK(tree.remove(ts[i]));
    }
    for (int i = 0; i < 500; ++i) {
        CHECK(tree.remove(ts[i]));
    }
}

TEST_CASE("AVL add") {
    lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

    TestStruct ts1{1, "one"};
    TestStruct ts2{2, "two"};
    TestStruct ts3{3, "three"};

    REQUIRE(tree.add(&ts1) == true);
    REQUIRE(tree.add(&ts2) == true);
    REQUIRE(tree.add(&ts3) == true);
    REQUIRE(tree.add(&ts1) == false);
}

TEST_CASE("AVL find") {
    lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

    TestStruct ts1{1, "one"};
    TestStruct ts2{2, "two"};
    TestStruct ts3{3, "three"};

    tree.add(&ts1);
    tree.add(&ts2);
    tree.add(&ts3);

    TestStruct *found = tree.find(ts2);

    REQUIRE(found != nullptr);
    REQUIRE(found->value == 2);
    REQUIRE(found->str == "two");
}

TEST_CASE("AVL update") {
    lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

    TestStruct ts1{1, "one"};
    TestStruct ts2{2, "two"};
    TestStruct ts3{3, "three"};

    tree.add(&ts1);
    tree.add(&ts2);

    TestStruct ts2_alt{2, "TWO"};
    REQUIRE(tree.update(&ts2_alt) == true);

    TestStruct *found = tree.find(ts2);

    REQUIRE(found != nullptr);
    REQUIRE(found->value == 2);
    REQUIRE(found->str == "TWO");
}

TEST_CASE("AVL remove") {
    lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

    TestStruct ts1{1, "one"};
    TestStruct ts2{2, "two"};
    TestStruct ts3{3, "three"};

    tree.add(&ts1);
    tree.add(&ts2);
    tree.add(&ts3);

    REQUIRE(tree.remove(ts2) == true);

    TestStruct *found = tree.find(ts2);
    REQUIRE(found == nullptr);
}

TEST_CASE("AVL clear") {
    lab618::CAVLTree<TestStruct, compareTestStruct> tree(10);

    TestStruct ts1{1, "one"};
    TestStruct ts2{2, "two"};
    TestStruct ts3{3, "three"};

    tree.add(&ts1);
    tree.add(&ts2);
    tree.add(&ts3);

    tree.clear();
    REQUIRE(tree.find(ts1) == nullptr);
    REQUIRE(tree.find(ts2) == nullptr);
    REQUIRE(tree.find(ts3) == nullptr);
}