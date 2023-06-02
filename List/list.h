#include<iostream>

#ifndef TEMPLATES_LIST_2023_02_02
#define TEMPLATES_LIST_2023_02_02

namespace lab618 {
    template<class T>
    class CSingleLinkedList {
    private:
        struct leaf {
            // Данные
            T data;
            // Указатель на следующий лист списка
            leaf *pNext;

            leaf(T &_data, leaf *_pNext) : data(_data), pNext(_pNext) {
            }

        };

    public:
        class CIterator {
        public:
            CIterator()
                    : m_pCurrent(0), m_pBegin(0) {
            }

            CIterator(leaf *p) : m_pBegin(nullptr), m_pCurrent(p) {
            }

            CIterator(const CIterator &src) = default;

            ~CIterator() = default;

            CIterator &operator=(const CIterator &src) = default;

            bool operator!=(const CIterator &it) const {
                return (m_pBegin != it.m_pBegin) || (m_pCurrent != it.m_pCurrent);
            }

            void operator++() {
                if (m_pCurrent) {
                    m_pCurrent = m_pCurrent->pNext;
                    return;
                }
                m_pCurrent = m_pBegin;
                m_pBegin = nullptr;
            }

            T &getData() {
                return m_pCurrent->data;
            }

            T &operator*() {
                return m_pCurrent->data;
            }

            leaf *getLeaf() {
                return m_pCurrent;
            }

            void setLeaf(leaf *p) {
                m_pCurrent = p;
            }

            void setLeafPreBegin(leaf *p) {
                m_pCurrent = nullptr;
                m_pBegin = p;
            }

            bool isValid() {
                return m_pCurrent != nullptr;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf *m_pBegin;
            // храним текущее положение
            leaf *m_pCurrent;
        };

    public:

        CSingleLinkedList() : m_pBegin(nullptr), m_pEnd(nullptr) {};

        virtual ~CSingleLinkedList() {
            clear();
        }

        void pushBack(T &data) {
            if (m_pEnd == nullptr) {
                m_pEnd = m_pBegin = new leaf(data, nullptr);
                return;
            }
            m_pEnd->pNext = new leaf(data, nullptr);
            m_pEnd = m_pEnd->pNext;

        }

        void pushFront(T &data) {
            if (m_pBegin == nullptr) {
                m_pEnd = m_pBegin = new leaf(data, nullptr);
                return;
            }
            leaf *newHead = new leaf(data, m_pBegin);
            m_pBegin = newHead;
        }

        T popFront() {
            T tmp = m_pBegin->data;
            leaf *newHead = m_pBegin->pNext;
            delete m_pBegin;
            m_pBegin = newHead;
            if (m_pBegin == nullptr)
                m_pEnd = nullptr;
            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator &it) {
            leaf *target = it.getLeaf();

            if (target == m_pBegin) {
                m_pBegin = target->pNext;
                it.setLeafPreBegin(m_pBegin);
            } else {
                leaf *tmp = m_pBegin;
                while (tmp->pNext != target) {
                    tmp = tmp->pNext;
                }
                it.setLeaf(tmp);
                tmp->pNext = target->pNext;
                if (tmp->pNext == nullptr) {
                    m_pEnd = tmp;
                }
            }

            delete target;
        }

        int getSize() {
            int cnt = 0;
            leaf *tmp = m_pBegin;
            while (tmp) {
                cnt++;
                tmp = tmp->pNext;
            }
            return cnt;
        }

        void clear() {
            while (m_pBegin) {
                leaf *newTail = m_pBegin->pNext;
                delete m_pBegin;
                m_pBegin = newTail;
            }
            m_pEnd = nullptr;
        }

        CIterator begin() {
            return CIterator(m_pBegin);
        }

    private:
        //храним голову и хвост списка
        leaf *m_pBegin, *m_pEnd;
    };

    template<class T>
    class CDualLinkedList {
    private:
        struct leaf {
            // Данные
            T data;
            // Указатель на предыдущий / следующий лист списка
            leaf *pNext, *pPrev;

            leaf(T &_data, leaf *_pPrev, leaf *_pNext) : data(_data), pPrev(_pPrev), pNext(_pNext) {
            }
        };

    public:
        class CIterator {
        public:
            CIterator()
                    : m_pCurrent(nullptr), m_pBegin(nullptr), m_pEnd(nullptr) {
            }

            CIterator(leaf *p) : m_pCurrent(p), m_pBegin(nullptr), m_pEnd(nullptr) {}

            CIterator(const CIterator &src) = default;

            ~CIterator() = default;

            CIterator &operator=(const CIterator &src) = default;

            bool operator!=(const CIterator &it) const {
                return m_pBegin != it.m_pBegin || m_pCurrent != it.m_pCurrent || m_pEnd != it.m_pEnd;
            }

