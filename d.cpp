#include <iostream> 
#include <istream> 
#include <fstream> 
#include <vector>
#include <string>


// Stream manager structure.
// Use for reading/writing different data from/to the stream.
// It makes processing input and output easier.
struct stream_manager {
public:
    // function read_int - procedure for reading an integer from the stream.
    // parameter std::istream &_Istr - address of any input stream.
    // parameter int &data - address of the integer, where should be stored input data.
    static void read_int(std::istream &_Istr, int &data) {
        _Istr >> data;
    }

    // function read_vector - procedure for reading an vector of the size from the stream.
    // Before reading the data, vector is going to be cleaned.
    // So do not forget, that all the data stored in this vector will be lost.
    // parameter std::istream &_Istr - address of any input stream.
    // parameter std::vector<int> &vector - vector, where should be stored input data.
    // parameter const int size - number of times to read integers from the stream.
    // Also it is the new size of the vector.
    static void read_vector(
        std::istream &_Istr,
        std::vector<int> &vector,
        const int size) {
        vector.clear();
        vector.resize(size);
        for (int i = 0; i < size; ++i) {
            _Istr >> vector[i];
        }
    }

    // function write_int - procedure for writing an integer to the stream.
    // parameter std::ostream &_Istr - address of any output stream.
    // parameter const int data - integer, the value of which should be written to the stream.
    static void write_int(std::ostream &_Ostr, const int data) {
        _Ostr << data << std::endl;
    }
};

struct smart_array {
public:
    smart_array(std::vector<int> &values, const int max_element) {
        this->values = values;
        values_count = values.size();
        this->max_element = max_element;
        fill_roots(build(0, values_count - 1));
    }

    int get_right(
        const int left_index, 
        const int differece) {
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

void read_data(
    int &values_count,
    int &max_value,
    std::vector<int> &values,
    int &queries_count) {
    stream_manager::read_int(std::cin, values_count);
    stream_manager::read_int(std::cin, max_value);
    stream_manager::read_vector(std::cin, values, values_count);
    stream_manager::read_int(std::cin, queries_count);
}

void read_query(
    int &query_x,
    int &query_y) {
    stream_manager::read_int(std::cin, query_x);
    stream_manager::read_int(std::cin, query_y);
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int values_count, max_value, queries_count;
    std::vector<int> values;
    read_data(values_count, max_value, values, queries_count);
    
    for (int i = 0; i < values_count; ++i) {
        --values[i];
    }

    smart_array arr = smart_array(values, max_value);

    int last_answer = 0;

    for (int i = 0; i < queries_count; ++i) {
        int query_x, query_y;
        read_query(query_x, query_y);

        const int left = ((query_x + last_answer) % values_count);
        const int difference = ((query_y + last_answer) % max_value) + 1;

        last_answer = arr.get_right(left, difference);
        stream_manager::write_int(std::cout, last_answer);
    }

    return 0;
}
