#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

template <typename T, bool is_const>
struct const_hanger {
    using type = T;
};

template <typename T>
struct const_hanger<T, true> {
    using type = const T;
};

template <typename T>
class DequeNode {
private:
    size_t size;
    T* dequeNode;
    vector<bool> bool_exist;

public:
    DequeNode(size_t s)
        : size(s)
    {
        bool_exist.resize(s, 0);
        dequeNode = reinterpret_cast<T*>(new int8_t[sizeof(T) * s]);
    }
    ~DequeNode()
    {
        for (size_t i = 0; i < size; ++i) {
            if (bool_exist[i]) {
                (dequeNode + i)->~T();
            }
        }
        delete[] reinterpret_cast<int8_t*>(dequeNode);
    }
    void set(size_t p,
        const T& v)
    {

        if (bool_exist[p]) {
            dequeNode[p] = v;
        }
        else {
            bool_exist[p] = 1;
            new (dequeNode + p) T(v);
        }
    }
    void print()
    {
        for (size_t i = 0; i < size; i++) {
            if (bool_exist[i] == 1)
                cout << dequeNode[i].x << " ";
            else
                cout << 0 << " ";
        }
        cout << endl;
    }
    void erase(size_t p)
    {
        if (bool_exist[p]) {
            bool_exist[p] = 0;
            (dequeNode + p)->~T();
        }
    }

    T& get(size_t pos)
    {
        return dequeNode[pos];
    }

    const T& get(size_t pos) const
    {
        return dequeNode[pos];
    }

    void check_existance(size_t pos)
    {
        if (pos >= bool_exist.size() || !bool_exist[pos]) {
            throw std::out_of_range("Error");
        }
    }
    T& at(size_t pos)
    {
        check_existance(pos);
        return dequeNode[pos];
    }

    const T& at(size_t pos) const
    {
        check_existance(pos);
        return dequeNode[pos];
    }
};

template <typename T>
class Deque {
private:
    vector<DequeNode<T>*> deque;
    size_t row_cnt = 3;
    size_t col_cnt = 10;

    pair<size_t, size_t> start = {1,0};
    pair<size_t, size_t> ending = {1,0};

    pair<size_t, size_t> adjust(pair<size_t, size_t> p, pair<size_t, size_t> shift) const
    {

        long long sec = p.second + shift.second;
        long long p1 = p.first;
        long long s1 = shift.first;

        if (sec >= 0) {

            p.first = p1 + s1 + (sec / (long long)col_cnt);
            p.second = (sec) % (long long)col_cnt;
        }
        else {
            p.first = p1 + s1 + ((-(long long)col_cnt + sec) / (long long)col_cnt);
            p.second = (sec % (long long)col_cnt) + (long long)col_cnt;
        }

        return p;
    }

    pair<size_t, size_t> realcords(pair<size_t, size_t> p) const
    {
        return adjust(p, start);
    }
    void size_fix()
    {
        size_t s1 = std::max((long long)row_cnt / 2 - (long long)start.first, 1ll);
        size_t e1 = std::max((long long)row_cnt / 2 - ((long long)row_cnt - (long long)ending.first), 1ll);
        for (size_t i = 0; i < s1; i++) {
            deque.insert(deque.begin(), new DequeNode<T>(col_cnt));
        }

        for (size_t i = 0; i < e1; i++) {
            deque.push_back(new DequeNode<T>(col_cnt));
        }

        start = adjust(start, { s1,
                                  0 });
        ending = adjust(ending, { s1,
                                    0 });
        row_cnt += (s1 + e1);
    }

    template <bool norm_const, bool norm_rev>
    class iterator_prototype {
    private:
        pair<size_t, size_t> current;
        const Deque* master = nullptr;
        using type = typename const_hanger<T, norm_const>::type;
        size_t col_cnt = 10;
        //CAUTION HERE WE WILL MAKE Deque friend.
        friend Deque;
    public:
        iterator_prototype() = default;
        iterator_prototype(pair<size_t, size_t> el,
            const Deque* deq)
            : current(el)
            , master(deq)
        {
        }

