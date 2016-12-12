#include <iostream> 
#include <istream> 
#include <fstream> 
#include <vector>
#include <string>

struct smart_array {
public:
    smart_array(std::vector<int> &values, const int max_element) {
        this->values = values;
        values_count = values.size();
        this->max_element = max_element;
        fill_roots(build(0, values_count - 1));
    }

    int get_right(int left_index, int differece, int last_answer) {
        diff = differece;
        int answer = get(roots[left_index], 0, values_count - 1) + 1;
        if (diff > 0) {
            answer = 0;
        }
        return answer;
    }

private:
    struct node {
        node *left;
        node *right;
        int value;
    };

    std::vector<int> values;
    std::vector<node*> roots;

    int values_count;
    int max_element;
    int diff;

    int get(node* root, const int current_left, const int current_right) {
        if (diff == 0) {
            return current_left - 1;
        }
        if (root->value < diff || current_left == current_right) {
            diff -= root->value;
            return current_right;
        }
        const int mid = (current_left + current_right) / 2;
        int answer = get(root->left, current_left, mid);
        if (diff > 0) {
            answer = get(root->right, mid + 1, current_right);
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

    void fill_roots(node *root) {
        std::vector<int> next(values_count);
        std::vector<int> last(max_element, -1);
        roots.clear();
        roots.resize(values_count);
        for (int i = values_count - 1; i >= 0; --i) {
            next[i] = last[values[i]];
            last[values[i]] = i;

            root = set(root, 0, values_count - 1, i, 1);
            if (next[i] >= 0) {
                root = set(root, 0, values_count - 1, next[i], 0);
            }
            roots[i] = root;
        }
    }
};

std::vector<int> read_values(std::istream &_Istr, const int values_count) {
    std::vector<int> values(values_count);
    for (int i = 0; i < values_count; ++i) {
        _Istr >> values[i];
        --values[i];
    }
    return values;
}

int read_int(std::istream &_Istr) {
    int value;
    _Istr >> value;
    return value;
}

void print_value(std::ostream &_Ostr, const int value) {
    _Ostr << value << std::endl;
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    const int values_count = read_int(std::cin);
    const int max_value = read_int(std::cin);
    std::vector<int> values = read_values(std::cin, values_count);

    smart_array arr = smart_array(values, max_value);

    int last_answer = 0;

    const int queries_count = read_int(std::cin);

    for (int i = 0; i < queries_count; ++i) {
        int query_x = read_int(std::cin);
        int query_y = read_int(std::cin);

        const int left = ((query_x + last_answer) % values_count);
        const int difference = ((query_y + last_answer) % max_value) + 1;

        last_answer = arr.get_right(left, difference, last_answer);
        print_value(std::cout, last_answer);
    }

    return 0;
}
