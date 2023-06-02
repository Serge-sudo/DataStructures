#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "sort.cpp"
#include "doctest.h"
#include <string>
#include <random>

bool is_sorted(void **array, int length, templates::CompareSortType pCompareFunc) {
    for (int i = 1; i < length; i++) {
        if (pCompareFunc(array[i - 1], array[i]) > 0) {
            return false;
        }
    }
    return true;
}

struct Person {
    std::string name;
    int age;
    std::string address;
};

int comparePerson(const void *a, const void *b) {
    const Person *pa = static_cast<const Person *>(a);
    const Person *pb = static_cast<const Person *>(b);
    int cmp = pa->name.compare(pb->name);
    return cmp ? cmp : pa->age - pb->age;
}

TEST_CASE("MergeSort") {
    SUBCASE("string") {
        const int length = 6;
        std::string array[length] = {"apple", "banana", "orange", "lemon", "grape", "kiwi"};
        void *ppArray[length] = {&array[0], &array[1], &array[2], &array[3], &array[4],
                                 &array[5]};
        templates::mergeSort(ppArray, length, [](const void *a, const void *b) -> int {
            const std::string *first = static_cast<const std::string *>(a);
            const std::string *second = static_cast<const std::string *>(b);
            return (*first).compare(*second);
        });
        CHECK(is_sorted(ppArray, length, [](const void *a, const void *b) -> int {
            const std::string *first = static_cast<const std::string *>(a);
            const std::string *second = static_cast<const std::string *>(b);
            return (*first).compare(*second);
        }));
    }
    SUBCASE("Int") {
        const int length = 10;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(-100, 100);
        std::vector<int> list(length);
        for (int i = 0; i < length; ++i) {
            list[i] = dis(gen);
        }
        void **array = new void *[length];
        for (int i = 0; i < length; ++i) {
            array[i] = &list[i];
        }

        templates::mergeSort(array, length, [](const void *a, const void *b) -> int {
            const int *first = static_cast<const int *>(a);
            const int *second = static_cast<const int *>(b);
            return (*first - *second);
        });

        CHECK(is_sorted(array, length, [](const void *a, const void *b) -> int {
            const int *first = static_cast<const int *>(a);
            const int *second = static_cast<const int *>(b);
            return (*first - *second);
        }));
    }
    SUBCASE("Double") {
        const int length = 1e5;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(DBL_MIN / 2, DBL_MAX / 2);
        std::vector<double> list(length);
        for (int i = 0; i < length; ++i) {
            list[i] = dis(gen);
        }
        void **array = new void *[length];
        for (int i = 0; i < length; ++i) {
            array[i] = &list[i];
        }

        templates::mergeSort(array, length, [](const void *a, const void *b) -> int {
            const double *first = static_cast<const double *>(a);
            const double *second = static_cast<const double *>(b);
            return (*first - *second);
        });

        CHECK(is_sorted(array, length, [](const void *a, const void *b) -> int {
            const double *first = static_cast<const double *>(a);
            const double *second = static_cast<const double *>(b);
            return (*first - *second);
        }));
    }
    SUBCASE("CustomType") {
        const int length = 10;
        std::vector<Person> list;
        list.push_back({"Ivan", 20, "Moscow"});
        list.push_back({"Petr", 30, "Moscow"});
        list.push_back({"Sidor", 40, "Moscow"});
        list.push_back({"Ivan", 50, "Moscow"});
        list.push_back({"Petr", 60, "Moscow"});
        list.push_back({"Sidor", 70, "Moscow"});
        list.push_back({"Ivan", 80, "Moscow"});
        list.push_back({"Petr", 90, "Moscow"});
        list.push_back({"Sidor", 100, "Moscow"});
        list.push_back({"Ivan", 110, "Moscow"});
        void **array = new void *[length];
        for (int i = 0; i < length; ++i) {
            array[i] = &list[i];
        }
        templates::mergeSort(array, length, comparePerson);
        CHECK(is_sorted(array, length, comparePerson));
    }
};

TEST_CASE("HeapSort") {
    SUBCASE("string") {
        const int length = 6;
        std::string array[length] = {"apple", "banana", "orange", "lemon", "grape", "kiwi"};
        void *ppArray[length] = {&array[0], &array[1], &array[2], &array[3], &array[4],
                                 &array[5]};
        templates::heapSort(ppArray, length, [](const void *a, const void *b) -> int {
            const std::string *first = static_cast<const std::string *>(a);
            const std::string *second = static_cast<const std::string *>(b);
            return (*first).compare(*second);
        });
        CHECK(is_sorted(ppArray, length, [](const void *a, const void *b) -> int {
            const std::string *first = static_cast<const std::string *>(a);
            const std::string *second = static_cast<const std::string *>(b);
            return (*first).compare(*second);
        }));
    }
    SUBCASE("Int") {
        const int length = 1e5;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(INT32_MIN / 2, INT32_MAX / 2);
        std::vector<int> list(length);
        for (int i = 0; i < length; ++i) {
            list[i] = dis(gen);
        }
        void **array = new void *[length];
        for (int i = 0; i < length; ++i) {
            array[i] = &list[i];
        }

        templates::heapSort(array, length, [](const void *a, const void *b) -> int {
            const int *first = static_cast<const int *>(a);
            const int *second = static_cast<const int *>(b);
            return (*first - *second);
        });


        CHECK(is_sorted(array, length, [](const void *a, const void *b) -> int {
            const int *first = static_cast<const int *>(a);
            const int *second = static_cast<const int *>(b);
            return (*first - *second);
        }));
    }
    SUBCASE("Double") {
        const int length = 1e5;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(DBL_MIN / 2, DBL_MAX / 2);
        std::vector<double> list(length);
        for (int i = 0; i < length; ++i) {
            list[i] = dis(gen);
        }
        void **array = new void *[length];
        for (int i = 0; i < length; ++i) {
            array[i] = &list[i];
        }

        templates::heapSort(array, length, [](const void *a, const void *b) -> int {
            const double *first = static_cast<const double *>(a);
            const double *second = static_cast<const double *>(b);
            return (*first - *second);
        });

        CHECK(is_sorted(array, length, [](const void *a, const void *b) -> int {
            const double *first = static_cast<const double *>(a);
            const double *second = static_cast<const double *>(b);
            return (*first - *second);
        }));
    }
    SUBCASE("CustomType") {
        const int length = 10;
        std::vector<Person> list;
        list.push_back({"Ivan", 20, "Moscow"});
        list.push_back({"Petr", 30, "Moscow"});
        list.push_back({"Sidor", 40, "Moscow"});
        list.push_back({"Ivan", 50, "Moscow"});
        list.push_back({"Petr", 60, "Moscow"});
        list.push_back({"Sidor", 70, "Moscow"});
        list.push_back({"Ivan", 80, "Moscow"});
        list.push_back({"Petr", 90, "Moscow"});
        list.push_back({"Sidor", 100, "Moscow"});
        list.push_back({"Ivan", 110, "Moscow"});
        void **array = new void *[length];
        for (int i = 0; i < length; ++i) {
            array[i] = &list[i];
        }
        templates::heapSort(array, length, comparePerson);
        CHECK(is_sorted(array, length, comparePerson));
    }
};