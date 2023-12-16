#include "avl.h"
#include <chrono>
#include <thread>
#include <cmath>
#include <functional>

enum class colors : uint32_t {
    alice_blue = 0xF0F8FF,               // rgb(240,248,255)
    antique_white = 0xFAEBD7,            // rgb(250,235,215)
    aqua = 0x00FFFF,                     // rgb(0,255,255)
    aquamarine = 0x7FFFD4,               // rgb(127,255,212)
    azure = 0xF0FFFF,                    // rgb(240,255,255)
    beige = 0xF5F5DC,                    // rgb(245,245,220)
    bisque = 0xFFE4C4,                   // rgb(255,228,196)
    black = 0x000000,                    // rgb(0,0,0)
    blanched_almond = 0xFFEBCD,          // rgb(255,235,205)
    blue = 0x0000FF,                     // rgb(0,0,255)
    blue_violet = 0x8A2BE2,              // rgb(138,43,226)
    brown = 0xA52A2A,                    // rgb(165,42,42)
    burly_wood = 0xDEB887,               // rgb(222,184,135)
    cadet_blue = 0x5F9EA0,               // rgb(95,158,160)
    chartreuse = 0x7FFF00,               // rgb(127,255,0)
    chocolate = 0xD2691E,                // rgb(210,105,30)
    coral = 0xFF7F50,                    // rgb(255,127,80)
    cornflower_blue = 0x6495ED,          // rgb(100,149,237)
    cornsilk = 0xFFF8DC,                 // rgb(255,248,220)
    crimson = 0xDC143C,                  // rgb(220,20,60)
    cyan = 0x00FFFF,                     // rgb(0,255,255)
    dark_blue = 0x00008B,                // rgb(0,0,139)
    dark_cyan = 0x008B8B,                // rgb(0,139,139)
    dark_golden_rod = 0xB8860B,          // rgb(184,134,11)
    dark_gray = 0xA9A9A9,                // rgb(169,169,169)
    dark_green = 0x006400,               // rgb(0,100,0)
    dark_khaki = 0xBDB76B,               // rgb(189,183,107)
    dark_magenta = 0x8B008B,             // rgb(139,0,139)
    dark_olive_green = 0x556B2F,         // rgb(85,107,47)
    dark_orange = 0xFF8C00,              // rgb(255,140,0)
    dark_orchid = 0x9932CC,              // rgb(153,50,204)
    dark_red = 0x8B0000,                 // rgb(139,0,0)
    dark_salmon = 0xE9967A,              // rgb(233,150,122)
    dark_sea_green = 0x8FBC8F,           // rgb(143,188,143)
    dark_slate_blue = 0x483D8B,          // rgb(72,61,139)
    dark_slate_gray = 0x2F4F4F,          // rgb(47,79,79)
    dark_turquoise = 0x00CED1,           // rgb(0,206,209)
    dark_violet = 0x9400D3,              // rgb(148,0,211)
    deep_pink = 0xFF1493,                // rgb(255,20,147)
    deep_sky_blue = 0x00BFFF,            // rgb(0,191,255)
    dim_gray = 0x696969,                 // rgb(105,105,105)
    dodger_blue = 0x1E90FF,              // rgb(30,144,255)
    fire_brick = 0xB22222,               // rgb(178,34,34)
    floral_white = 0xFFFAF0,             // rgb(255,250,240)
    forest_green = 0x228B22,             // rgb(34,139,34)
    fuchsia = 0xFF00FF,                  // rgb(255,0,255)
    gainsboro = 0xDCDCDC,                // rgb(220,220,220)
    ghost_white = 0xF8F8FF,              // rgb(248,248,255)
    gold = 0xFFD700,                     // rgb(255,215,0)
    golden_rod = 0xDAA520,               // rgb(218,165,32)
    gray = 0x808080,                     // rgb(128,128,128)
    green = 0x008000,                    // rgb(0,255,0)
    green_yellow = 0xADFF2F,             // rgb(173,255,47)
    honey_dew = 0xF0FFF0,                // rgb(240,255,240)
    hot_pink = 0xFF69B4,                 // rgb(255,105,180)
    indian_red = 0xCD5C5C,               // rgb(205,92,92)
    indigo = 0x4B0082,                   // rgb(75,0,130)
    ivory = 0xFFFFF0,                    // rgb(255,255,240)
    khaki = 0xF0E68C,                    // rgb(240,230,140)
    lavender = 0xE6E6FA,                 // rgb(230,230,250)
    lavender_blush = 0xFFF0F5,           // rgb(255,240,245)
    lawn_green = 0x7CFC00,               // rgb(124,252,0)
    lemon_chiffon = 0xFFFACD,            // rgb(255,250,205)
    light_blue = 0xADD8E6,               // rgb(173,216,230)
    light_coral = 0xF08080,              // rgb(240,128,128)
    light_cyan = 0xE0FFFF,               // rgb(224,255,255)
    light_golden_rod_yellow = 0xFAFAD2,  // rgb(250,250,210)
    light_gray = 0xD3D3D3,               // rgb(211,211,211)
    light_green = 0x90EE90,              // rgb(144,238,144)
    light_pink = 0xFFB6C1,               // rgb(255,182,193)
    light_salmon = 0xFFA07A,             // rgb(255,160,122)
    light_sea_green = 0x20B2AA,          // rgb(32,178,170)
    light_sky_blue = 0x87CEFA,           // rgb(135,206,250)
    light_slate_gray = 0x778899,         // rgb(119,136,153)
    light_steel_blue = 0xB0C4DE,         // rgb(176,196,222)
    light_yellow = 0xFFFFE0,             // rgb(255,255,224)
    lime = 0x00FF00,                     // rgb(0,255,0)
    lime_green = 0x32CD32,               // rgb(50,205,50)
    linen = 0xFAF0E6,                    // rgb(250,240,230)
    magenta = 0xFF00FF,                  // rgb(255,0,255)
    maroon = 0x800000,                   // rgb(128,0,0)
    medium_aquamarine = 0x66CDAA,        // rgb(102,205,170)
    medium_blue = 0x0000CD,              // rgb(0,0,205)
    medium_orchid = 0xBA55D3,            // rgb(186,85,211)
    medium_purple = 0x9370DB,            // rgb(147,112,219)
    medium_sea_green = 0x3CB371,         // rgb(60,179,113)
    medium_slate_blue = 0x7B68EE,        // rgb(123,104,238)
    medium_spring_green = 0x00FA9A,      // rgb(0,250,154)
    medium_turquoise = 0x48D1CC,         // rgb(72,209,204)
    medium_violet_red = 0xC71585,        // rgb(199,21,133)
    midnight_blue = 0x191970,            // rgb(25,25,112)
    mint_cream = 0xF5FFFA,               // rgb(245,255,250)
    misty_rose = 0xFFE4E1,               // rgb(255,228,225)
    moccasin = 0xFFE4B5,                 // rgb(255,228,181)
    navajo_white = 0xFFDEAD,             // rgb(255,222,173)
    navy = 0x000080,                     // rgb(0,0,128)
    old_lace = 0xFDF5E6,                 // rgb(253,245,230)
    olive = 0x808000,                    // rgb(128,128,0)
    olive_drab = 0x6B8E23,               // rgb(107,142,35)
    orange = 0xFFA500,                   // rgb(255,165,0)
    orange_red = 0xFF4500,               // rgb(255,69,0)
    orchid = 0xDA70D6,                   // rgb(218,112,214)
    pale_golden_rod = 0xEEE8AA,          // rgb(238,232,170)
    pale_green = 0x98FB98,               // rgb(152,251,152)
    pale_turquoise = 0xAFEEEE,           // rgb(175,238,238)
    pale_violet_red = 0xDB7093,          // rgb(219,112,147)
    papaya_whip = 0xFFEFD5,              // rgb(255,239,213)
    peach_puff = 0xFFDAB9,               // rgb(255,218,185)
    peru = 0xCD853F,                     // rgb(205,133,63)
    pink = 0xFFC0CB,                     // rgb(255,192,203)
    plum = 0xDDA0DD,                     // rgb(221,160,221)
    powder_blue = 0xB0E0E6,              // rgb(176,224,230)
    purple = 0x800080,                   // rgb(128,0,128)
    rebecca_purple = 0x663399,           // rgb(102,51,153)
    red = 0xFF0000,                      // rgb(255,0,0)
    rosy_brown = 0xBC8F8F,               // rgb(188,143,143)
    royal_blue = 0x4169E1,               // rgb(65,105,225)
    saddle_brown = 0x8B4513,             // rgb(139,69,19)
    salmon = 0xFA8072,                   // rgb(250,128,114)
    sandy_brown = 0xF4A460,              // rgb(244,164,96)
    sea_green = 0x2E8B57,                // rgb(46,139,87)
    sea_shell = 0xFFF5EE,                // rgb(255,245,238)
    sienna = 0xA0522D,                   // rgb(160,82,45)
    silver = 0xC0C0C0,                   // rgb(192,192,192)
    sky_blue = 0x87CEEB,                 // rgb(135,206,235)
    slate_blue = 0x6A5ACD,               // rgb(106,90,205)
    slate_gray = 0x708090,               // rgb(112,128,144)
    snow = 0xFFFAFA,                     // rgb(255,250,250)
    spring_green = 0x00FF7F,             // rgb(0,255,127)
    steel_blue = 0x4682B4,               // rgb(70,130,180)
    tan = 0xD2B48C,                      // rgb(210,180,140)
    teal = 0x008080,                     // rgb(0,128,128)
    thistle = 0xD8BFD8,                  // rgb(216,191,216)
    tomato = 0xFF6347,                   // rgb(255,99,71)
    turquoise = 0x40E0D0,                // rgb(64,224,208)
    violet = 0xEE82EE,                   // rgb(238,130,238)
    wheat = 0xF5DEB3,                    // rgb(245,222,179)
    white = 0xFFFFFF,                    // rgb(255,255,255)
    white_smoke = 0xF5F5F5,              // rgb(245,245,245)
    yellow = 0xFFFF00,                   // rgb(255,255,0)
    yellow_green = 0x9ACD32              // rgb(154,205,50)
};

