#include "avl.h"
#include <chrono>
#include <thread>
#include <map>
#include <cmath>
#include <random>

int randint(int max) {
    return rand() % max;
}

void test_ints_as_keys();
void test_strings_as_keys();
void test_delete();
void time_it();

int main(int argc, char* argv[]) {
    test_ints_as_keys();
    test_strings_as_keys();
    test_delete();
    time_it();
 
    return 0;
}

// ======= TESTS =======


void fill_avl_ints_ints(AVL<int, int>& avl, int n, int max = 100) {
    int value = 1;
    int key = 0;
    for (int i = 0; i < n; ++i) {
        key = randint(max);
        avl[key] = value;
    }
}

void test_strings_as_keys() {
    std::cout << "======================== STRINGS ==========================\n";

    AVL<std::string, int> avl;

    std::vector<std::string> keys = {"INTEGER", "FLOAT", "STRING", "TRUE", "FALSE", "FOR", "BREAK", "IF", "ELSE", "AND", "OR", "IMPLIES", "IDENTIFIER", "LESS", "LESS_OR_EQUAL", "GREATER", "GREATER_OR_EQUAL", "NOT_EQUAL", "EQUAL"};

    for (int i = 0; i < (int)keys.size(); ++i) {
        std::cout << "iteration " << i << "\n";
        avl[keys[i]] = i + 1;
        std::cout << avl;
    }

    for (auto it = avl.begin(); it != avl.end(); ++it) {
        std::cout << *it << "\n";
    }

    if (avl.balanced()) {
        std::cout << "tree is balanced\n";
    } else {
        std::cout << "tree is unbalanced :-(\n";
    }
}

void test_ints_as_keys() {
    std::cout << "=========================== INTS ================================================\n";

    AVL<int, int> avl;
    int value = 1;
    int key = 0;
    for (int i = 0; i < 20; ++i) {
        std::cout << "iteration " << i << "\n";
        key = randint(20);
        avl[key] = value;
        std::cout << avl;
        //avl.to_dot_format("iter.gv");
        //avl.gen_png("iter");
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }

    for (auto it = avl.begin(); it != avl.end(); ++it) {
        std::cout << *it << "\n";
    }
    
    if (avl.balanced()) {
        std::cout << "tree is balanced\n";
    } else {
        std::cout << "tree is unbalanced :-(\n";
    }
    //avl.to_dot_format("ints_test.gv");
    //avl.gen_png("ints_test");
}

void test_delete() {
    std::cout << "=========================== DELETE ================================================\n";

    AVL<int, int> avl;
    int value = 1;
    for (int i = 0; i < 10; ++i) {
        std::cout << "iteration " << i << "\n";
        avl[i] = value;
        //avl.to_dot_format("iter.gv");
        //avl.gen_png("iter");
        //std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
    std::cout << avl;
    for (int i = 0; i < 10; ++i) {
        std::cout << "iteration " << i << "\n";
        avl.del(i);
        std::cout << avl;
        //avl.to_dot_format("iter.gv");
        //avl.gen_png("iter");
        //std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
    
    if (avl.balanced()) {
        std::cout << "tree is balanced\n";
    } else {
        std::cout << "tree is unbalanced :-(\n";
    }
}

void time_it() {
    std::cout << "=========================== STL TIME COMPARISON ================================================\n";

    AVL<int, int> avl;
    std::map<int, int> m;
    auto begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 10000; ++i) {
        avl[i] = i + 1;
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] my realization\n";

    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 10000; ++i) {
        m[i] = i + 1;
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] stl realization\n";

    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 10000; ++i) {
        m.erase(i);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] stl realization\n";

    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 10000; ++i) {
        avl.del(i);
    }
    end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] my realization\n";

}