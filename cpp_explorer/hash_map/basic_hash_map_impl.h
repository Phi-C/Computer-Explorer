#include <iostream>
#include <list>
#include <string>
#include <vector>

template <typename K, typename V> class HashMap {
  private:
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr double LOAD_FACTOR = 0.75;
    struct Node {
        K key;
        V value;
        Node* next;
        Node(const K& k, const V& v) : key{k}, value{v}, next{nullptr} {}
    };

    size_t hash(const K& key) const noexcept { return std::hash<K>{}(key); }

    std::vector<std::list<Node*>> _buckets;
    size_t _size;

  public:
    HashMap() : _buckets(DEFAULT_CAPACITY), _size(0) {}

    [[nodiscard]] int32_t get(const K& key, V& value);

    [[nodiscard]] int32_t insert(const K& key, const V& value);

    void rehash();
};