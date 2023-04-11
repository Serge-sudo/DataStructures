#include <map>
#include <optional>


//Task Description
//interval_map<K,V> is a data structure that associates keys of type K with values of type V.
//It is designed to be used efficiently in situations where intervals of consecutive keys
//are associated with the same value. Your task is to implement the assign member
//function of this data structure, which is outlined below.
//
//interval_map<K, V> is implemented on top of std::map. For more information
//on std::map, you may refer to cppreference.com.
//
//Each key-value-pair (k,v) in interval_map<K,V>::m_map means that the value
//v is associated with all keys from k (including) to the next key (excluding)
//in m_map. The member interval_map<K,V>::m_valBegin holds the value that is
//associated with all keys less than the first key in m_map.
//Do not make more than one operation of amortized O(log N), in contrast to O(1),
//running time, where N is the number of elements in m_map.



template<typename K, typename V>
class interval_map {
    V m_valBegin;
    std::map<K, V> m_map;
 public:
    // constructor associates whole range of K with val
    interval_map(V const& val)
        : m_valBegin(val) {}

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign(K const& keyBegin, K const& keyEnd, V const& val) {
        if (!(keyBegin < keyEnd)) {
            return;
        }

        auto main_it = m_map.upper_bound(keyBegin);
        auto prev_val = (main_it == m_map.begin()) ? m_valBegin : std::prev(main_it)->second;

        if (!(prev_val == val)) {

            auto prev_it = main_it == m_map.begin() ? main_it : std::prev(main_it);
            auto it = m_map.insert_or_assign(prev_it, keyBegin, val);
            if (prev_it == it) {
                if (it != m_map.begin()) {
                    if ((--it)->second == val) {
                        m_map.erase(++it);
                    }
                } else {
                    if (val == m_valBegin) {
                        m_map.erase(it);
                    }
                }

            }
        }

        auto it = main_it;
        while (it != m_map.end() && !(keyEnd < it->first)) {
            it++;
        }
        if (it != main_it) {
            prev_val = std::prev(it)->second;
        }
        if (main_it != m_map.end() && main_it != it) {
            m_map.erase(main_it, it);
        }
        if (!(val == prev_val)) {
            auto prev_it = (it == m_map.begin()) ? m_map.begin() : std::prev(it);
            m_map.insert_or_assign(prev_it, keyEnd, prev_val);
        }

    }

    // look-up of the value associated with key
    V const& operator[](K const& key) const {
        auto it = m_map.upper_bound(key);
        if (it == m_map.begin()) {
            return m_valBegin;
        } else {
            return (--it)->second;
        }
    }

};
