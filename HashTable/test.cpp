#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <unordered_map>
#include "doctest.h"
#include "hash.h"

namespace {
    unsigned int dummy_hash(const std::string *str) {
        return str->size();
    }

    int dummy_compare(const std::string *str1, const std::string *str2) {
        return str1->compare(*str2);
    }
}

TEST_CASE("Basic operations") {

    lab618::CHash<std::string, dummy_hash, dummy_compare> hash_table(10, 10);

    std::string str1 = "hello";
    REQUIRE(hash_table.add(&str1));
    std::string str2 = "world";
    REQUIRE(hash_table.add(&str2));

    std::string str3 = "hello";
    bool updated = hash_table.update(&str3);
    REQUIRE(updated);

    std::string *found = hash_table.find(str1);
    REQUIRE(found != nullptr);
    CHECK(*found == str3);

    bool removed = hash_table.remove(str1);
    REQUIRE(removed);
    found = hash_table.find(str1);
    REQUIRE(found == nullptr);

    hash_table.clear();
    found = hash_table.find(str2);
    REQUIRE(found == nullptr);
}

TEST_CASE("Handling collisions") {

    lab618::CHash<std::string, dummy_hash, dummy_compare> hash_table(2, 10);

    std::string str1 = "aa";
    REQUIRE(hash_table.add(&str1));
    std::string str2 = "bb";
    REQUIRE(hash_table.add(&str2));
    std::string str3 = "cc";
    REQUIRE(hash_table.add(&str3));

    std::string *found1 = hash_table.find(str1);
    REQUIRE(found1 != nullptr);
    CHECK(*found1 == str1);
    std::string *found2 = hash_table.find(str2);
    REQUIRE(found2 != nullptr);
    CHECK(*found2 == str2);
    std::string *found3 = hash_table.find(str3);
    REQUIRE(found3 != nullptr);
    CHECK(*found3 == str3);

    bool removed = hash_table.remove(str1);
    REQUIRE(removed);
    found1 = hash_table.find(str1);
    REQUIRE(found1 == nullptr);
    found2 = hash_table.find(str2);
    REQUIRE(found2 != nullptr);
    CHECK(*found2 == str2);
    found3 = hash_table.find(str3);
    REQUIRE(found3 != nullptr);
    CHECK(*found3 == str3);
}

unsigned int intHash(const int *pKey) {
    return *pKey;
}

int intCompare(const int *pKey1, const int *pKey2) {
    return *pKey1 - *pKey2;
}

TEST_CASE("Custom unordered_map vs std::unordered_map") {

    lab618::CHash<int, intHash, intCompare> customMap(10000, 10000);
    std::unordered_map<int, int> stdMap;

    for (int i = 0; i < 100000; i++) {
        customMap.add(new int(i));
        stdMap[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; i++) {
        customMap.find(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto customTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; i++) {
        stdMap.find(i);
    }
    end = std::chrono::high_resolution_clock::now();
    auto stdTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Custom time: " << customTime.count() << std::endl;
    std::cout << "Std time: " << stdTime.count() << std::endl;
}