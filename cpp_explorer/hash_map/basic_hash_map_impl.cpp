#include "basic_hash_map_impl.h"

// NOTE
template <typename K, typename V> void HashMap<K, V>::rehash() {
    size_t new_capacity = _buckets.size() * 2;
    std::vector<std::list<Node*>> new_buckets(new_capacity);
    for (auto& item : _buckets) {
        for (auto& node : item) {
            size_t index = hash(node->key) % new_capacity;
            new_buckets[index].push_front(node);
        }
    }

    _buckets = std::move(new_buckets);
}

template <typename K, typename V>
int32_t HashMap<K, V>::get(const K& key, V& value) {
    size_t index = hash(key) % _buckets.size();
    for (auto node : _buckets[index]) {
        if (node->key == key) {
            value = node->value;
            return 1; // 返回对应的值
        }
    }
    return -1; // 如果没有找到, 返回-1或其他错误值
}

template <typename K, typename V>
int32_t HashMap<K, V>::insert(const K& key, const V& value) {
    if (double(_size) >= _buckets.size() * LOAD_FACTOR) {
        rehash();
        std::cout << "Expand" << std::endl;
    }

    size_t index = hash(key) % _buckets.size();
    for (auto node : _buckets[index]) {
        if (node->key == key) {
            node->value = value; // 更新值
            _size++;
            return 0; // 成功插入或更新
        }
    }

    _buckets[index].push_front(new Node(key, value));
    _size++;
    return 1;
}

// 示例用法
int main() {
    HashMap<std::string, int> map;
    int32_t ret = 0;

    ret = map.insert("one", 1);
    ret = map.insert("two", 2);
    ret = map.insert("three", 3);

    int value;
    if (map.get("two", value)) {
        std::cout << "two: " << value << std::endl;
    }

    // map.remove("two");
    // if (!map.contains("two")) {
    //     std::cout << "two removed" << std::endl;
    // }

    return 0;
}
