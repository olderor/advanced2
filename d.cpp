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

// Persistent Segment Tree structure.
// Use for solving the given problem.
struct persistent_segment_tree {
public:
    // initialize array with given data.
    persistent_segment_tree(std::vector<int> &values, const int max_element) {
        this->values = values;
        values_count = values.size();
        this->max_element = max_element;
        fill_roots(build(0, values_count - 1));
    }

    // function get_right - function for finding the minimum value of right index of the array,
    // such that there is no less different elements in array than given 
    // between given left index and right index.
    // parameter const int left_index - left index of array.
    // parameter const int differece - number of different elements in array.
    // return int - right index of the array.
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
    // Node structure.
    // Use for storing different versions of the tree.
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

    // function get - function for finding the minimum value of right index of the array,
    // such that there is no less different elements in array than given 
    // between given left index and right index.
    // parameter const int current_left - left index of array.
    // parameter const int current_right - right index of array (maximum value).
    // return int - right index of the array.
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

    // function set - function for updating values.
    // parameter node *root - pointer to the current node, where should be updating.
    // parameter const int left - left index of array.
    // parameter const int right - right index of array.
    // parameter const int index - index in the array where the value should be updated.
    // parameter const int value - new value.
    // return node* - new version of the tree.
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

    // function build - function for creating a tree.
    // parameter const int left - left index of array.
    // parameter const int right - right index of array.
    // return node* - new version of the tree.
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

    // function fill_roots - function for creating a segment tree
    // with given values in the array.
    // parameter node *root - pointer to the current root of the tree.
    void fill_roots(node *root) {
        std::vector<int> last(max_element, -1);
        roots.resize(values_count);
        for (int i = values_count - 1; i >= 0; --i) {
            const int previous_index = last[values[i]];
            last[values[i]] = i;

            root = set(root, 0, values_count - 1, i, 1);
            if (previous_index >= 0) {
                root = set(root, 0, values_count - 1, previous_index, 0);
            }
            roots[i] = root;
        }
    }
};


// function read_data - procedure for reading input data from the stream.
// parameter std::istream &_Istr - address of any input stream.
// parameter int &values_count - integer, where should be stored the size of the array.
// parameter int &max_value - integer, where should be stored the maximum value in the array.
// parameter std::vector<int> &values - vector, where should be stored elements of the array.
// parameter int &queries_count - integer, where should be stored the number of given queries.
void read_data(
    std::istream &_Istr,
    int &values_count,
    int &max_value,
    std::vector<int> &values,
    int &queries_count) {
    stream_manager::read_int(_Istr, values_count);
    stream_manager::read_int(_Istr, max_value);
    stream_manager::read_vector(_Istr, values, values_count);
    stream_manager::read_int(_Istr, queries_count);
}

// function read_data - procedure for reading input data from the stream.
// parameter std::istream &_Istr - address of any input stream.
// parameters int &query_x, int &query_y - addresses of the integeres, 
// where should be stored the information about queries.
void read_query(
    std::istream &_Istr,
    int &query_x,
    int &query_y) {
    stream_manager::read_int(_Istr, query_x);
    stream_manager::read_int(_Istr, query_y);
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int values_count, max_value, queries_count;
    std::vector<int> values;
    read_data(std::cin, values_count, max_value, values, queries_count);

    for (int i = 0; i < values_count; ++i) {
        --values[i];
    }

    persistent_segment_tree arr = persistent_segment_tree(values, max_value);

    int last_answer = 0;

    for (int i = 0; i < queries_count; ++i) {
        int query_x, query_y;
        read_query(std::cin, query_x, query_y);

        const int left = ((query_x + last_answer) % values_count);
        const int difference = ((query_y + last_answer) % max_value) + 1;

        last_answer = arr.get_right(left, difference);
        stream_manager::write_int(std::cout, last_answer);
    }

    return 0;
}
