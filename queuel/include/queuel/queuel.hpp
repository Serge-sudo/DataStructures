#include <stdexcept>

#ifndef QUEUE_L_HEADER_FILE_H
#define QUEUE_L_HEADER_FILE_H

template<typename T>
class QueueL {
    struct Node {
        T data_;
        Node *next_{nullptr};
        Node(const T& d) : data_(d) {}
    };
    Node *head{nullptr};
    Node *tail{nullptr};
    int32_t size_{0};

    void clear();
 public:
    QueueL() noexcept = default;

    ~QueueL() noexcept;
    QueueL(const QueueL<T>& other) noexcept;
    QueueL(QueueL<T>&& other) noexcept;
    QueueL& operator=(const QueueL<T>& other) noexcept;
    QueueL& operator=(QueueL<T>&& other) noexcept;

    bool empty() const noexcept;

    int32_t size() const noexcept;

    void push(const T& data) noexcept;

    void pop() noexcept;

    const T& front() const;

};

template<class T>
QueueL<T>::~QueueL() noexcept {
    clear();
}

template<class T>
bool QueueL<T>::empty() const noexcept {
    return head == nullptr;
}

template<class T>
void QueueL<T>::clear() {
    Node *current = head;
    while (current) {
        Node *next = current->next_;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    size_ = 0;
}

template<class T>
void QueueL<T>::push(const T& data) noexcept {
    Node *newNode = new Node(data);
    size_++;
    if (tail == nullptr) {
        head = tail = newNode;
        return;
    }
    tail->next_ = newNode;
    tail = newNode;
}

template<class T>
void QueueL<T>::pop() noexcept {
    if (empty()) {
        return;
    }
    Node *temp = head;
    head = head->next_;
    if (head == nullptr) {
        tail = nullptr;
    }
    delete temp;
    size_--;
}

template<class T>
const T& QueueL<T>::front() const {
    if (empty()) {
        throw std::runtime_error("Queue is empty");
    }
    return head->data_;
}

template<class T>
int32_t QueueL<T>::size() const noexcept {
    return size_;
}

template<typename T>
QueueL<T>::QueueL(const QueueL<T>& other) noexcept {
    Node *current = other.head;
    while (current) {
        push(current->data_);
        current = current->next_;
    }
}

template<typename T>
QueueL<T>::QueueL(QueueL<T>&& other) noexcept
    : head{other.head}, tail{other.tail}, size_{other.size_} {
    other.head = other.tail = nullptr;
    other.size_ = 0;
}

template<typename T>
QueueL<T>& QueueL<T>::operator=(const QueueL<T>& other) noexcept {
    if (this != &other) {
        if (other.empty()) {
            clear();
        }
        Node *prev = nullptr;
        Node *current = head;
        Node *other_current = other.head;
        while (other_current) {
            if (current) {
                current->data_ = other_current->data_;
                prev = current;
                current = current->next_;
            } else {
                push(other_current->data_);
            }
            other_current = other_current->next_;
        }
        if (current) {
            tail = prev;
            prev->next_ = nullptr;
            while (current) {
                Node *next = current->next_;
                delete current;
                size_--;
                current = next;
            }
        }
    }
    return *this;
}

template<typename T>
QueueL<T>& QueueL<T>::operator=(QueueL<T>&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        size_ = other.size_;
        other.head = other.tail = nullptr;
        other.size_ = 0;
    }
    return *this;
}

#endif // QUEUE_L_HEADER_FILE_H