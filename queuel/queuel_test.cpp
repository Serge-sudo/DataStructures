#include <doctest/doctest.h>
#include <queuel/queuel.hpp>
#include <chrono>
#include <queue>
#include <random>
#include <iostream>
#include <fstream>


TEST_CASE("STRESS") {

    const int num_elements = 1000000;

    QueueL<int> q1;
    for (int i = 0; i < num_elements; ++i) {
        q1.push(i);
    }

    QueueL<int> q2(q1);

    QueueL<int> q3(std::move(q1));

    REQUIRE(q1.size() == 0);
    REQUIRE(q1.empty() == true);

    q1 = q2;

    q1 = std::move(q3);

    std::vector<QueueL<int> *> queues{&q1, &q2};
    for (auto q : queues) {
        REQUIRE(q->size() == num_elements);
        for (int i = 0; i < num_elements; ++i) {
            REQUIRE(q->front() == i);
            q->pop();
        }
        REQUIRE(q->empty() == true);
    }
    REQUIRE(q3.size() == 0);
    REQUIRE(q3.empty() == true);


}

TEST_CASE("Queue functionality") {
    QueueL<int> q;

    SUBCASE("Empty queuelt throws errors") {
        CHECK(q.empty());
        CHECK_EQ(q.size(), 0);

        CHECK_THROWS(q.front());
        CHECK_NOTHROW(q.pop());
    }

    SUBCASE("Push and pop elements") {
        CHECK(q.empty());

        q.push(5);

        CHECK_FALSE(q.empty());
        CHECK_EQ(q.size(), 1);
        CHECK_EQ(q.front(), 5);

        q.push(10);

        CHECK_EQ(q.size(), 2);
        CHECK_EQ(q.front(), 5);

        CHECK_EQ(q.front(), 5);
        q.pop();
        CHECK_EQ(q.size(), 1);
        CHECK_EQ(q.front(), 10);

        CHECK_EQ(q.front(), 10);
        q.pop();
        CHECK(q.empty());
        CHECK_THROWS(q.front());
        CHECK_NOTHROW(q.pop());
    }

    SUBCASE("Push and pop many elements") {
        CHECK(q.empty());

        for (int i = 0; i < 100000; i++) {
            q.push(i);
        }

        CHECK_FALSE(q.empty());
        CHECK_EQ(q.size(), 100000);
        CHECK_EQ(q.front(), 0);

        for (int i = 0; i < 99999; i++) {
            q.pop();
        }

        CHECK_EQ(q.size(), 1);
        CHECK_EQ(q.front(), 99999);

        CHECK_EQ(q.front(), 99999);
        q.pop();

        CHECK(q.empty());
        CHECK_THROWS(q.front());
        CHECK_NOTHROW(q.pop());
    }

    SUBCASE("Resize queuelt capacity") {
        CHECK(q.empty());

        for (int i = 0; i < 10; i++) {
            q.push(i);
        }

        CHECK_EQ(q.size(), 10);

        for (int i = 10; i < 20; i++) {
            q.push(i);
        }

        CHECK_EQ(q.size(), 20);

        for (int i = 0; i < 10; i++) {
            CHECK_EQ(q.front(), i);
            q.pop();
        }

        for (int i = 20; i < 30; i++) {
            q.push(i);
        }

        CHECK_EQ(q.size(), 20);

        for (int i = 10; i < 30; i++) {
            CHECK_EQ(q.front(), i);
            q.pop();
        }

        CHECK(q.empty());
        CHECK_THROWS(q.front());
        CHECK_NOTHROW(q.pop());
    }

    SUBCASE("Mix push and pop operations") {
        CHECK(q.empty());

        q.push(5);
        q.push(10);
        CHECK_EQ(q.size(), 2);

        CHECK_EQ(q.front(), 5);
        q.pop();
        q.push(15);
        CHECK_EQ(q.size(), 2);
        CHECK_EQ(q.front(), 10);

        CHECK_EQ(q.front(), 10);
        q.pop();
        CHECK_EQ(q.front(), 15);
        q.pop();
    }
}

TEST_CASE("QueueL copy constructor") {
    QueueL<int> q1;
    q1.push(1);
    q1.push(2);
    q1.push(3);

    QueueL<int> q2 = q1;

    REQUIRE(q1.front() == q2.front());
    q1.pop();
    REQUIRE(q1.front() != q2.front());
    REQUIRE(q1.size() == q2.size() - 1);
}

TEST_CASE("QueueL move constructor") {
    QueueL<int> q1;
    q1.push(1);
    q1.push(2);
    q1.push(3);

    QueueL<int> q2 = std::move(q1);

    REQUIRE(q2.size() == 3);
    REQUIRE(q2.front() == 1);
    REQUIRE(q1.empty());
}

TEST_CASE("QueueL copy assignment operator") {
    QueueL<int> q1;
    q1.push(1);
    q1.push(2);
    q1.push(3);

    QueueL<int> q2;
    q2.push(4);
    q2.push(5);
    q2.push(6);

    q2 = q1;

    REQUIRE(q2.size() == q1.size());
    REQUIRE(q2.front() == q1.front());

    q1.pop();
    REQUIRE(q1.front() != q2.front());
    REQUIRE(q1.size() == q2.size() - 1);
}