            void operator++() {
                if (m_pCurrent) {
                    m_pCurrent = m_pCurrent->pNext;
                    return;
                }
                m_pCurrent = m_pBegin;
                m_pBegin = nullptr;
            }

            void operator--() {
                if (m_pCurrent) {
                    m_pCurrent = m_pCurrent->pPrev;
                    return;
                }
                m_pCurrent = m_pEnd;
                m_pEnd = nullptr;
            }

            T &getData() {
                return m_pCurrent->data;
            }

            T &operator*() {
                return m_pCurrent->data;
            }

            leaf *getLeaf() {
                return m_pCurrent;
            }

            // применяется в erase и eraseAndNext
            void setLeaf(leaf *p) {
                m_pCurrent = p;
            }

            // применяется в erase и eraseAndNext
            void setLeafPreBegin(leaf *p) {
                m_pBegin = p;
                m_pCurrent = nullptr;
                m_pEnd = nullptr;
            }

            // применяется в erase и eraseAndNext
            void setLeafPostEnd(leaf *p) {
                m_pEnd = p;
                m_pCurrent = nullptr;
                m_pBegin = nullptr;
            }

            bool isValid() {
                return m_pCurrent != nullptr;
            }

        private:
            //храним голову списка, если мы находимся перед началом
            leaf *m_pBegin;
            // храним текущее положение
            leaf *m_pCurrent;
            //храним конец списка, если мы находимся после конца
            leaf *m_pEnd;
        };

    public:

        CDualLinkedList() : m_pBegin(nullptr), m_pEnd(nullptr) {};

        virtual ~CDualLinkedList() {
            clear();
        };

        void pushBack(T &data) {
            if (m_pEnd == nullptr) {
                m_pBegin = m_pEnd = new leaf(data, nullptr, nullptr);
                return;
            }
            m_pEnd->pNext = new leaf(data, m_pEnd, nullptr);
            m_pEnd = m_pEnd->pNext;
        }

        T popBack() {
            T tmp = m_pEnd->data;
            leaf *newTail = m_pEnd->pPrev;
            delete m_pEnd;
            m_pEnd = newTail;
            if (m_pEnd == nullptr)
                m_pBegin = nullptr;
            else
                m_pEnd->pNext = nullptr;
            return tmp;
        }

        void pushFront(T &data) {
            if (m_pBegin == nullptr) {
                m_pBegin = m_pEnd = new leaf(data, nullptr, nullptr);
                return;
            }
            m_pBegin->pPrev = new leaf(data, nullptr, m_pBegin);
            m_pBegin = m_pBegin->pPrev;
        }

        T popFront() {
            T tmp = m_pBegin->data;
            leaf *newHead = m_pBegin->pNext;
            delete m_pBegin;
            m_pBegin = newHead;
            if (m_pBegin == nullptr)
                m_pEnd = nullptr;
            else
                m_pBegin->pPrev = nullptr;
            return tmp;
        }

        // изменяет состояние итератора. выставляет предыдущую позицию.
        void erase(CIterator &it) {
            leaf *target = it.getLeaf();

            if (target == m_pBegin) {
                popFront();
                it.setLeafPreBegin(m_pBegin);
            } else {
                leaf *tmp = m_pBegin;
                while (tmp->pNext != target) {
                    tmp = tmp->pNext;
                }
                it.setLeaf(tmp);
                tmp->pNext = target->pNext;
                if (target->pNext) {
                    target->pNext->pPrev = tmp;
                } else {
                    m_pEnd = tmp;
                }
                delete target;
            }


        }

        // изменяет состояние итератора. выставляет следующую позицию.
        void eraseAndNext(CIterator &it) {
            leaf *target = it.getLeaf();

            if (target == m_pEnd) {
                popBack();
                it.setLeafPostEnd(m_pEnd);
            } else {
                leaf *tmp = m_pEnd;
                while (tmp->pPrev != target) {
                    tmp = tmp->pPrev;
                }
                it.setLeaf(tmp);
                tmp->pPrev = target->pPrev;
                if (target->pPrev) {
                    target->pPrev->pNext = tmp;
                } else {
                    m_pBegin = tmp;
                }
                delete target;
            }
        }

        int getSize() {
            int cnt = 0;
            leaf *tmp = m_pBegin;
            while (tmp) {
                cnt++;
                tmp = tmp->pNext;
            }
            return cnt;
        }

        void clear() {
            while (m_pBegin) {
                leaf *newHead = m_pBegin->pNext;
                delete m_pBegin;
                m_pBegin = newHead;
            }
            m_pEnd = nullptr;
        }

        CIterator begin() {
            return CIterator(m_pBegin);
        }

        CIterator end() {
            return CIterator(m_pEnd);
        }

    private:
        //храним голову и хвост списка
        leaf *m_pBegin, *m_pEnd;
    };
};
#endif //#ifndef TEMPLATES_LIST_2022_02_03
