#include <string>
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;
using namespace chrono;

using write_sequence = vector<string>;

using test_pair = pair<uint64_t, string>;
using modify_sequence = vector<test_pair>;
using read_sequence = vector<test_pair>;

ifstream& operator >> (ifstream& _is, test_pair& _value)
{
    _is >> _value.first;
    _is >> _value.second;

    return _is;
}

template <typename S>
S get_sequence(const string& _file_name)
{
    ifstream infile(_file_name);
    S result;

    typename S::value_type item;

    while (infile >> item)
    {
        result.emplace_back(move(item));
    }

    return result;
}

struct Node
{
    std::string value;
    Node* left;
    Node* right;
    int size;

    Node(const std::string& val)
        : value(val), left(nullptr), right(nullptr), size(1) {}
};

class storage
{
public:
    storage()
        : root(nullptr) {}

    void insert(const std::string& val)
    {
        root = insertNode(root, val);
    }

    void erase(uint64_t index)
    {
        root = eraseNode(root, index);
    }

    const std::string& get(uint64_t index)
    {
        Node* current = root;

        while (current)
        {
            int leftSize = current->left ? current->left->size : 0;
            if (index == leftSize)
            {
                return current->value;
            }
            else if (index < leftSize)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
                index -= (leftSize + 1);
            }
        }

        return "";
    }

private:
    Node* insertNode(Node* node, const std::string& val)
    {
        if (!node)
        {
            return new Node(val);
        }
        else if (val < node->value)
        {
            node->left = insertNode(node->left, val);
        }
        else
        {
            node->right = insertNode(node->right, val);
        }

        node->size = 1 + getSize(node->left) + getSize(node->right);

        return node;
    }

    Node* eraseNode(Node* node, uint64_t index)
    {
        if (!node)
        {
            return nullptr;
        }

        int leftSize = node->left ? node->left->size : 0;

        if (index < leftSize)
        {
            node->left = eraseNode(node->left, index);
        }
        else if (index > leftSize)
        {
            node->right = eraseNode(node->right, index - (leftSize + 1));
        }
        else
        {
            if (!node->left)
            {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* successor = minNode(node->right);
            node->value = successor->value;
            node->right = eraseNode(node->right, 0);
        }

        node->size = 1 + getSize(node->left) + getSize(node->right);

        return node;
    }

    Node* minNode(Node* node)
    {
        Node* current = node;

        while (current->left)
        {
            current = current->left;
        }

        return current;
    }

    int getSize(Node* node)
    {
        return node ? node->size : 0;
    }

private:
    Node* root;
};

int main()
{
    write_sequence write = get_sequence<write_sequence>("files/write.txt");
    modify_sequence modify = get_sequence<modify_sequence>("files/modify.txt");
    read_sequence read = get_sequence<read_sequence>("files/read.txt");

    storage st;

    for (const string& item : write)
    {
        st.insert(item);
    }

    uint64_t progress = 0;
    uint64_t percent = modify.size() / 100;

    time_point<system_clock> time;
    nanoseconds total_time(0);

    modify_sequence::const_iterator mitr = modify.begin();
    read_sequence::const_iterator ritr = read.begin();

    for (; mitr != modify.end() && ritr != read.end(); ++mitr, ++ritr)
    {
        time = system_clock::now();
        st.erase(mitr->first);
        st.insert(mitr->second);
        const string& str = st.get(ritr->first);
        total_time += system_clock::now() - time;

        if (ritr->second != str)
        {
            cout << "test failed" << endl;
            return 1;
        }

        if (++progress % (5 * percent) == 0)
        {
            cout << "time: " << duration_cast<milliseconds>(total_time).count()
                << "ms progress: " << progress << " / " << modify.size() << "\n";
        }
    }

    return 0;
}
