#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>

const int BALANCED = 0, LEFT_HEAVY = -1, RIGHT_HEAVY = 1;

template <typename KeyType, typename ValueType>
class AVL {
private:
    struct node {
        KeyType key;
        ValueType value;
        int balance_factor;

        node* left;
        node* right;

        node(const KeyType& key): key(key), balance_factor(BALANCED), left(nullptr), right(nullptr) { }
        node(const KeyType& key, const ValueType& value): key(key), value(value), balance_factor(BALANCED), left(nullptr), right(nullptr) { }
    };
    struct stack_node {
        node* ptr;
        stack_node* prev;
        stack_node(node* ptr): ptr(ptr), prev(nullptr) { }
    };
    struct stack {
        stack_node* top;
        stack(): top(nullptr) { }
        bool empty() const { return top == nullptr; }
        void push(node* ptr) {
            if (top == nullptr) {
                top = new stack_node(ptr);
                return;
            }
            stack_node* new_top = new stack_node(ptr);
            new_top->prev = top;
            top = new_top;
        }
        node* peek() const {
            return top->ptr;
        }
        node* pop() {
            node* ptr = top->ptr;
            stack_node* tmp = top;
            top = top->prev;
            delete tmp;
            return ptr;
        }
        ~stack() {
            while (top != nullptr) {
                stack_node* tmp = top;
                top = top->prev;
                delete tmp;
            }
        }
        void print() const {
            stack_node* tmp = top;
            std::cout << "[ ";
            while (tmp != nullptr) {
                if (tmp->ptr != nullptr) {
                    std::cout << tmp->ptr->key << " ";
                }
                tmp = tmp->prev;
            }
            std::cout << "]\n";
        }
    };
    node* root;
    node* rotate_left(node* root) {
        node* new_root = root->right;
        root->right = new_root->left;
        new_root->left = root;
        
        if (new_root->balance_factor == BALANCED) {
            new_root->balance_factor = LEFT_HEAVY;
            root->balance_factor = RIGHT_HEAVY;
        } else {
            new_root->balance_factor = BALANCED;
            root->balance_factor = BALANCED;
        }
        return new_root;
    }

    node* rotate_right(node* root) {
        node* new_root = root->left;
        root->left = new_root->right;
        new_root->right = root;
        
        if (new_root->balance_factor == BALANCED) {
            new_root->balance_factor = RIGHT_HEAVY;
            root->balance_factor = LEFT_HEAVY;
        } else {
            new_root->balance_factor = BALANCED;
            root->balance_factor = BALANCED;
        }
        return new_root;
    }

    node* rotate_right_left(node* root) {
        node* new_root = root->right->left;
        root->right->left = new_root->right;
        new_root->right = root->right;
        root->right = new_root->left;
        new_root->left = root;
        
        if (new_root->balance_factor == BALANCED) {
            new_root->right->balance_factor = BALANCED;
            root->balance_factor = BALANCED;
        } else if (new_root->balance_factor == RIGHT_HEAVY) {
            new_root->right->balance_factor = BALANCED;
            root->balance_factor = LEFT_HEAVY;
        } else {
            new_root->right->balance_factor = RIGHT_HEAVY;
            root->balance_factor = BALANCED;
        }
        new_root->balance_factor = BALANCED;
        return new_root;
    }

    node* rotate_left_right(node* root) {
        node* new_root = root->left->right;
        root->left->right = new_root->left;
        new_root->left = root->left;
        root->left = new_root->right;
        new_root->right = root;
        
        if (new_root->balance_factor == BALANCED) {
            new_root->left->balance_factor = BALANCED;
            root->balance_factor = BALANCED;
        } else if (new_root->balance_factor == RIGHT_HEAVY) {
            new_root->left->balance_factor = LEFT_HEAVY;
            root->balance_factor = BALANCED;
        } else {
            new_root->left->balance_factor = BALANCED;
            root->balance_factor = RIGHT_HEAVY;
        }
        new_root->balance_factor = BALANCED;
        return new_root;
    }