        iterator_prototype(const iterator_prototype& other) = default;

        ~iterator_prototype() = default;

        template <bool is_const_other, bool is_reversed_other>
        explicit operator iterator_prototype<is_const_other, is_reversed_other>() const
        {

            return iterator_prototype<is_const_other, is_reversed_other>(this->current, this->master);
        }

        iterator_prototype& operator=(const iterator_prototype& other) = default;

        iterator_prototype& operator+=(size_t sft)
        {
            if (norm_rev) {

                current = master->adjust(current, make_pair(0, -sft));
            }
            else {

                current = master->adjust(current, make_pair(0, sft));
            }
            return *this;
        }
        iterator_prototype operator+(int sft) const
        {
            iterator_prototype cp(*this);

            cp += sft;

            return cp;
        }
        iterator_prototype& operator-=(int sft)
        {
            return * this += -sft;
        }

        iterator_prototype operator-(int sft) const
        {
            iterator_prototype cp(*this);
            cp -= sft;
            return cp;
        }
        int operator-(const iterator_prototype other) const
        {
            int res = (current.first - other.current.first) * col_cnt + current.second - other.current.second;
            if (norm_rev)
                res *= (-1);
            return res;
        }

        iterator_prototype& operator--()
        {
            return * this -= 1;
        }

        iterator_prototype operator--(int)
        {
            iterator_prototype copy(*this);
            --*this;
            return copy;
        }

        iterator_prototype& operator++()
        {
            return * this += 1;
        }

        iterator_prototype operator++(int)
        {
            iterator_prototype copy(*this);
            ++*this;
            return copy;
        }

        type& operator*() const
        {
            return master->deque[current.first]->get(current.second);
        }

        type* operator->()
        {
            return &master->deque[current.first]->get(current.second);
        }

        bool operator<(const iterator_prototype& other) const
        {
            bool res = (*this - other) < 0;
            if (norm_rev)
                res = !res;
            return res;
        }

        bool operator<=(const iterator_prototype& other) const
        {
            return !(other < *this);
        }

        bool operator>=(const iterator_prototype& other) const
        {
            return !(*this < other);
        }

        bool operator>(const iterator_prototype& other) const
        {
            return other < *this;
        }

        bool operator==(const iterator_prototype& other) const
        {
            return other <= *this && *this <= other;
        }

        bool operator!=(const iterator_prototype& other) const
        {
            return !(*this == other);
        }
    };

public:
    using iterator = iterator_prototype<0, 0>;
    using reverse_iterator = iterator_prototype<0, 1>;

    Deque()
    {

        for (size_t i = 0; i < row_cnt; ++i) {
            deque.push_back(new DequeNode<T>(col_cnt));
        }
    }

    ~Deque()
    {

        for (size_t i = 0; i < row_cnt; ++i) {
            delete deque[i];
        }
    }

    Deque(size_t a,
        const T& b)
        : Deque()
    {
        while (a--) {
            push_back(b);
        }
    }
    Deque(int a)
        : Deque(1, a)
    {
    }

    void push_back(const T& item)
    {

        if (ending == make_pair(row_cnt - 1, col_cnt - 1)) {
            size_fix();
        }
        deque[ending.first]->set(ending.second, item);
        ending = adjust(ending, { 0,
                                    1 });
    }

    void push_front(const T& item)
    {
        if (start == make_pair(0lu, 0lu))
            size_fix();
        start = adjust(start, { 0,
                                  -1 });
        deque[start.first]->set(start.second, item);
    }
    void pop_front()
    {
        deque[start.first]->erase(start.second);
        start = adjust(start, { 0,
                                  1 });
    }
    void pop_back()
    {
        auto real_end = adjust(ending, { 0,
                                           -1 });
        deque[real_end.first]->erase(real_end.second);
        ending = real_end;
    }

