#include <iostream>
#include <functional>

#define times(n) for(int _n=0,_m=(n);_n<_m;_n++)

/// @brief LapisLazuli is a collection of utilities for OI.
namespace ll {

    template<typename T>
    constexpr void panic(T msg) {
        std::cerr << msg << std::endl;
        std::abort();
    }

    constexpr void panic() { panic(""); }

    template<typename T = int>
    class Rng final {
        T const left, right;
    public:
        constexpr Rng(T left, T right) noexcept : left(left), right(right) {}
        class Iterator final {
            T curr;
        public:
            constexpr Iterator(T start) noexcept : curr(start) {}
            constexpr T operator*() const noexcept { return curr; }
            constexpr Iterator& operator++() noexcept { ++curr; return *this; }
            constexpr bool operator!=(const Iterator& other) const noexcept { return curr != other.curr; }
        };
        constexpr Iterator begin() const noexcept { return Iterator(left); }
        constexpr Iterator end() const noexcept { return Iterator(right); }
    };

    // template<int W = 1024, int H = 1024>
    class Grid final {
    private:
        static constexpr int W = 1024, H = 1024;
        using T = char;
        static inline T MAP[W][H];
        static inline std::vector<bool> DONE = std::vector(W * H, false);
    public:
        static inline int WIDTH = W, HEIGHT = H;
        static constexpr void set(int width, int height) noexcept { WIDTH = width; HEIGHT = height; }
        static inline void init(std::istream& input) noexcept {
            for (auto y : Rng(0, HEIGHT)) for (auto x : Rng(0, WIDTH)) input >> MAP[x][y];
        }
        static inline void output(std::ostream& output) noexcept {
            for (auto y : Rng(0, HEIGHT)) {
                for (auto x : Rng(0, WIDTH))
                    output << MAP[x][y];
                output << std::endl;
            }
        }
        static inline void debug() noexcept {
            std::cerr << std::endl << "┌";
            times(WIDTH) std::cerr << "─";
            std::cerr << WIDTH << std::endl;
            for (auto y : Rng(0, HEIGHT)) {
                std::cerr << "│";
                for (auto x : Rng(0, WIDTH))
                    std::cerr << MAP[x][y];
                std::cerr << std::endl;
            }
            std::cerr << HEIGHT << std::endl << std::endl;
        }
        static constexpr void refresh() noexcept {
            for (auto i : Rng(0, WIDTH* HEIGHT))DONE[i] = false;
        }
        int const x, y;
        constexpr Grid() noexcept : x(-1), y(-1) {}
        constexpr Grid(int x, int y) noexcept : x(x), y(y) {}
        constexpr Grid dx(int dx) const noexcept { return Grid(x + dx, y); }
        constexpr Grid dy(int dy) const noexcept { return Grid(x, y + dy); }
        constexpr bool operator==(Grid other) const noexcept { return this->x == other.x && this->y == other.y; };
        constexpr bool valid() const noexcept {
            return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
        }
        constexpr char& tile() const noexcept {
            if (!this->valid()) panic("visiting invalid tile");
            return MAP[this->x][this->y];
        }
        friend constexpr std::ostream& operator<<(std::ostream& output, Grid map) noexcept {
            return output << "(" << map.x << ", " << map.y << ")";
        }
        constexpr std::vector<bool>::reference done() const noexcept {
            return DONE[this->y * WIDTH + this->x];
        }
        constexpr std::vector<Grid> neighbor() const noexcept {
            return { this->dy(1), this->dx(1), this->dx(-1), this->dy(-1) };
        }
        constexpr void walk(std::function<bool(Grid)> cond, std::function<void(Grid)> then) const noexcept {
            if (!this->valid()) return;
            if (this->done()) return;
            if (!cond(*this)) return;
            this->done() = true;
            for (auto g : this->neighbor()) g.walk(cond, then);
            then(*this);
        }
        constexpr int conn_area() const noexcept {
            int ans = 0;
            auto cond = [&](Grid map) { return map.tile() == this->tile(); };
            auto then = [&](auto) { ans++; };
            this->walk(cond, then);
            return ans;
        }
        static constexpr Grid next(char pat) noexcept {
            for (int y = HEIGHT - 1; y >= 0; y--) for (int x = 0; x < WIDTH; x++)
                if (MAP[x][y] == pat && !Grid(x, y).done()) return Grid(x, y);
            return Grid();
        }
        static constexpr int stat(char pat) noexcept {
            int ans = 0;
            for (int y = 0; y < HEIGHT; y++) for (int x = 0; x < WIDTH; x++)
                if (MAP[x][y] == pat) ans++;
            return ans;
        }
    };

}
