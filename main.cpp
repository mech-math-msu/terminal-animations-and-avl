#include "avl.h"
#include <chrono>
#include <vector>
#include <thread>
#include <map>
#include <cmath>
#include <random>

void test_ints_as_keys();
void test_strings_as_keys();
void test_delete();
void time_it();

int randint(int max) {
    auto delta = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return delta % max;
}

struct coord {
    int x, y;
    coord(): x(0), y(0) { }
    coord(int x, int y): x(x), y(y) { }
    friend std::ostream& operator<<(std::ostream& out, const coord& c) {
        out << c.x << " " << c.y;
        return out;
    }
    bool operator==(const coord& other) const { return x == other.x && y == other.y; }
    bool operator!=(const coord& other) const { return !(*this == other); }
    bool operator>(const coord& other) const { return (x > other.x) || (x == other.x && (y > other.y)); }
    bool operator>=(const coord& other) const { return *this == other || *this > other; }
    bool operator<(const coord& other) const { return !(*this >= other); }
    bool operator<=(const coord& other) const { return !(*this > other); }
};

void print_pixel(fmt::detail::color_type c) {
    fmt::print(fmt::fg(c), "██");
}

class image: public AVL<coord, fmt::detail::color_type> {
private:
    int w, h;
public:
    image(): AVL<coord, fmt::detail::color_type>(), w(0), h(0) { }
    image(int width, int height): AVL<coord, fmt::detail::color_type>(), w(width), h(height) { }
    void fill(fmt::detail::color_type color = fmt::color::white) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                (*this)[coord(i, j)] = color;
            }
        }
    }
    image(const std::string& filepath) {
        std::ifstream file;
        file.open(filepath);
        file >> w >> h;
        int r, g, b;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                file >> r >> g >> b;
                if (r != -1) {
                    (*this)(i, j) = fmt::rgb(r, g, b);
                } 
                // else {
                //     (*this)(i, j) = fmt::color::red;
                // }
            }
        }
        file.close();
    }
    fmt::detail::color_type& operator()(int i, int j) {
        return (*this)[coord(i, j)];
    }
    const fmt::detail::color_type& operator()(int i, int j) const {
        return (*this)[coord(i, j)];
    }
    template <typename image_type>
    void fill_subimg(int x, int y, const image_type& img) {
        if (x + img.width() < 0 || x > w || y > h || y + img.height() < 0) {
            return;
        }
        for (int i = std::max(h - y - img.height(), 0); i < std::min(h - y, h); ++i) {
            for (int j = x; j < std::min(w, x + img.width()); ++j) {
                if (img.has(coord(i - std::max(h - y - img.height(), 0), j - x))) {
                    (*this)[coord(i, j)] = img(i - std::max(h - y - img.height(), 0), j - x);
                }
            }
        }
    }
    void fill_subimg(coord p, const image& img) {
        int x = p.x, y = p.y;
        fill_subimg(x, y, img);
    }
    int width() const { return w; }
    int height() const { return h; }
    void show() const {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if ((*this).has(coord(i, j))) {
                    print_pixel((*this)(i, j));
                } else {
                    fmt::print("  ");//print_pixel(bg);
                }
            }
            fmt::print("\n");
        }
    }
    void show(int x, int y) const {
        fmt::print("\x1b[{}E", y);
        for (int i = 0; i < h; ++i) {
            fmt::print("\x1b[{}C", x);
            for (int j = 0; j < w; ++j) {
                if ((*this).has(coord(i, j))) {
                    print_pixel((*this)(i, j));
                } else {
                    fmt::print("  ");
                }
            }
            fmt::print("\n");
        }
        fmt::print("\x1b[{}A", y + height());
    }
};

class sprite: public image {
public:
    int x, y;
    sprite(): image(0, 0), x(0), y(0) { }
    sprite(int w, int h, int x, int y): image(w, h), x(x), y(y) { }
    void update() { }
};

class animated_sprite {
private:
    AVL<int, image> states;
    int curr_state;
    int n_states;
    int vx, vy;
public:
    int x, y;
    animated_sprite(): curr_state(0), n_states(0), vx(0), vy(0), x(0), y(0) { }
    animated_sprite(int x, int y, int n_states, const std::string& fn, int vx = 0, int vy = 0): curr_state(0), n_states(n_states), vx(vx), vy(vy), x(x), y(y) {
        for (int i = 0; i < n_states; ++i) {
            image state("sprites/" + fn + std::to_string(i) + ".pxl");
            states[i] = state;
        }
    }
    void upd_sprite_sheet(int n_states, const std::string& fn) {
        this->n_states = n_states;
        curr_state = 0;
        for (int i = 0; i < n_states; ++i) {
            image state("sprites/" + fn + std::to_string(i) + ".pxl");
            states[i] = state;
        }
    }
    void update() {
        ++curr_state;
        curr_state %= n_states;
        if (vx) { x += vx; if (x > 85) { x = -width() + 1; } }
        if (vy) { x += vy; }
    }
    fmt::detail::color_type& operator()(int i, int j) {
        return states[curr_state](i, j);
    }
    const fmt::detail::color_type& operator()(int i, int j) const {
        return states[curr_state](i, j);
    }
    bool has(coord p) const {
        return states[curr_state].has(p);
    }
    const image& at(int i) const {
        return states[i];
    }
    void show() const {
        states[curr_state].show();
    }
    void show(int x, int y) const {
        states[curr_state].show(x, y);
    }
    int width() const { return states[curr_state].width(); }
    int height() const { return states[curr_state].height(); }
};