    template <typename Iterator>
    void insert(Iterator position,
        const T& value)
    {
        Deque<T> cp = *this;
        cp.deque[cp.end().current.first]->set(cp.end().current.second, value);
        for (iterator iter(cp.end().current, &cp); iter > position; --iter) {
            *iter = *(iter - 1);
        }

        iterator iter(position.current, &cp);
        *iter = value;

        cp.ending = cp.adjust(cp.ending, { 0,
                                             1 });
        *this = cp;
    }
    template <typename Iterator>
    void erase(Iterator position)
    {
        Deque<T> cp = *this;
        for (iterator iter(position.current, &cp); iter < cp.end() - 1; ++iter) {
            *iter = *(iter + 1);
        }
        cp.ending = cp.adjust(cp.ending, { 0,
                                             -1 });
        cp.deque[cp.ending.first]->erase(cp.ending.second);
        *this = cp;
    }

    void print() const
    {
        for (size_t i = 0; i < row_cnt; i++) {

            deque[i]->print();
        }
        cout << endl;
    }
    size_t size() const
    {
        return (ending.first - start.first) * col_cnt + (ending.second - start.second);
    }

    T& operator[](size_t i)
    {
        auto x = realcords({ 0,
            i });

        return deque[x.first]->get(x.second);
    }

    const T& operator[](size_t i) const
    {
        auto x = realcords({ 0,
            i });
        return deque[x.first]->get(x.second);
    }

    T& at(size_t i)
    {
        auto x = realcords({ 0,
            i });
        if (x.first >= row_cnt)
            throw std::out_of_range("Error");

        return deque[x.first]->at(x.second);
    }

    const T& at(size_t i) const
    {
        auto x = realcords({ 0,
            i });
        if (x.first >= row_cnt)
            throw std::out_of_range("Error");

        return deque[x.first]->at(x.second);
    }

    Deque(const Deque<T>& other)
        : Deque()
    {

        for (size_t i = 0; i < other.size(); i++) {
            this->push_back(other[i]);
        }
    }

    Deque<T>& operator=(const Deque<T>& other)
    {
        while (size() > 0u) {
            pop_back();
        }
        for (size_t i = 0; i < other.size(); i++) {
            this->push_back(other[i]);
        }
        return *this;
    }

    iterator_prototype<0, 0> begin()
    {
        return iterator_prototype<0, 0>(start, this);
    }

    iterator_prototype<0, 0> end()
    {
        return iterator_prototype<0, 0>(ending, this);
    }

    iterator_prototype<1, 0> cbegin() const
    {
        return iterator_prototype<1, 0>(start, this);
    }

    iterator_prototype<1, 0> cend() const
    {
        return iterator_prototype<1, 0>(ending, this);
    }

    iterator_prototype<1, 0> begin() const
    {
        return static_cast<iterator_prototype<1, 0> >(iterator_prototype<0, 0>(start, this));
    }

    iterator_prototype<1, 0> end() const
    {
        return static_cast<iterator_prototype<1, 0> >(iterator_prototype<0, 0>(ending, this));
    }

    iterator_prototype<0, 1> rbegin()
    {
        return iterator_prototype<0, 1>(adjust(ending, { 0,
                                                  -1 }),
            this);
    }

    iterator_prototype<0, 1> rend()
    {
        return iterator_prototype<0, 1>(adjust(start, { 0,
                                                 -1 }),
            this);
    }

    iterator_prototype<1, 1> rbegin() const
    {
        return static_cast<iterator_prototype<1, 1> >(iterator_prototype<0, 1>(adjust(ending, { 0,
                                                                                -1 }),
            this));
    }

    iterator_prototype<1, 1> rend() const
    {
        return static_cast<iterator_prototype<1, 1> >(iterator_prototype<0, 1>(adjust(start, { 0,
                                                                               -1 }),
            this));
    }

    iterator_prototype<1, 1> crbegin()
    {
        return iterator_prototype<1, 1>(adjust(ending, { 0,
                                                  -1 }),
            this);
    }

    iterator_prototype<1, 1> crend()
    {
        return iterator_prototype<1, 1>(adjust(start, { 0,
                                                 -1 }),
            this);
    }

};
