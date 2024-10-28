#include <iostream>
#include <functional>
#include <algorithm>
#include <cmath>

/// @brief LapisLazuli is a collection of utilities for OI.
namespace ll {

    constexpr int fact(int n) noexcept { return n == 0 ? 1 : n * fact(n - 1); }

    class BaseN final {
    public:
        int const base;
        int const num;
        constexpr BaseN(int base, int num) noexcept : base(base), num(num) {}
        class Iterator final {
            int const base;
            int curr;
        public:
            constexpr Iterator(int base, int curr) noexcept : base(base), curr(curr) {}
            constexpr int operator*() const noexcept { return this->curr % this->base; }
            constexpr Iterator& operator++() noexcept { this->curr /= base; return *this; }
            constexpr bool operator!=(Iterator const& rhs) const noexcept { return this->curr != rhs.curr; }
        };
        constexpr Iterator begin() const noexcept { return Iterator(base, num); }
        constexpr Iterator end() const noexcept { return Iterator(base, 0); }
        inline operator std::vector<int>() const noexcept {
            auto res = std::vector<int>();
            for (auto i : *this) res.push_back(i);
            return res;
        }
    };

    template<typename T>
    constexpr void panic(T msg) noexcept {
        std::cerr << msg << std::endl;
        std::abort();
    }

    constexpr void panic() noexcept { panic(""); }

    template<typename T = int>
    class Range final {
        T const left, right;
    public:
        constexpr Range(T left, T right) noexcept : left(left), right(right) {}
        class Iterator final {
            T curr;
        public:
            constexpr Iterator(T curr) noexcept : curr(curr) {}
            constexpr T operator*() const noexcept { return this->curr; }
            constexpr Iterator& operator++() noexcept { ++this->curr; return *this; }
            constexpr bool operator!=(Iterator const& other) const noexcept { return this->curr != other.curr; }
        };
        constexpr Iterator begin() const noexcept { return Iterator(left); }
        constexpr Iterator end() const noexcept { return Iterator(right); }
    };

    template<typename T>
    constexpr Range<T> rng(T left, T right) noexcept { return Range(left, right); }

    template<typename T>
    constexpr Range<T> rng(T term) noexcept { return Range(T(), term); }

    class Permut final {
    public:
        std::vector<int> const el;
        constexpr Permut(std::vector<int> el) noexcept : el(el) {}
        constexpr int cnt() const noexcept { return fact(el.size()); }
        class Iterator final {
        public:
            std::vector<int> const& el;
        private:
            std::vector<int> curr;
            std::vector<int> buf;
            bool next = true;
        public:
            constexpr Iterator(std::vector<int> const& el)
                : el(el), curr(std::vector<int>(el.size())), buf(std::vector<int>(el.size())) {
                for (auto i : rng(el.size())) {
                    this->curr[i] = i;
                    this->buf[i] = el[curr[i]];
                }
            }
            constexpr std::vector<int> const& operator*() const noexcept { return this->buf; }
            constexpr Iterator& operator++() noexcept {
                this->next = std::next_permutation(this->curr.begin(), this->curr.end());
                for (auto i : rng(this->el.size()))  this->buf[i] = this->el[curr[i]];
                return *this;
            }
            constexpr bool operator!=(Iterator const& _) const noexcept { return this->next; }
        };

        constexpr Iterator begin() const noexcept { return Iterator(this->el); }
        constexpr Iterator end() const noexcept { return Iterator(this->el); }
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
            for (auto y : rng(HEIGHT)) for (auto x : rng(WIDTH)) input >> MAP[x][y];
        }
        static inline void output(std::ostream& output) noexcept {
            for (auto y : rng(HEIGHT)) {
                for (auto x : rng(WIDTH))
                    output << MAP[x][y];
                output << std::endl;
            }
        }
        static inline void debug() noexcept {
            std::cerr << std::endl << "┌";
            for (auto _ : rng(WIDTH)) std::cerr << "─";
            std::cerr << WIDTH << std::endl;
            for (auto y : rng(HEIGHT)) {
                std::cerr << "│";
                for (auto x : rng(WIDTH))
                    std::cerr << MAP[x][y];
                std::cerr << std::endl;
            }
            std::cerr << HEIGHT << std::endl << std::endl;
        }
        static constexpr void refresh() noexcept {
            DONE.assign(DONE.size(), false);
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
        constexpr T& tile() const noexcept {
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
        static constexpr Grid next(T const& pat) noexcept {
            for (auto y : rng(HEIGHT)) for (auto x : rng(WIDTH))
                if (MAP[x][y] == pat && !Grid(x, y).done()) return Grid(x, y);
            return Grid();
        }
        static constexpr int stat(T const& pat) noexcept {
            int ans = 0;
            for (auto y : rng(HEIGHT)) for (auto x : rng(WIDTH))
                if (MAP[x][y] == pat) ans++;
            return ans;
        }
    };

}