struct rgb {
    rgb() : r(0), g(0), b(0) { }
    rgb(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) { }
    rgb(uint32_t hex)
      : r((hex >> 16) & 0xFF), g((hex >> 8) & 0xFF), b(hex & 0xFF) { }
    rgb(colors hex)
      : r((uint32_t(hex) >> 16) & 0xFF),
        g((uint32_t(hex) >> 8) & 0xFF),
        b(uint32_t(hex) & 0xFF) { }
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct color: rgb {
    bool is_real;
    color(): rgb(), is_real(false) { }
    color(const rgb& c): rgb(c), is_real(true) { }
    color(colors c): rgb(c), is_real(true) { }
};

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

void print_pixel(rgb c) {
    std::string cmd = "\x1b[38;2;" + std::to_string(c.r) + ";" + std::to_string(c.g) + ";" + std::to_string(c.b) + "m";
    std::cout << cmd << "██";
}

class image: public AVL<coord, color> {
private:
    int w, h;
public:
    image(): AVL<coord, color>(), w(0), h(0) { }
    image(int width, int height): AVL<coord, color>(), w(width), h(height) { }
    void fill(color c = colors::white) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                (*this)[coord(i, j)] = c;
            }
        }
    }
    void clear() {
        for (auto it = begin(); it != end(); ++it) {
            (*it).value.is_real = false;
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
                    (*this)(i, j) = rgb(r, g, b);
                }
            }
        }
        file.close();
    }
    color& operator()(int i, int j) {
        return (*this)[coord(i, j)];
    }
    const color& operator()(int i, int j) const {
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
                    color c = (*this)(i, j);
                    if (c.is_real) {
                        print_pixel(c);
                    } else {
                        std::cout << "  ";
                    }
                } else {
                    std::cout << "  ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\x1b[0m";
    }
    void show(int x, int y) const {
        std::cout << "\x1b[" << y << "E";
        for (int i = 0; i < h; ++i) {
            std::cout << "\x1b[" << x << "C";
            for (int j = 0; j < w; ++j) {
                if ((*this).has(coord(i, j))) {
                    color c = (*this)(i, j);
                    if (c.is_real) {
                        print_pixel(c);
                    } else {
                        std::cout << "  ";
                    }
                } else {
                    std::cout << "  ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\x1b[0m";
        std::cout << "\x1b[" << y + height() << "A";
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
    color& operator()(int i, int j) {
        return states[curr_state](i, j);
    }
    const color& operator()(int i, int j) const {
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
    circle(int x, int y, int radius, rgb color): sprite(2 * radius + 2, 2 * radius + 2, x, y) {
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
    rect(int x, int y, int width = 5, int height = 5, rgb color = colors::orange): sprite(width, height, x, y) {
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
    color background_color;
    image canvas;
    int n_sprites;
    AVL<int, sprite> sprites;
public:
    screen(
        int width,
        int height,
        color background_color):
            width(width),
            height(height),
            background_color(background_color),
            canvas(width, height),
            n_sprites(0) { }
    screen(
        int width=10,
        int height=10):
            width(width),
            height(height),
            background_color(),
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
        if (background_color.is_real) {
            canvas.fill(background_color);
        } else {
            canvas.clear();
        }

        for (auto it = sprites.begin(); it != sprites.end(); ++it) {
            canvas.fill_subimg<sprite>((*it).value.x, (*it).value.y, (*it).value);
        }
        canvas.show();
    }
    void mainloop(int iters, int fps = 100) {
        std::cout << "\x1b[2J";
        std::cout << "\033[?25l";
        for (int i = 0; i < iters; ++i) {
            std::cout << "\x1b[H";
            show();
            std::this_thread::sleep_for(std::chrono::milliseconds(fps));
            update();
        }
        std::cout << "\033[?25h";
    }
};

void animate() {
    int w = 32, h = 32;
    screen<sprite> c(w, h);

    rect r(3, 3, 4, 4, colors::orange);
    c.embed(r);

    circle c1(5, 10, 3, colors::orange);
    circle c2(10, 10, 10, colors::aquamarine);
    c.embed(c1);
    c.embed(c2);

    rect r1(16, 16, 5, 6, colors::light_coral);
    rect r2(5, 25, 2, 3, colors::tomato);
    c.embed(r1);
    c.embed(r2);

    c.show();
}

void show_cat() {
    int w = 16, h = 16;
    screen<animated_sprite> c(w, h);
    animated_sprite cat(0, 0, 1, "cat");
    c.embed(cat);
    c.show();
}

void show_yoda() {
    int w = 16, h = 16;
    screen<animated_sprite> c(w, h);
    animated_sprite yoda(0, 0, 1, "yoda");
    c.embed(yoda);
    c.show();
}

void show_gopher() {
    int w = 16, h = 16;
    screen<animated_sprite> c(w, h);
    animated_sprite go(0, 0, 1, "gopher");
    c.embed(go);
    c.show();
}

void animate_fox_wag() {
    int w = 20, h = 16;
    screen<animated_sprite> c(w, h);
    animated_sprite fox(0, 0, 5, "wag");
    c.embed(fox);
    c.mainloop(100, 60);
}

void animate_fox_lying() {
    int w = 20, h = 16;
    screen<animated_sprite> c(w, h);
    animated_sprite fox(0, 0, 6, "lying");
    c.embed(fox);
    c.mainloop(100, 150);
}

void animate_fox_wag_head_rotate() {
    int w = 20, h = 16;
    screen<animated_sprite> c(w, h);
    animated_sprite fox(0, 0, 14, "wag_head_rotate");
    c.embed(fox);
    c.mainloop(100, 60);
}

void animate_fox_scared() {
    int w = 20, h = 16;
    screen<animated_sprite> c(w, h);
    animated_sprite fox(0, 0, 5, "scared");
    c.embed(fox);
    c.mainloop(100, 120);
}

void animate_fox_jump() {
    int w = 85, h = 16;
    screen<animated_sprite> c(w, h);
    animated_sprite fox(0, 0, 8, "jump", 1);
    c.embed(fox);
    c.mainloop(100, 100);
}

void animate_fox_long_jump() {
    int w = 85, h = 20;
    screen<animated_sprite> c(w, h);
    animated_sprite fox(0, 0, 11, "long_jump", 1);
    c.embed(fox);
    c.mainloop(100, 100);
}

void show_help_message() {
    std::cout << "usage pet [name]\n";
    std::cout << "\tfox:\n";
    std::cout << "\t\twag\n";
    std::cout << "\t\thead_rotate\n";
    std::cout << "\t\tidle\n";
    std::cout << "\t\tjump\n";
    std::cout << "\t\tljump\n";
    std::cout << "\t\tscared\n";
    std::cout << "\tyoda\n";
    std::cout << "\tcat\n";
    std::cout << "\tanim\n";
    std::cout << "\tgopher\n";
}

int main(int argc, char* argv[]) {
    
    AVL<std::string, std::function<void()>> avl;
    avl["wag"] = animate_fox_wag;
    avl["head_rotate"] = animate_fox_wag_head_rotate;
    avl["jump"] = animate_fox_jump;
    avl["scared"] = animate_fox_scared;
    avl["ljump"] = animate_fox_long_jump;
    avl["idle"] = animate_fox_lying;
    avl["anim"] = animate;
    avl["cat"] = show_cat;
    avl["yoda"] = show_yoda;
    avl["go"] = show_gopher;
    avl["--help"] = show_help_message;

    if (argc > 0) {
        std::string cmd(argv[1]);
        if (avl.has(cmd)) {
            avl[cmd]();
        }
    } else {
        animate_fox_wag();
    }

    return 0;
}