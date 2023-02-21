#ifndef MEMORY_MANAGER_HEAD_H_2023_02_10
#define MEMORY_MANAGER_HEAD_H_2023_02_10

#include <cstring>
#include <iostream>


namespace lab618 {
    template<class T>
    class CMemoryManager {
    private:
        struct block {
            explicit block(int size) : firstFreeIndex(0), usedCount(0), pnext(nullptr) {
                pdata = reinterpret_cast<T *>(new char[size * sizeof(T)]);
                for (int i = 0; i < size - 1; ++i) {
                    *reinterpret_cast<int *>(pdata + i) = i + 1;
                }
                *reinterpret_cast<int *>(pdata + size - 1) = -1;
            }

            // ������ ������ �����
            T *pdata;
            // ����� ���������� �����
            block *pnext;
            // ������ ��������� ������
            int firstFreeIndex;
            // ����� ����������� �����
            int usedCount;
        };

    public:
        class CException : public std::exception {
        public:
            CException() = default;
        };

    public:
        /**
          _default_block_size - ���������� ��������� � ����� ������
          isDeleteElementsOnDestruct - ���������� �������� � ����������� ��������� ��� ��������� 
                                       �� ������� ��������������� �������� deleteObject ���������.
        */
        explicit CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false) : m_blkSize(
                _default_block_size), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct), m_pBlocks(nullptr),
                                                                                                    m_pCurrentBlk(
                                                                                                            nullptr) {
        }

        virtual ~CMemoryManager() {
            clear();
        }


        // �������� ����� ������ �������� �� ���������
        T *newObject() {
            if (m_pCurrentBlk == nullptr || m_pCurrentBlk->firstFreeIndex == -1) {
                m_pCurrentBlk = m_pBlocks;
                while (m_pCurrentBlk && m_pCurrentBlk->firstFreeIndex == -1) {
                    m_pCurrentBlk = m_pCurrentBlk->pnext;
                }
                if (m_pCurrentBlk == nullptr) {
                    m_pCurrentBlk = newBlock();
                }
            }

            int oldInd = m_pCurrentBlk->firstFreeIndex;
            ++m_pCurrentBlk->usedCount;
            T *new_element = m_pCurrentBlk->pdata + oldInd;
            m_pCurrentBlk->firstFreeIndex = *reinterpret_cast<int *>(new_element);
            ConstructElements(new_element);
            return new_element;
        }

        // ���������� ������� � ���������
        bool deleteObject(T *p) {
            block *pTmp = m_pBlocks;
            while (pTmp != nullptr) {

                if (p < pTmp->pdata || p >= pTmp->pdata + m_blkSize) {
                    pTmp = pTmp->pnext;
                    continue;
                }

                DestructElements(p);
                *reinterpret_cast<int *> (p) = pTmp->firstFreeIndex;
                --pTmp->usedCount;
                pTmp->firstFreeIndex = p - pTmp->pdata;
                return true;
            }
            return false;
        }

        // ������� ������, ������� �� m_isDeleteElementsOnDestruct
        void clear() {
            m_pCurrentBlk = m_pBlocks;
            m_pBlocks = nullptr;
            bool *pEmpty = new bool[m_blkSize];
            while (m_pCurrentBlk) {
                block *pTmp = m_pCurrentBlk->pnext;
                if (!m_isDeleteElementsOnDestruct && m_pCurrentBlk->usedCount) {
                    throw CException();
                }
                deleteBlock(m_pCurrentBlk, pEmpty);
                m_pCurrentBlk = pTmp;
            }
            delete[] pEmpty;

        }

    private:

        // ������� ����� ���� ������. ����������� � newObject
        block *newBlock() {
            if (m_pBlocks == nullptr) {
                m_pCurrentBlk = m_pBlocks = new block(m_blkSize);
                return m_pBlocks;
            }
            block *pTmp = m_pBlocks;
            while (pTmp->pnext) {
                pTmp = pTmp->pnext;
            }
            pTmp->pnext = new block(m_blkSize);
            return pTmp->pnext;
        }


        // ���������� ������ ����� ������. ����������� � clear
        void deleteBlock(block *p, bool *pEmpty) {
            if (m_isDeleteElementsOnDestruct) {
                std::memset(pEmpty, 0, m_blkSize * sizeof(bool));
                while (p->firstFreeIndex != -1) {
                    pEmpty[p->firstFreeIndex] = 1;
                    p->firstFreeIndex = *reinterpret_cast<int *>(p->pdata + p->firstFreeIndex);
                }
                for (int i = 0; i < m_blkSize; ++i) {
                    if (pEmpty[i] == false) {
                        p->pdata[i].~T();
                        --p->usedCount;
                    }
                }
            }
            delete[] reinterpret_cast<char *>(p->pdata);
            p->pdata = nullptr;
            delete p;
            p = nullptr;
        }


        inline void ConstructElements(T *pElement) {
            memset(reinterpret_cast<void *>(pElement), 0, sizeof(T));
            ::new(reinterpret_cast<void *>(pElement)) T;
        }

        inline void DestructElements(T *pElement) {
            pElement->~T();
            memset(reinterpret_cast<void *>(pElement), 0, sizeof(T));
        }


        // ������ �����
        int m_blkSize;
        // ������ ������ ������
        block *m_pBlocks;
        // ������� ����
        block *m_pCurrentBlk;
        // ������� �� �������� ��� ������������
        bool m_isDeleteElementsOnDestruct;
    };
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2023_02_10