class circle: public sprite {
public:
    circle(int x, int y, int radius, fmt::detail::color_type color): sprite(2 * radius + 2, 2 * radius + 2, x, y) {
        int i, j;
        for (double t = 0.05; t < 2 * 3.14; t += 0.05) {
            j = radius + (int)(radius * std::cos(t));
            i = radius + (int)(radius * std::sin(t));
            (*this)(i, j) = color;
        }
    }
    void update() { }
};

class rect: public sprite {
public:
    rect(int x, int y, int width = 5, int height = 5, fmt::detail::color_type color = fmt::color::orange): sprite(width, height, x, y) {
        for (int j = 0; j < width; ++j) {
            (*this)(0, j) = color;
        }
        for (int j = 0; j < width; ++j) {
            (*this)(height - 1, j) = color;
        }
        for (int i = 0; i < height; ++i) {
            (*this)(i, 0) = color;
        }
        for (int i = 0; i < height; ++i) {
            (*this)(i, width - 1) = color;
        }
    }
    void update() { }
};

template <typename sprite>
class screen {
private:
    int width, height;
    fmt::detail::color_type background_color;
    image canvas;
    int n_sprites;
    AVL<int, sprite> sprites;
public:
    screen(
        int width=10,
        int height=10,
        fmt::detail::color_type background_color=fmt::color::white):
            width(width),
            height(height),
            background_color(background_color),
            canvas(width, height),
            n_sprites(0) { }
    void embed(const sprite& s) {
        sprites[n_sprites] = s;
        ++n_sprites;
    }
    void update() {
        for (int i = 0; i < n_sprites; ++i) {
            sprites[i].update();
        }
    }
    sprite& at(int i) {
        if (i < n_sprites) {
            return sprites[i];
        }
    }
    void show() {
        canvas.fill(background_color);
        for (auto it = sprites.begin(); it != sprites.end(); ++it) {
            canvas.fill_subimg<sprite>((*it).value.x, (*it).value.y, (*it).value);
        }
        canvas.show();
    }
    void mainloop(int iters, int fps = 100) {
        fmt::print("\x1b[2J");
        fmt::print("\033[?25l");
        for (int i = 0; i < iters; ++i) {
            fmt::print("\x1b[H");
            show();
            std::this_thread::sleep_for(std::chrono::milliseconds(fps));
            update();
        }
        fmt::print("\033[?25h");
    }
};

void animate() {
    int w = 32, h = 32;
    screen<sprite> c(w, h, fmt::color::black);

    rect r(3, 3, 4, 4, fmt::color::orange);
    c.embed(r);

    circle c1(5, 10, 3, fmt::color::orange);
    circle c2(10, 10, 10, fmt::color::aquamarine);
    c.embed(c1);
    c.embed(c2);

    rect r1(16, 16, 5, 6, fmt::color::light_coral);
    rect r2(5, 25, 2, 3, fmt::color::tomato);
    c.embed(r1);
    c.embed(r2);

    c.mainloop(30);
}

void animate_fox_wag() {
    int w = 20, h = 16;
    screen<animated_sprite> c(w, h, fmt::color::black);
    animated_sprite fox(0, 0, 5, "wag");
    c.embed(fox);
    c.mainloop(100, 60);
}

void animate_fox_lying() {
    int w = 20, h = 16;
    screen<animated_sprite> c(w, h, fmt::color::black);
    animated_sprite fox(0, 0, 6, "lying");
    c.embed(fox);
    c.mainloop(100, 60);
}

void animate_fox_wag_head_rotate() {
    int w = 20, h = 16;
    screen<animated_sprite> c(w, h, fmt::color::black);
    animated_sprite fox(0, 0, 14, "wag_head_rotate");
    c.embed(fox);
    c.mainloop(100, 60);
}

void animate_fox_jump() {
    int w = 85, h = 16;
    screen<animated_sprite> c(w, h, fmt::color::black);
    animated_sprite fox(0, 0, 8, "jump", 1);
    c.embed(fox);
    c.mainloop(100, 60);
}

void animate_fox_long_jump() {
    int w = 85, h = 16;
    screen<animated_sprite> c(w, h, fmt::color::black);
    animated_sprite fox(0, 0, 11, "long_jump", 1);
    c.embed(fox);
    c.mainloop(100, 60);
}

int main() {
    //test_ints_as_keys();
    //test_strings_as_keys();
    //test_delete();
    //time_it();
    
    //image img(5, 5);

    //std::cout << img;

    //fmt::print("{}\n", img[0][0].value.rgb_fmt::detail::color_type);

    //print_img(img);
    //animate_matrix();
    animate_fox_wag();

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
        avl.to_dot_format("iter.gv");
        avl.gen_png("iter");
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
    avl.to_dot_format("ints_test.gv");
    avl.gen_png("ints_test");
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