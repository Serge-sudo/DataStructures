#ifndef MEMORY_MANAGER_HEAD_H_2023_02_10
#define MEMORY_MANAGER_HEAD_H_2023_02_10
#include <set>
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

            // Массив данных блока
            T *pdata;
            // Адрес следующего блока
            block *pnext;
            // Первая свободная ячейка
            int firstFreeIndex;
            // Число заполненных ячеек
            int usedCount;
        };

     public:
        class CException : public std::exception {
         public:
            CException() = default;
        };

     public:
        /**
          _default_block_size - количество элементов в блоке данных
          isDeleteElementsOnDestruct - уничтожать елементы в деструкторе менеджера или проверять 
                                       на наличие неосвобожденных функцией deleteObject элементов.
        */
        explicit CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false) : m_blkSize(
            _default_block_size), m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct), m_pBlocks(nullptr),
                                                                                                    m_pCurrentBlk(
                                                                                                        nullptr) {
        }

        virtual ~CMemoryManager() {
            clear();
        }

        // Получить адрес нового элемента из менеджера
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

        // Освободить элемент в менеджере
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

        // Очистка данных, зависит от m_isDeleteElementsOnDestruct
        void clear() {
            m_pCurrentBlk = m_pBlocks;
            m_pBlocks = nullptr;
            while (m_pCurrentBlk) {
                block *pTmp = m_pCurrentBlk->pnext;
                if (!m_isDeleteElementsOnDestruct && m_pCurrentBlk->usedCount) {
                    throw CException();
                }
                deleteBlock(m_pCurrentBlk);
                m_pCurrentBlk = pTmp;
            }

        }

     private:

        // Создать новый блок данных. применяется в newObject
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

        // Освободить память блока данных. Применяется в clear
        void deleteBlock(block *p) {
            if (m_isDeleteElementsOnDestruct) {
                bool *pEmpty = new bool[m_blkSize];
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
                delete[] pEmpty;
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

        // Размер блока
        int m_blkSize;
        // Начало списка блоков
        block *m_pBlocks;
        
        // Текущий блок
        block *m_pCurrentBlk;
        // Удалять ли элементы при освобождении
        bool m_isDeleteElementsOnDestruct;
    };
}; // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2023_02_10
