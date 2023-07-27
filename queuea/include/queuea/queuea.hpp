#include <stdexcept>
#include <iostream>
#ifndef QUEUE_A_HEADER_FILE_H
#define QUEUE_A_HEADER_FILE_H

template<class T>
class QueueA {
 public:
    QueueA() noexcept;

    ~QueueA() noexcept;

    QueueA(const QueueA<T>& other) noexcept;
    QueueA(QueueA<T>&& other) noexcept;
    QueueA<T>& operator=(const QueueA<T>& other) noexcept;
    QueueA<T>& operator=(QueueA<T>&& other) noexcept;

    bool empty() const noexcept;

    int32_t size() const noexcept;

    void push(const T& item) noexcept;

    void pop() noexcept;

    const T& front() const;
 private:
    void expand() noexcept;

    T *data;
    uint32_t head{1};
    uint32_t tail{0};
    uint32_t capacity{1};
};

template<class T>
QueueA<T>::QueueA() noexcept {
    data = reinterpret_cast<T *>(new char[capacity * sizeof(T)]);
}

template<class T>
QueueA<T>::~QueueA() noexcept {
    for (int i = head; i <= tail; ++i) {
        data[i % capacity].~T();
    }
    delete[] reinterpret_cast<char *>(data);
}

template<class T>
bool QueueA<T>::empty() const noexcept {
    return tail < head;
}

template<class T>
void QueueA<T>::expand() noexcept {
    int32_t new_capacity = capacity * 2;
    T *new_data = reinterpret_cast<T *>(new char[new_capacity * sizeof(T)]);
    for (int i = 0; i < size(); ++i) {
        new(reinterpret_cast<void *>( &new_data[i]))  T(std::move(data[(head + i) % capacity]));
    }
    for (int i = 0; i < size(); ++i) {
        data[(head + i) % capacity].~T();
    }

    delete[] reinterpret_cast<char *>(data);
    data = new_data;
    tail = tail - head;
    head = 0;
    capacity = new_capacity;
}

template<class T>
void QueueA<T>::push(const T& item) noexcept {
    if (size() == capacity) {
        expand();
    }
    tail++;
    new(reinterpret_cast<void *>(&data[tail % capacity])) T(item);
}

template<class T>
void QueueA<T>::pop() noexcept {
    if (empty()) {
        return;
    }
    data[head % capacity].~T();
    head++;
}

template<class T>
const T& QueueA<T>::front() const {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    return data[head % capacity];
}

template<class T>
int32_t QueueA<T>::size() const noexcept {
    return tail - head + 1;
}

template<class T>
QueueA<T>::QueueA(const QueueA<T>& other) noexcept {
    if (other.empty()) {
        head = 1;
        tail = 0;
        capacity = 1;
        data = reinterpret_cast<T *>(new char[capacity * sizeof(T)]);
    } else {
        head = 0;
        tail = other.tail - other.head;
        capacity = other.capacity;
        data = reinterpret_cast<T *>(new char[capacity * sizeof(T)]);
        for (int i = 0; i < other.size(); ++i) {
            new(reinterpret_cast<void *>( &data[i]))  T(other.data[(other.head + i) % other.capacity]);
        }
    }

}

template<class T>
QueueA<T>::QueueA(QueueA<T>&& other) noexcept : data{other.data},
                                                head{other.head},
                                                tail{other.tail},
                                                capacity{other.capacity} {
    other.head = 1;
    other.tail = 0;
    other.capacity = 1;
    other.data = reinterpret_cast<T *>(new char[other.capacity * sizeof(T)]);
}

template<class T>
QueueA<T>& QueueA<T>::operator=(const QueueA<T>& other) noexcept {
    if (this != &other) {

        for (int i = head; i <= tail; ++i) {
            data[i % capacity].~T();
        }
        delete[] reinterpret_cast<char *>( data);

        if (other.empty()) {
            head = 1;
            tail = 0;
            capacity = 1;
            data = reinterpret_cast<T *>(new char[capacity * sizeof(T)]);
        } else {
            capacity = other.capacity;
            head = 0;
            tail = other.tail - other.head;
            data = reinterpret_cast<T *>( new char[capacity * sizeof(T)]);
            for (int i = 0; i < other.size(); ++i) {
                new(reinterpret_cast<void *>( &data[i]))  T(other.data[(other.head + i) % capacity]);
            }
        }

    }
    return *this;
}

template<class T>
QueueA<T>& QueueA<T>::operator=(QueueA<T>&& other) noexcept {
    if (this != &other) {
        for (int i = head; i <= tail; ++i) {
            data[i % capacity].~T();
        }
        delete[] reinterpret_cast<char *>( data);
        data = other.data;
        head = other.head;
        tail = other.tail;
        capacity = other.capacity;
        other.head = 1;
        other.tail = 0;
        other.capacity = 1;
        other.data = reinterpret_cast<T *>( new char[other.capacity * sizeof(T)]);
    }
    return *this;
}

#endif // QUEUE_A_HEADER_FILE_H