    node* insert(node* root, const KeyType& key, int& grow) {
        if (root == nullptr) {
            root = new node(key);
            grow = 1;
            return root;
        }

        if (root->key < key) {
            root->right = insert(root->right, key, grow);
            if (grow) {
                ++(root->balance_factor);
                if (root == this->root) {
                    grow = 0;
                }
                if (root->balance_factor > RIGHT_HEAVY) {
                    grow = 0;
                    if (root->right->balance_factor == RIGHT_HEAVY) {
                        root = rotate_left(root);
                    } else {
                        root = rotate_right_left(root);
                    }
                } else if (root->balance_factor == BALANCED) {
                    grow = 0;
                }
            }
            return root;
        } else if (key < root->key) {
            root->left = insert(root->left, key, grow);
            if (grow) {
                --(root->balance_factor);
                if (root == this->root) {
                    grow = 0;
                }
                if (root->balance_factor < LEFT_HEAVY) {
                    grow = 0;
                    if (root->left->balance_factor == LEFT_HEAVY) {
                        root = rotate_right(root);
                    } else {
                        root = rotate_left_right(root);
                    }
                } else if (root->balance_factor == BALANCED) {
                    grow = 0;
                }
            }
            return root;
        } else {
            grow = 0;
            return root;
        }
    }
    int get_height(node* root) const {
        if (root == nullptr) {
            return 0;
        }
        if (root->left == nullptr && root->right == nullptr) {
            return 1;
        }
        return std::max(get_height(root->left), get_height(root->right)) + 1;
    }
    node* find(node* root, const KeyType& key) const {
        if (root == nullptr) {
            return nullptr;
        }
        if (root->key < key) {
            return find(root->right, key);
        } else if (key < root->key) {
            return find(root->left, key);
        } else {
            return root;
        }
    }
    void to_dot_format(node* root, std::ofstream& file) const {
        if (root == nullptr) {
            return;
        }
        if (root->left != nullptr) {
            file << "\t" << root->key << " -- " << root->left->key << "\n";
        }
        if (root->right != nullptr) {
            file << "\t" << root->key << " -- " << root->right->key << "\n";
        }
        to_dot_format(root->left, file);
        to_dot_format(root->right, file);
    }
    void print_subtree(std::ostream& out, node* root, std::string prefix, bool is_left) const {
        if (root != nullptr) {
            out << prefix;
            out << (is_left ? "├── " : "└── " );

            out << root->key << " " << "\n";

            print_subtree(out, root->right, prefix + (is_left ? "│   " : "    "), true);
            print_subtree(out, root->left, prefix + (is_left ? "│   " : "    "), false);
        }
    }
    node* smallest(node* root) const {
        if (root == nullptr) {
            return nullptr;
        }
        while (root->left != nullptr) { root = root->left; }
        return root;
    }
    node* del(node* root, const KeyType& key, int& shrink) {
        if (root == nullptr) {
            return nullptr;
        }

        if (root->key < key) {
            root->right = del(root->right, key, shrink);
            if (shrink) {
                --(root->balance_factor);
                if (root == this->root) {
                    shrink = 0;
                }
                if (root->balance_factor < LEFT_HEAVY) {
                    shrink = 0;
                    if (root->left->balance_factor == LEFT_HEAVY) {
                        root = rotate_right(root);
                    } else {
                        root = rotate_left_right(root);
                    }
                } else if (root->balance_factor != BALANCED) {
                    shrink = 0;
                }
            }
            return root;
        } else if (key < root->key) {
            root->left = del(root->left, key, shrink);
            if (shrink) {
                ++(root->balance_factor);
                if (root == this->root) {
                    shrink = 0;
                }
                if (root->balance_factor > RIGHT_HEAVY) {
                    shrink = 0;
                    if (root->right->balance_factor == RIGHT_HEAVY) {
                        root = rotate_left(root);
                    } else {
                        root = rotate_right_left(root);
                    }
                } else if (root->balance_factor != BALANCED) {
                    shrink = 0;
                }
            }
            return root;
        } else { // (root->key == key)
            if (root->left != nullptr && root->right != nullptr) {
                node* switch_node = smallest(root->left);
                root->key = switch_node->key;
                root->value = switch_node->value;
                root->left = del(root->left, switch_node->key, shrink);
                if (shrink) {
                    ++(root->balance_factor);
                    if (root == this->root) {
                        shrink = 0;
                    }
                    if (root->balance_factor > RIGHT_HEAVY) {
                        shrink = 0;
                        if (root->right->balance_factor == RIGHT_HEAVY) {
                            root = rotate_left(root);
                        } else {
                            root = rotate_right_left(root);
                        }
                    } else if (root->balance_factor != BALANCED) {
                        shrink = 0;
                    }
                }
                return root;
            }
            node* new_root = root->left != nullptr ? root->left : root->right;
            shrink = 1;
            if (root == this->root) {
                this->root = new_root;
            }
            delete root;
            return new_root;
        }
        return root; // :-)
    }
    void destroy(node* root) {
        if (root != nullptr) {
            destroy(root->left);
            destroy(root->right);
            delete root;
        }
    }
public:
    AVL(): root(nullptr) { };
    virtual ~AVL() {
        destroy(root);
    }
    AVL(const AVL<KeyType, ValueType>& other): root(nullptr) {
        for (auto it = other.begin(); it != other.end(); ++it) {
            pair p = *it;
            (*this)[p.key] = p.value;
        }
    }
    AVL<KeyType, ValueType>& operator=(const AVL<KeyType, ValueType>& other) {
        if (this == &other) { return *this; }
        destroy(root);
        for (auto it = other.begin(); it != other.end(); ++it) {
            pair p = *it;
            (*this)[p.key] = p.value;
        }
        return *this;
    }
    void destroy() {
        destroy(root);
    }
    struct pair {
        const KeyType key;
        ValueType& value;
        pair(const KeyType& key, ValueType& value): key(key), value(value) { }
        friend std::ostream& operator<<(std::ostream& out, const pair& p) {
            std::cout << p.key << " " << p.value;
            return out;
        }
    };
    struct const_pair {
        const KeyType& key;
        const ValueType& value;
        const_pair(const KeyType& key, const ValueType& value): key(key), value(value) { }
        friend std::ostream& operator<<(std::ostream& out, const const_pair& p) {
            std::cout << p.key << " " << p.value;
            return out;
        }
    };
    class iterator {
    private:
        stack st;
    public:
        iterator(node* root) {
            if (root == nullptr) {
                return;
            }
            st.push(root);
            while (st.peek()->left) {
                st.push(st.peek()->left);
            }
        }
        pair operator*() {
            return pair(st.peek()->key, st.peek()->value);
        }
        const_pair operator*() const {
            return const_pair(st.peek()->key, st.peek()->value);
        }
        iterator& operator++() {
            if (st.peek()->right) {
                st.push(st.peek()->right);
                while (st.peek()->left) {
                    st.push(st.peek()->left);
                }
                return *this;
            }
            node* tmp = st.pop();
            if (st.empty()) {
                return *this;
            }
            while (st.peek()->right == tmp) {
                tmp = st.pop();
                if (st.empty()) {
                    return *this;
                }
            }
            return *this;
        }
        bool operator==(const iterator& other) const {
            if (other.st.empty() && st.empty()) {
                return true;
            }
            if (other.st.empty() || st.empty()) {
                return false;
            }
            return st.peek() == other.st.peek();
        }
        bool operator!=(const iterator& other) const {
            return !(other == *this);
        }
    };
    iterator begin() {
        return iterator(root);
    }
    iterator end() {
        return iterator(nullptr);
    }
    iterator begin() const {
        return iterator(root);
    }
    iterator end() const {
        return iterator(nullptr);
    }
    void to_dot_format(const std::string& filename) const {
        std::ofstream file;
        file.open(filename);
        if (!file) {
            return;
        }
        file << "graph avl {\n";
        file << "\tnode [shape=circle style=filled fillcolor=\"#7bc99a\" fixedsize=true]\n";
        to_dot_format(root, file);
        file << "}\n";
        file.close();
    }
    void gen_png(const std::string& filename) const {
        std::string cmd = "dot -Tpng " + filename + ".gv -o" + filename + ".png";
        std::system(cmd.c_str());
    }
    void gen_svg(const std::string& filename) const {
        std::string cmd = "dot -Tsvg " + filename + ".gv -o" + filename + ".svg";
        std::system(cmd.c_str());
    }
    bool empty() const {
        return root == nullptr;
    }
    bool balanced() const {
        if (root == nullptr) {
            return true;
        }
        int balance = get_height(root->right) - get_height(root->left);
        return balance == BALANCED || balance == LEFT_HEAVY || balance == RIGHT_HEAVY;
    }
    friend std::ostream& operator<<(std::ostream& out, const AVL<KeyType, ValueType>& avl) {
        avl.print_subtree(out, avl.root, "", false);
        out << "\n";
        return out;
    }
    node* smallest() const {
        return smallest(root);
    }
    void del(const KeyType& key) {
        int shrink = 0;
        del(root, key, shrink);
    }
    ValueType& operator[](const KeyType& key) {
        int grow = 0;
        node* res;
        res = find(root, key);
        if (res != nullptr) {
            return res->value;
        }
        root = insert(root, key, grow);
        res = find(root, key);
        return res->value;
    }

    const ValueType& operator[](const KeyType& key) const {
        node* res;
        res = find(root, key);
        if (res != nullptr) {
            return res->value;
        }
        throw std::out_of_range("value not found.");
    }

    bool has(const KeyType& key) const {
        node* res;
        res = find(root, key);
        return res != nullptr;
    }
};