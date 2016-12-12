#include <iostream> 
#include <fstream> 
#include <vector>
#include <string>


struct node {
    node *left;
    node *right;
    int value;
};

int query_k = 0;

node* build(const int left, const int right) {
    node *new_node = new node();
    if (left == right) {
        new_node->value = 0;
        return new_node;
    }
    const int mid = (left + right) / 2;
    new_node->left = build(left, mid);
    new_node->right = build(mid + 1, right);
    new_node->value = new_node->left->value + new_node->right->value;
    return new_node;
}

int get(node* root, const int cur_l, const int cur_r) {
    if (query_k == 0) {
        return cur_l - 1;
    }
    if (root->value < query_k || cur_l == cur_r) {
        query_k -= root->value;
        return cur_r;
    }
    const int mid = (cur_l + cur_r) / 2;
    int answer = get(root->left, cur_l, mid);
    if (query_k > 0) {
        answer = get(root->right, mid + 1, cur_r);
    }
    return answer;
}

node* set(
    node *root, 
    const int left, 
    const int right,
    const int index,
    const int value) {
    node *new_node = new node();
    if (left == right) {
        new_node->value = value;
        return new_node;
    }
    const int mid = (left + right) / 2;

    new_node->value = root->value;
    new_node->left = root->left;
    new_node->right = root->right;

    if (index <= mid) {
        new_node->left = set(root->left, left, mid, index, value);
    } else {
        new_node->right = set(root->right, mid + 1, right, index, value);
    }
    new_node->value = new_node->left->value + new_node->right->value;
    return new_node;
}


const int MAX = 100001;
int values[MAX], last[MAX], next[MAX];
node* roots[MAX];

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int values_count, max;
    std::cin >> values_count >> max;

    for (int i = 0; i < values_count; ++i) {
        std::cin >> values[i];
        --values[i];
    }

    int query_p = 0;

    int queries_count;
    std::cin >> queries_count;

    node *root = build(0, values_count - 1);
    for (int i = 0; i < values_count; ++i) {
        last[i] = -1;
    }

    for (int i = values_count - 1; i >= 0; --i) {
        next[i] = last[values[i]];
        last[values[i]] = i;

        root = set(root, 0, values_count - 1, i, 1);
        if (next[i] >= 0) {
            root = set(root, 0, values_count - 1, next[i], 0);
        }
        roots[i] = root;
    }

    for (int i = 0; i < queries_count; ++i) {
        int query_x, query_y;
        std::cin >> query_x >> query_y;
        const int query_l = ((query_x + query_p) % values_count);
        query_k = ((query_y + query_p) % max) + 1;
        query_p = get(roots[query_l], 0, values_count - 1) + 1;
        if (query_k > 0) {
            query_p = 0;
        }
        std::cout << query_p << std::endl;
    }

    return 0;
}