TEST_CASE("QueueL move assignment operator") {
    QueueL<int> q1;
    q1.push(1);
    q1.push(2);
    q1.push(3);

    QueueL<int> q2;
    q2.push(4);
    q2.push(5);
    q2.push(6);

    q2 = std::move(q1);

    REQUIRE(q2.size() == 3);
    REQUIRE(q2.front() == 1);
    REQUIRE(q1.empty());
}

class Person {
 public:
    Person(std::string name, int age) : mName(name), mAge(age), mPtr(new double(10)) {}
    ~Person() {
        delete mPtr;
    }
    Person(const Person& other) : mAge(other.mAge) {
        if (this == &other)
            return;
        mName = other.mName;
        mPtr = new double(*other.mPtr);
    }
    Person(Person&& other) : mAge(other.mAge) {
        if (this == &other)
            return;
        mName = std::move(other.mName);
        mPtr = other.mPtr;
        other.mPtr = nullptr;
    }
    std::string mName;
    int mAge;
    double *mPtr;
};

TEST_CASE("NoDefaultConstructor") {
    QueueL<Person> queue;
    REQUIRE(queue.empty());

    queue.push(Person("Alice", 30));
    queue.push(Person("Bob", 25));
    queue.push(Person("Charlie", 33));

    REQUIRE(queue.front().mName == "Alice");
    REQUIRE(queue.front().mAge == 30);

    queue.pop();

    REQUIRE(queue.front().mName == "Bob");
    REQUIRE(queue.front().mAge == 25);
    REQUIRE(queue.size() == 2);
}

TEST_CASE("queuelt performance test") {
    const int COUNT = 100;
    const int n = 100000;
    std::ofstream file("statQueueL.txt");

    for (int M = 2; M <= COUNT; ++M) {
        double timeAPush = 0.0;
        double timeStdPush = 0.0;
        double timeAPop = 0.0;
        double timeStdPop = 0.0;

        for (int i = 0; i < M; ++i) {
            QueueL<int> qA;
            std::queue<int> qStd;
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < n; ++j) {
                qA.push(j);
            }
            auto stop = std::chrono::high_resolution_clock::now();
            timeAPush += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < n; ++j) {
                qStd.push(j);
            }
            stop = std::chrono::high_resolution_clock::now();
            timeStdPush += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < n; ++j) {
                qA.pop();
            }
            stop = std::chrono::high_resolution_clock::now();
            timeAPop += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < n; ++j) {
                qStd.pop();
            }
            stop = std::chrono::high_resolution_clock::now();
            timeStdPop += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        }

        double avgAPush = timeAPush / M;
        double avgStdPush = timeStdPush / M;
        double avgAPop = timeAPop / M;
        double avgStdPop = timeStdPop / M;
        double stdDevAPush = 0.0;
        double stdDevStdPush = 0.0;
        double stdDevAPop = 0.0;
        double stdDevStdPop = 0.0;

        for (int i = 0; i < M; ++i) {
            QueueL<int> qA;
            std::queue<int> qStd;
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < n; ++j) {
                qA.push(j);
            }
            auto stop = std::chrono::high_resolution_clock::now();
            stdDevAPush +=
                std::pow(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() - avgAPush, 2);

            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < n; ++j) {
                qStd.push(j);
            }
            stop = std::chrono::high_resolution_clock::now();
            stdDevStdPush +=
                std::pow(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() - avgStdPush, 2);

            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < n; ++j) {
                qA.pop();
            }
            stop = std::chrono::high_resolution_clock::now();
            stdDevAPop +=
                std::pow(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() - avgAPop, 2);

            start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < n; ++j) {
                qStd.pop();
            }
            stop = std::chrono::high_resolution_clock::now();
            stdDevStdPop +=
                std::pow(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() - avgStdPop, 2);

        }

        double stdAPush = std::sqrt(stdDevAPush / (M - 1));
        double stdStdPush = std::sqrt(stdDevStdPush / (M - 1));
        double stdAPop = std::sqrt(stdDevAPop / (M - 1));
        double stdStdPop = std::sqrt(stdDevStdPop / (M - 1));

        std::cout << "-------------Info---------------" << std::endl;
        std::cout << "M = " << M << std::endl;
        std::cout << "n = " << n << std::endl;
        std::cout << "------------QueueL--------------" << std::endl;
        std::cout << "E(push) = " << avgAPush << " ms" << std::endl;
        std::cout << "σ(push) = " << stdAPush << " ms" << std::endl;
        std::cout << "E(pop) = " << avgAPop << " ms" << std::endl;
        std::cout << "σ(pop) = " << stdAPop << " ms" << std::endl;
        std::cout << "----------std::queuelt------------" << std::endl;
        std::cout << "E(push) = " << avgStdPush << " ms" << std::endl;
        std::cout << "σ(push) = " << stdStdPush << " ms" << std::endl;
        std::cout << "E(pop) = " << avgStdPop << " ms" << std::endl;
        std::cout << "σ(pop) = " << stdStdPop << " ms" << std::endl;
        file << M << " " << avgAPush << " " << stdAPush << " " << avgStdPush << " " << stdStdPush << " ";
        file << avgAPop << " " << stdAPop << " " << avgStdPop << " " << stdStdPop << std::endl;
    }
}