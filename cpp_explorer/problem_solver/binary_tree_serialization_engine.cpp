/*
 * Usage: g++ -std=c++11 binary_tree_serialization_engine.cpp -o
 * binary_tree_serialization_engine
 * Description:
 * https://leetcode.cn/problems/serialize-and-deserialize-binary-tree/description/
 */
#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

using namespace std;

struct BinaryTreeNode {
    int val;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
    BinaryTreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class BinaryTreeCodecEngine {
  public:
    // 序列化（层序遍历）
    string serialize(BinaryTreeNode* root) {
        if (!root)
            return "";
        queue<BinaryTreeNode*> q;
        q.push(root);
        stringstream ss;

        while (!q.empty()) {
            BinaryTreeNode* node = q.front();
            q.pop();
            if (node) {
                ss << node->val << ",";
                q.push(node->left);
                q.push(node->right);
            } else {
                ss << "#,";
            }
        }

        string s = ss.str();
        return s.substr(0, s.size() - 1); // 去掉最后的逗号
    }

    // 反序列化（层序遍历重建）
    BinaryTreeNode* deserialize(string data) {
        if (data.empty())
            return nullptr;
        vector<string> nodes;
        stringstream ss(data);
        string token;

        while (getline(ss, token, ',')) {
            nodes.push_back(token);
        }

        BinaryTreeNode* root = new BinaryTreeNode(stoi(nodes[0]));
        queue<BinaryTreeNode*> q;
        q.push(root);
        int i = 1;

        while (!q.empty() && i < nodes.size()) {
            BinaryTreeNode* node = q.front();
            q.pop();

            // 处理左子节点
            if (i < nodes.size() && nodes[i] != "#") {
                node->left = new BinaryTreeNode(stoi(nodes[i]));
                q.push(node->left);
            }
            i++;

            // 处理右子节点
            if (i < nodes.size() && nodes[i] != "#") {
                node->right = new BinaryTreeNode(stoi(nodes[i]));
                q.push(node->right);
            }
            i++;
        }

        return root;
    }
};

// 测试代码
int main() {
    BinaryTreeNode* root = new BinaryTreeNode(1);
    root->left = new BinaryTreeNode(2);
    root->right = new BinaryTreeNode(3);
    root->right->left = new BinaryTreeNode(4);
    root->right->right = new BinaryTreeNode(5);

    BinaryTreeCodecEngine codec;
    string serialized = codec.serialize(root);
    cout << "Serialized: " << serialized << endl; // 输出: 1,2,3,#,#,4,5,#,#,#,#

    BinaryTreeNode* deserialized = codec.deserialize(serialized);
    string reserialized = codec.serialize(deserialized);
    cout << "Reserialized: " << reserialized << endl; // 输出应与之前一致

    return 0;
}