## 哈希碰撞
LLM在实现prefix_cache时, 需要计算block得hash值。考虑大量数据场景下, 可能会发生哈希碰撞: 假设哈希表有100个桶, 但是要存储1000个元素，平均每个桶会有10个元素，这些元素就是发生了碰撞。发生碰撞后，需要遍历链表/红黑树，哈希表操作由O(1)退化成O(n)或O(logn)。

一些减少哈希碰撞的方法:
1. 选择高质量的哈希函数
* 加密哈希函数：使用SHA-256、SHA-3等加密级哈希算法
* 非加密但高质量的哈希函数：如MurmurHash、CityHash、xxHash等
* 避免简单哈希：不要使用简单的取模运算作为哈希函数

2. 增加哈希空间
* 使用更长的哈希值(如从32位升级到64位或128位), 这可以显著降低碰撞概率(根据生日问题原理)

3. 优化数据结构设计
* 开放寻址法：线性探测、二次探测等
* 链地址法：每个桶使用链表存储冲突元素
* 完美哈希：对于静态数据集，可以构造无碰撞的完美哈希函数

4. 负载因子管理
* 当哈希表填充到一定比例(如70%)时进行扩容
* 保持合理的负载因子(通常0.7-0.8)

5. 双重哈希
* 当发生碰撞时，使用第二个哈希函数计算新的位置

## 平滑HashMap
基本实现`HashMap`在进行扩容时, 会产生性能毛刺。通过将扩容过程均摊, 可以实现更平滑的HashMap.


## Ref IMPL
```C++
template <typename K, typename V> class HashMap {
  private:
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr double LOAD_FACTOR = 0.75;

    struct Entry {
        K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    std::vector<std::list<Entry>> buckets;
    // size表示当前哈希表中存储的键值对数量
    // 当size超过buckets.size() * LOAD_FACTOR时, 需要进行rehash
    size_t size;

    size_t hash(const K& key) const { return std::hash<K>{}(key); }

    void rehash() {
        size_t new_capacity = buckets.size() * 2;
        std::vector<std::list<Entry>> new_buckets(new_capacity);

        for (auto& bucket : buckets) {
            for (auto& entry : bucket) {
                // 对key求哈希函数, 然后对buckets的大小取模, 确定索引位置
                size_t index = hash(entry.key) % new_capacity;
                new_buckets[index].push_back(entry);
            }
        }

        buckets = std::move(new_buckets);
    }

  public:
    HashMap() : buckets(DEFAULT_CAPACITY), size(0) {}

    void put(const K& key, const V& value) {
        // 这种实现会导致毛刺显现: 进行rehash会花费更长时间
        if ((double)size > buckets.size() * LOAD_FACTOR) {
            rehash();
        }

        size_t index = hash(key) % buckets.size();

        // 如果键已存在, 更新其值
        for (auto& entry : buckets[index]) {
            if (entry.key == key) {
                entry.value = value;
                return;
            }
        }

        // 添加新条目
        buckets[index].emplace_back(key, value);
        size++;
    }

    bool get(const K& key, V& value) const {
        size_t index = hash(key) % buckets.size();

        for (const auto& entry : buckets[index]) {
            if (entry.key == key) {
                value = entry.value;
                return true;
            }
        }

        return false;
    }

    bool remove(const K& key) {
        size_t index = hash(key) % buckets.size();

        auto& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                size--;
                return true;
            }
        }

        return false;
    }

    bool contains(const K& key) const {
        size_t index = hash(key) % buckets.size();

        for (const auto& entry : buckets[index]) {
            if (entry.key == key) {
                return true;
            }
        }

        return false;
    }

    size_t getSize() const { return size; }

    bool isEmpty() const { return size == 0; }
};
```
