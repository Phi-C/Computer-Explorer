#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

class TopKSolver {
  public:
    // 获取前 k 个出现频率最高的元素
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // 使用哈希表统计频率
        unordered_map<int, int> freq_map;
        for (int num : nums) {
            freq_map[num]++;
        }

        auto cmp = [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second > b.second; // 小根堆，频率小的在前面
        };
        priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)>
            min_heap(cmp);

        // 将频率放入小根堆
        for (const auto& [num, freq] : freq_map) {
            if (min_heap.size() < k) {
                min_heap.push({num, freq});
            } else if (freq > min_heap.top().second) {
                min_heap.pop();
                min_heap.push({num, freq});
            }
        }

        // 提取前 k 个频率最高的元素
        vector<int> result;
        while (!min_heap.empty()) {
            result.push_back(min_heap.top().first);
            min_heap.pop();
        }

        return result;
    }

    vector<int> smallestK(vector<int>& arr, int k) {
        vector<int> result;
        if (k == 0)
            return result;

        auto cmp = [](const int& a, const int& b) {
            return a < b; // 大根堆，最大的在top
        };
        priority_queue<int, vector<int>, decltype(cmp)> max_heap(cmp);

        for (int num : arr) {
            if (max_heap.size() < k) {
                max_heap.push(num);
            } else if (num < max_heap.top()) {
                max_heap.pop();
                max_heap.push(num);
            }
        }

        while (!max_heap.empty()) {
            result.push_back(max_heap.top());
            max_heap.pop();
        }

        return result;
    }
};

// 辅助函数：打印vector
void printVector(const vector<int>& vec, const string& name) {
    cout << name << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
        cout << vec[i];
        if (i != vec.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

// 测试代码
int main() {
    TopKSolver solver;

    // 测试topKFrequent
    {
        vector<int> nums1 = {1, 1, 1, 2, 2, 3};
        int k1 = 2;
        auto result1 = solver.topKFrequent(nums1, k1);
        printVector(result1, "topKFrequent test1");

        vector<int> nums2 = {4, 1, -1, 2, -1, 2, 3};
        int k2 = 2;
        auto result2 = solver.topKFrequent(nums2, k2);
        printVector(result2, "topKFrequent test2");

        vector<int> nums3 = {1, 2, 3, 4, 5};
        int k3 = 3;
        auto result3 = solver.topKFrequent(nums3, k3);
        printVector(result3, "topKFrequent test3");
    }

    // 测试smallestK
    {
        vector<int> arr1 = {4, 5, 1, 6, 2, 7, 3, 8};
        int k1 = 4;
        auto result1 = solver.smallestK(arr1, k1);
        printVector(result1, "smallestK test1");

        vector<int> arr2 = {1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
        int k2 = 5;
        auto result2 = solver.smallestK(arr2, k2);
        printVector(result2, "smallestK test2");

        vector<int> arr3 = {10, 20, 30, 40, 50};
        int k3 = 2;
        auto result3 = solver.smallestK(arr3, k3);
        printVector(result3, "smallestK test3");
    }

    return 0;
}