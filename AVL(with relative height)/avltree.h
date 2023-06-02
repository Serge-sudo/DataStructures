#ifndef AVL_HEAD_H_2023_03_30
#define AVL_HEAD_H_2023_03_30

#include "mm.h"

namespace lab618 {

    template<class T>
    T max(const T &val1, const T &val2) {
        return val1 > val2 ? val1 : val2;
    }

    template<class T, int(*Compare)(const T *pElement, const T *pElement2)>
    class CAVLTree {
    private:
        struct leaf {
            T *pData;
            leaf *pLeft;
            leaf *pRight;
            int balanceFactor;

            leaf() : balanceFactor(0),
                     pLeft(nullptr),
                     pRight(nullptr) {}

            leaf(T *data) : leaf() {
                pData = data;
            }
        };

    public:
        class CException {
        public:
            CException() {
            }
        };

    public:
        CAVLTree(int defaultBlockSize) : m_pRoot(nullptr), m_Memory(defaultBlockSize, true) {
        }

        virtual ~CAVLTree() {
            clear();
        }

        bool add(T *pElement) {
            leaf *existingNode = nullptr;
            Insert(&m_pRoot, pElement, &existingNode);
            return existingNode == nullptr;
        }

        bool update(T *pElement) {
            leaf *existingNode = nullptr;
            Insert(&m_pRoot, pElement, &existingNode);
            if (existingNode == nullptr) {
                return false;
            }
            existingNode->pData = pElement;
            return true;
        }

        T *find(const T &pElement) {
            leaf *node = find(m_pRoot, &pElement);
            if (node) {
                return node->pData;
            }
            return nullptr;
        }

        bool remove(const T &element) {
            bool removed = false;
            Remove(&m_pRoot, &element, removed);
            return removed;
        }

        void clear() {
            m_Memory.clear();
            m_pRoot = nullptr;
        }

    private:

        leaf *find(leaf *node, const T *data) {
            if (node == nullptr) return nullptr;
            int cmp = Compare(data, node->pData);
            if (cmp < 0) return find(node->pLeft, data);

            else if (cmp > 0) return find(node->pRight, data);

            return node;
        }


        int Balance(leaf **root, bool forDel = false, bool heightChanged = false) {
            leaf *node = *root;
            if (node->balanceFactor > 1) {
                heightChanged = node->pRight->balanceFactor;
                if (node->pRight->balanceFactor < 0) node->pRight = RotateRight(node->pRight);
                *root = RotateLeft(node);
                return forDel ? heightChanged : true;
            } else if (node->balanceFactor < -1) {
                heightChanged = node->pLeft->balanceFactor;
                if (node->pLeft->balanceFactor > 0) node->pLeft = RotateLeft(node->pLeft);
                *root = RotateRight(node);
                return forDel ? heightChanged : true;
            } else {
                return forDel ? heightChanged : false;
            }

        }

        bool Insert(leaf **root, T *value, leaf **existingNode) {
            bool heightChanged;
            leaf *node = *root;
            if (!node) {
                *root = m_Memory.newObject();
                (*root)->pData = value;
                return true;
            }
            int cmp = Compare(value, node->pData);
            if (cmp < 0)
                heightChanged = Insert(&node->pLeft, value, existingNode) && --node->balanceFactor;
            else if (cmp > 0)
                heightChanged = Insert(&node->pRight, value, existingNode) && ++node->balanceFactor;
            else {
                *existingNode = node;
                return false;
            }
            if (Balance(root)) heightChanged = false;
            return heightChanged;
        }

        void RotateRightHelper(int &parentBalance, int &childBalance) {
            if (parentBalance == -1) {
                if (childBalance == -1) childBalance = 1, parentBalance = 1;
                else if (childBalance == 0) childBalance = 1, parentBalance = 0;
                else if (childBalance == 1) childBalance = 2, parentBalance = 0;

            } else if (parentBalance == -2) {
                if (childBalance == -1) childBalance = 0, parentBalance = 0;
                else if (childBalance == -2) childBalance = 0, parentBalance = 1;
                else if (childBalance == 0) childBalance = 1, parentBalance = -1;
            }
        }

        leaf *RotateRight(leaf *node) {
            leaf *p1 = node->pLeft;
            leaf *p12 = p1->pRight;
            p1->pRight = node;
            node->pLeft = p12;
            RotateRightHelper(node->balanceFactor, p1->balanceFactor);
            return p1;
        }

        void RotateLeftHelper(int &parentBalance, int &childBalance) {
            if (parentBalance == 1) {
                if (childBalance == -1) childBalance = -2, parentBalance = 0;
                else if (childBalance == 0) childBalance = -1, parentBalance = 0;
                else if (childBalance == 1) childBalance = -1, parentBalance = -1;

            } else if (parentBalance == 2) {
                if (childBalance == 0) childBalance = -1, parentBalance = 1;
                else if (childBalance == 1) childBalance = 0, parentBalance = 0;
                else if (childBalance == 2) childBalance = 0, parentBalance = -1;
            }
        }

        leaf *RotateLeft(leaf *node) {
            leaf *p2 = node->pRight;
            leaf *p21 = p2->pLeft;
            p2->pLeft = node;
            node->pRight = p21;
            RotateLeftHelper(node->balanceFactor, p2->balanceFactor);
            return p2;
        }

        bool GetMin(leaf **root, leaf **heightChanged) {
            leaf *node = *root;
            if (node->pLeft) {
                if (GetMin(&node->pLeft, heightChanged) && !++node->balanceFactor) return true;
                return Balance(root, true);
            }
            *heightChanged = node;
            *root = node->pRight;
            return true;
        }

        bool Remove(leaf **root, const T *value, bool &removed) {
            bool heightChanged = false;
            leaf *node = *root;
            if (!node) return heightChanged;
            int cmp = Compare(node->pData, value);
            if (cmp < 0) {
                if (Remove(&node->pRight, value, removed) && !--node->balanceFactor) heightChanged = true;
            } else if (cmp > 0) {
                if (Remove(&node->pLeft, value, removed) && !++node->balanceFactor) heightChanged = true;
            } else {
                removed = true;
                if (!node->pRight) {
                    *root = node->pLeft;
                    m_Memory.deleteObject(node);
                    return true;
                }
                heightChanged = GetMin(&node->pRight, root);
                (*root)->balanceFactor = node->balanceFactor;
                (*root)->pLeft = node->pLeft;
                (*root)->pRight = node->pRight;
                m_Memory.deleteObject(node);
                if (heightChanged) heightChanged = !--(*root)->balanceFactor;
            }
            return Balance(root, true, heightChanged);
        }

        leaf *m_pRoot;
        CMemoryManager<leaf> m_Memory;
    };
}; // namespace templates

#endif // AVL_HEAD_H_2023_03_30
