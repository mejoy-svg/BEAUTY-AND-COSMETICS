#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cctype>
#include <initializer_list>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;
// This program demonstrates arrays, linked lists, stacks, queues, trees, searching, and sorting.
namespace Config {
    const char* const APP_NAME = "BEAUTY & COSMETICS"; const char* const APP_SUBTITLE = "Inventory, Search & Order Management System";
    const int LOW_STOCK_LEVEL = 5, PAGE_WIDTH = 92;
}
struct Product {
    int id; string name, category, brand; double price; int stockQuantity;
    Product() : id(0), price(0.0), stockQuantity(0) {}
    Product(int i, const string& n, const string& c, const string& b, double p, int s) : id(i), name(n), category(c), brand(b), price(p), stockQuantity(s) {}
};
struct EofException {}; static bool g_ansi = false;
static string cRs, cBd, cDm, cAc, cGd, cCy, cMg, cBl, cWh, cGn, cYl, cRd;
static void initConsole() {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); DWORD mode = 0;
    g_ansi = h != INVALID_HANDLE_VALUE && h != NULL && GetConsoleMode(h, &mode) && SetConsoleMode(h, mode | 0x0004);
#else
    g_ansi = isatty(STDOUT_FILENO) != 0 && getenv("NO_COLOR") == NULL;
#endif
    if (!g_ansi) return;
    cRs = "\033[0m"; cBd = "\033[1m"; cDm = "\033[38;5;250m"; cAc = "\033[38;5;205m"; cGd = "\033[38;5;221m";
    cCy = "\033[96m"; cMg = "\033[95m"; cBl = "\033[94m"; cWh = "\033[97m"; cGn = "\033[92m"; cYl = "\033[93m"; cRd = "\033[91m";
}
static void clearScreen() { cout.flush(); if (g_ansi) { cout << "\033[2J\033[3J\033[H"; cout.flush(); } }
static string toLower(string s) { for (size_t i = 0; i < s.size(); ++i) s[i] = (char)tolower((unsigned char)s[i]); return s; }
static string toUpper(string s) { for (size_t i = 0; i < s.size(); ++i) s[i] = (char)toupper((unsigned char)s[i]); return s; }
static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    return a == string::npos ? "" : s.substr(a, s.find_last_not_of(" \t\r\n") - a + 1);
}
static const string& brandColor(const string& brand) {   // each partner brand has its own color
    string b = toLower(brand);
    return b == "maybelline" ? cMg : b == "l'oreal paris" ? cGd : b == "cetaphil" ? cCy : b == "laneige" ? cBl : cRs;
}
static string col(const string& str, size_t w, bool right = false, const string& color = "") {
    string t = str.size() <= w ? str : (w <= 2 ? str.substr(0, w) : str.substr(0, w - 2) + "..");
    string sp(w > t.size() ? w - t.size() : 0, ' ');
    t = right ? sp + t : t + sp;
    return color.empty() ? t : color + t + cRs;
}
static string money(double v) {
    char buf[64]; snprintf(buf, sizeof(buf), "%.2f", v);
    string s = buf, ip = s.substr(0, s.find('.')), fp = s.substr(s.find('.')), out; int c = 0;
    for (int i = (int)ip.size() - 1; i >= 0; --i) { out.insert(out.begin(), ip[i]); if (++c % 3 == 0 && i > 0 && ip[i - 1] != '-') out.insert(out.begin(), ','); }
    return "PHP " + out + fp;
}
static string itemsText(int n) { return to_string(n) + (n == 1 ? " item" : " items"); }
static void printRule() { cout << "  " << cDm << string(Config::PAGE_WIDTH, '-') << cRs << "\n"; }
static void printOk(const string& m)   { cout << "  " << cGn << "[OK]" << cRs << " " << m << "\n"; } static void printErr(const string& m)  { cout << "  " << cRd << "[!]" << cRs << " " << m << "\n"; }
static void printNote(const string& m) { cout << "  " << cDm << m << cRs << "\n"; }
static void menuItem(const string& num, const string& text) { cout << "    " << cAc << num << cRs << "  " << text << "\n"; }
static void pageTitle(const string& title) {
    clearScreen();
    cout << "\n  " << cBd << cAc << "BEAUTY & COSMETICS" << cRs << cDm << "  /  " << cRs << cBd << title << cRs << "\n";
    printRule(); cout << "\n";
}
static string readLine(const string& prompt) {
    cout << prompt; cout.flush();
    string s; if (!getline(cin, s)) throw EofException();
    return trim(s);
}
static bool parseLong(const string& s, long& out) {
    if (s.empty()) return false;
    char* end = NULL; errno = 0; long v = strtol(s.c_str(), &end, 10);
    if (errno == ERANGE || *end != '\0') return false;
    out = v; return true;
}
static int readInt(const string& prompt, int lo, int hi) {
    while (true) {
        long v; if (parseLong(readLine(prompt), v) && v >= lo && v <= hi) return (int)v;
        printErr("Enter a whole number from " + to_string(lo) + " to " + to_string(hi) + ".");
    }
}
static double readDouble(const string& prompt, double lo, double hi) {
    while (true) {
        string s = readLine(prompt), clean;
        for (size_t i = 0; i < s.size(); ++i) if (s[i] != ',') clean += s[i];   // allows 5,000
        if (!clean.empty()) {
            char* end = NULL; errno = 0; double v = strtod(clean.c_str(), &end);
            if (errno != ERANGE && *end == '\0' && v >= lo && v <= hi) return v;
        }
        printErr("Enter an amount from " + money(lo) + " to " + money(hi) + ".");
    }
}
static string readNonEmpty(const string& prompt) {
    while (true) { string s = readLine(prompt); if (!s.empty()) return s; printErr("This field cannot be empty."); }
}
static bool confirm(const string& prompt) {
    while (true) {
        string s = toLower(readLine(prompt + " (y/n): ")); if (s == "y" || s == "yes") return true;
        if (s == "n" || s == "no") return false;
        printErr("Please answer y or n.");
    }
}
static void pauseScreen() {
    cout << "\n  " << cDm << "Press Enter to continue..." << cRs; cout.flush();
    string tmp; if (!getline(cin, tmp)) throw EofException();
}
// Catalog stores products in a dynamic array. It grows when full and shrinks when mostly empty.
class Catalog {
    Product* data; int count, cap;
    void resize(int newCap) {
        Product* nd = new Product[newCap]; for (int i = 0; i < count; ++i) nd[i] = data[i];
        delete[] data; data = nd; cap = newCap;
    }
public:
    Catalog() : data(new Product[4]), count(0), cap(4) {}
    ~Catalog() { delete[] data; }
    Catalog(const Catalog&) = delete; Catalog& operator=(const Catalog&) = delete;
    int size() const { return count; }
    Product& at(int i) { return data[i]; }
    const Product& at(int i) const { return data[i]; }
    const Product* raw() const { return data; }
    void insertAt(int pos, const Product& p) {
        if (pos < 0 || pos > count) pos = count;
        if (count == cap) resize(cap * 2);
        for (int i = count; i > pos; --i) data[i] = data[i - 1];
        data[pos] = p; ++count;
    }
    void removeAt(int pos) {
        if (pos < 0 || pos >= count) return;
        for (int i = pos; i < count - 1; ++i) data[i] = data[i + 1];
        if (--count, cap > 8 && count <= cap / 4) resize(cap / 2);
    }
};
// The BST quickly finds a product ID and returns its current catalog position.
struct BstNode { int id, index; BstNode *left, *right; };
class ProductBST {
    BstNode* root;
    static void destroy(BstNode* n) { if (n) { destroy(n->left); destroy(n->right); delete n; } }
    static BstNode* build(const Catalog& c, int lo, int hi) {   // middle element becomes the root of each subtree
        if (lo > hi) return NULL;
        int mid = lo + (hi - lo) / 2;
        BstNode* n = new BstNode{c.at(mid).id, mid, NULL, NULL};
        n->left = build(c, lo, mid - 1); n->right = build(c, mid + 1, hi); return n;
    }
public:
    ProductBST() : root(NULL) {}
    ~ProductBST() { destroy(root); }
    ProductBST(const ProductBST&) = delete; ProductBST& operator=(const ProductBST&) = delete;
    int search(int id) const {   // catalog index or -1
        const BstNode* cur = root;
        while (cur && cur->id != id) cur = id < cur->id ? cur->left : cur->right;
        return cur ? cur->index : -1;
    }
    void rebuild(const Catalog& cat) { destroy(root); root = build(cat, 0, cat.size() - 1); }   // catalog is always ordered by ID
};
// The cart uses previous/next links; the wishlist uses a simple next link.
struct CartNode { int productId; string name, brand; double price; int qty; CartNode *prev, *next; };
class ShoppingCart {
    CartNode *head, *tail, *current; int count;
public:
    ShoppingCart() : head(NULL), tail(NULL), current(NULL), count(0) {}
    ~ShoppingCart() { clear(); }
    ShoppingCart(const ShoppingCart&) = delete; ShoppingCart& operator=(const ShoppingCart&) = delete;
    bool empty() const { return count == 0; }
    int size() const { return count; }
    CartNode* first() const { return head; }
    CartNode* cursor() const { return current; }
    CartNode* find(int id) const { for (CartNode* n = head; n; n = n->next) if (n->productId == id) return n; return NULL; }
    void addItem(const Product& p, int qty) {
        CartNode* ex = find(p.id);
        if (ex) { ex->qty += qty; ex->price = p.price; current = ex; return; }
        CartNode* n = new CartNode{p.id, p.name, p.brand, p.price, qty, tail, NULL};
        if (tail) tail->next = n; else head = n;
        tail = current = n; ++count;
    }
    bool moveNext() { if (current && current->next) { current = current->next; return true; } return false; }
    bool movePrev() { if (current && current->prev) { current = current->prev; return true; } return false; }
    void removeNode(CartNode* v) {
        if (v->prev) v->prev->next = v->next; else head = v->next;
        if (v->next) v->next->prev = v->prev; else tail = v->prev;
        current = v->next ? v->next : v->prev; delete v; --count;
    }
    int positionOfCursor() const { int pos = 1; for (CartNode* n = head; n && n != current; n = n->next) ++pos; return pos; }
    int units() const { int u = 0; for (CartNode* n = head; n; n = n->next) u += n->qty; return u; }
    double total() const { double t = 0; for (CartNode* n = head; n; n = n->next) t += n->price * n->qty; return t; }
    void clear() { while (head) { CartNode* nx = head->next; delete head; head = nx; } tail = current = NULL; count = 0; }
};
struct WishNode { int productId; string name; WishNode* next; };
class Wishlist {
    WishNode* head; int count;
public:
    Wishlist() : head(NULL), count(0) {}
    ~Wishlist() { while (head) { WishNode* nx = head->next; delete head; head = nx; } }
    Wishlist(const Wishlist&) = delete; Wishlist& operator=(const Wishlist&) = delete;
    int size() const { return count; }
    WishNode* first() const { return head; }
    bool contains(int id) const { for (WishNode* n = head; n; n = n->next) if (n->productId == id) return true; return false; }
    bool add(int id, const string& name) {   // append at tail, no duplicates
        if (contains(id)) return false;
        WishNode** link = &head; while (*link) link = &(*link)->next;
        *link = new WishNode{id, name, NULL}; ++count; return true;
    }
    void remove(int id) {
        for (WishNode** link = &head; *link; link = &(*link)->next)
            if ((*link)->productId == id) { WishNode* d = *link; *link = d->next; delete d; --count; return; }
    }
};
// The banner list is circular, so the last brand links back to the first.
struct BannerNode { string brand, tagline; BannerNode* next; };
class BannerRing {
    BannerNode* tail; int count;
public:
    BannerRing() : tail(NULL), count(0) {}
    ~BannerRing() {
        if (!tail) return;
        BannerNode* cur = tail->next; tail->next = NULL;   // break the circle first
        while (cur) { BannerNode* nx = cur->next; delete cur; cur = nx; }
    }
    BannerRing(const BannerRing&) = delete; BannerRing& operator=(const BannerRing&) = delete;
    int size() const { return count; }
    BannerNode* head() const { return tail ? tail->next : NULL; }
    void add(const string& brand, const string& tagline) {
        BannerNode* n = new BannerNode{brand, tagline, NULL};
        if (!tail) n->next = n; else { n->next = tail->next; tail->next = n; }
        tail = n; ++count;
    }
    const BannerNode* at(int i) const { BannerNode* cur = head(); for (int k = 0; cur && k < i; ++k) cur = cur->next; return cur; }
};
// The stack follows LIFO: the most recent inventory action is undone first.
enum ActionType { ACT_ADD, ACT_REMOVE, ACT_STOCK, ACT_PRICE, ACT_ORDER };
struct Action {
    ActionType type; string desc; Product prod;
    int pos, id, delta; double oldPrice, newPrice;
    int n; int *ids, *qtys;   // ORDER: deducted lines (owned)
    Action() : type(ACT_ADD), pos(0), id(0), delta(0), oldPrice(0), newPrice(0), n(0), ids(NULL), qtys(NULL) {}
    ~Action() { delete[] ids; delete[] qtys; }
    Action(const Action&) = delete; Action& operator=(const Action&) = delete;
};
struct StackNode { Action* act; StackNode* next; };
class ActionStack {
    StackNode* topNode; int count;
public:
    ActionStack() : topNode(NULL), count(0) {}
    ~ActionStack() { clear(); }
    ActionStack(const ActionStack&) = delete; ActionStack& operator=(const ActionStack&) = delete;
    bool empty() const { return topNode == NULL; }
    int size() const { return count; }
    Action* peek() const { return topNode ? topNode->act : NULL; }
    StackNode* top() const { return topNode; }
    void push(Action* a) { topNode = new StackNode{a, topNode}; ++count; }
    Action* pop() { StackNode* n = topNode; Action* a = n->act; topNode = n->next; delete n; --count; return a; }
    void clear() { while (topNode) delete pop(); }
};
// The checkout queue follows FIFO: the earliest order is processed first.
struct OrderLine { int productId; string name; int qty; double unitPrice; };
struct Order {
    int orderNo; string customer; int n; OrderLine* lines;
    Order() : orderNo(0), n(0), lines(NULL) {}
    ~Order() { delete[] lines; }
    Order(const Order&) = delete; Order& operator=(const Order&) = delete;
    double total() const { double t = 0; for (int i = 0; i < n; ++i) t += lines[i].unitPrice * lines[i].qty; return t; }
    int units() const { int u = 0; for (int i = 0; i < n; ++i) u += lines[i].qty; return u; }
};
struct QueueNode { Order* order; QueueNode* next; };
class OrderQueue {
    QueueNode *front, *rear; int count;
public:
    OrderQueue() : front(NULL), rear(NULL), count(0) {}
    ~OrderQueue() { while (front) delete dequeue(); }
    OrderQueue(const OrderQueue&) = delete; OrderQueue& operator=(const OrderQueue&) = delete;
    bool empty() const { return front == NULL; }
    int size() const { return count; }
    QueueNode* head() const { return front; }
    void enqueue(Order* o) {
        QueueNode* n = new QueueNode{o, NULL};
        if (rear) rear->next = n; else front = n;
        rear = n; ++count;
    }
    Order* dequeue() { QueueNode* n = front; Order* o = n->order; front = n->next; if (!front) rear = NULL; delete n; --count; return o; }
};
// Products can be sorted by price, stock, or brand using either algorithm.
enum SortKey { KEY_PRICE = 1, KEY_STOCK = 2, KEY_BRAND = 3 };
static int cmp3(double a, double b) { return a < b ? -1 : (a > b ? 1 : 0); }
static bool comesBefore(const Product& a, const Product& b, int key, bool desc) {
    int c;
    if (key == KEY_PRICE) c = cmp3(a.price, b.price);
    else if (key == KEY_STOCK) c = cmp3(a.stockQuantity, b.stockQuantity);
    else { c = toLower(a.brand).compare(toLower(b.brand)); if (c == 0) c = toLower(a.name).compare(toLower(b.name)); c = cmp3(c, 0); }
    if (c == 0) c = cmp3(a.id, b.id);
    return desc ? c > 0 : c < 0;
}
static void insertionSort(Product* a, int n, int key, bool desc) {
    for (int i = 1; i < n; ++i) {
        Product k = a[i]; int j = i - 1;
        while (j >= 0 && comesBefore(k, a[j], key, desc)) { a[j + 1] = a[j]; --j; }
        a[j + 1] = k;
    }
}
static void swapProducts(Product& x, Product& y) { Product t = x; x = y; y = t; }
static void quickSort(Product* a, int lo, int hi, int key, bool desc) {
    while (lo < hi) {
        swapProducts(a[lo + (hi - lo) / 2], a[hi]);   // middle element as pivot
        Product pivot = a[hi]; int i = lo;
        for (int j = lo; j < hi; ++j) if (comesBefore(a[j], pivot, key, desc)) { swapProducts(a[i], a[j]); ++i; }
        swapProducts(a[i], a[hi]);
        if (i - lo < hi - i) { quickSort(a, lo, i - 1, key, desc); lo = i + 1; }   // recurse smaller half
        else                 { quickSort(a, i + 1, hi, key, desc); hi = i - 1; }
    }
}
static void printProductTable(const Product* a, int n) {
    const string g = "  ";
    cout << "  " << cBd << col("ID", 4, true) << g << col("Product", 29) << g << col("Brand", 13) << g << col("Category", 11)
         << g << col("Unit Price", 12, true) << g << col("Stock", 5, true) << g << col("Status", 6) << cRs << "\n";
    printRule();
    for (int i = 0; i < n; ++i) {
        const Product& p = a[i]; int s = p.stockQuantity;
        string st = s <= 0 ? "OUT" : (s <= Config::LOW_STOCK_LEVEL ? "LOW" : "OK");
        cout << "  " << col(to_string(p.id), 4, true) << g << col(p.name, 29) << g << col(p.brand, 13, false, brandColor(p.brand)) << g
             << col(p.category, 11) << g << col(money(p.price), 12, true) << g << col(to_string(s), 5, true) << g
             << col(st, 6, false, st == "OK" ? cGn : (st == "LOW" ? cYl : cRd)) << "\n";
    }
    printRule();
}
// ============================================================================
// APPLICATION
// ============================================================================
class StoreApp {
    Catalog catalog; ProductBST bst; BannerRing brands; ShoppingCart cart; Wishlist wishlist;
    OrderQueue orders; ActionStack undoStack, redoStack;
    int nextProductId, nextOrderNo;
    Product* findProduct(int id) { int idx = bst.search(id); return idx < 0 ? NULL : &catalog.at(idx); }
    Product* newTable() { return new Product[catalog.size() > 0 ? catalog.size() : 1]; }
    void showAllProducts() {
        if (catalog.size() == 0) cout << "  The catalog is empty.\n"; else printProductTable(catalog.raw(), catalog.size());
        cout << "\n";
    }
    Product* askProduct(const string& prompt) {   // NULL when cancelled
        while (true) {
            int id = readInt(prompt + " (0 = cancel): ", 0, 2000000000); if (id == 0) return NULL;
            Product* p = findProduct(id); if (p) return p;
            printErr("No product with ID " + to_string(id) + ".");
        }
    }
    bool chooseBrand(string& out) {
        cout << "  Official partner brands\n";
        for (int i = 0; i < brands.size(); ++i) menuItem(to_string(i + 1), brandColor(brands.at(i)->brand) + brands.at(i)->brand + cRs);
        cout << "\n";
        while (true) {
            long v;
            if (parseLong(readLine("  Select brand number (0 = cancel): "), v)) {
                if (v == 0) return false;
                if (v >= 1 && v <= brands.size()) { out = brands.at((int)v - 1)->brand; return true; }
            }
            printErr("Not a partner brand. Try again.");
        }
    }
    // Prints a numbered menu (0 = last label) and returns the choice.
    int showMenu(const string& title, initializer_list<const char*> items) {
        pageTitle(title); int k = 1, n = (int)items.size();
        for (const char* it : items) { if (k < n) menuItem(to_string(k), it); else menuItem("0", it); ++k; }
        cout << "\n"; return readInt("  Choose: ", 0, n - 1);
    }
    void seed() {
        brands.add("Maybelline", "Bold color, big lashes, all-day wear");
        brands.add("L'Oreal Paris", "Science-backed skin, hair and color care");
        brands.add("Cetaphil", "Gentle everyday care for sensitive skin"); brands.add("Laneige", "Hydration-first Korean skin care");
        struct Seed { const char *n, *c, *b; double p; int s; };
        static const Seed items[] = {
            {"Super Stay Matte Ink Lipstick", "Lips", "Maybelline", 399, 60}, {"Fit Me Matte Foundation", "Face", "Maybelline", 449, 45},
            {"Sky High Mascara", "Eyes", "Maybelline", 549, 38}, {"Instant Age Rewind Concealer", "Face", "Maybelline", 499, 27},
            {"Brow Ultra Slim Pencil", "Eyes", "Maybelline", 349, 4}, {"Revitalift Hyaluronic Serum", "Serum", "L'Oreal Paris", 1099, 22},
            {"Elvive Total Repair 5 Shampoo", "Hair Care", "L'Oreal Paris", 289, 55}, {"UV Perfect Sunscreen SPF50", "Sun Care", "L'Oreal Paris", 599, 30},
            {"Infallible 24H Foundation", "Face", "L'Oreal Paris", 799, 18}, {"Color Riche Satin Lipstick", "Lips", "L'Oreal Paris", 699, 12},
            {"Gentle Skin Cleanser 236ml", "Cleanser", "Cetaphil", 499, 70}, {"Moisturizing Cream 250g", "Moisturizer", "Cetaphil", 749, 33},
            {"Daily Moisturizer SPF35", "Moisturizer", "Cetaphil", 899, 9}, {"Oily Skin Cleanser 236ml", "Cleanser", "Cetaphil", 549, 41},
            {"Moisturizing Lotion 473ml", "Moisturizer", "Cetaphil", 799, 2}, {"Lip Sleeping Mask", "Lips", "Laneige", 899, 40},
            {"Water Bank Blue HA Cream", "Moisturizer", "Laneige", 1799, 14}, {"Cream Skin Refiner Toner", "Toner", "Laneige", 1699, 11},
            {"Neo Cushion Matte", "Face", "Laneige", 1899, 8}, {"Water Sleeping Mask", "Mask", "Laneige", 1499, 16}
        }; for (int i = 0; i < 20; ++i) catalog.insertAt(i, Product(101 + i, items[i].n, items[i].c, items[i].b, items[i].p, items[i].s));
        nextProductId = 121; bst.rebuild(catalog);
    }
    bool doInsert(const Product& p, int pos, string& msg) {
        if (bst.search(p.id) >= 0) { msg = "Product ID already exists."; return false; }
        catalog.insertAt(pos, p); bst.rebuild(catalog); return true;
    }
    bool doRemove(int id, string& msg) {
        int idx = bst.search(id);
        if (idx < 0) { msg = "Product no longer exists."; return false; }
        catalog.removeAt(idx); bst.rebuild(catalog); return true;
    }
    bool applyAction(Action& a, bool forward, string& msg) {   // forward = redo, backward = undo
        if (a.type == ACT_ADD) return forward ? doInsert(a.prod, a.pos, msg) : doRemove(a.prod.id, msg);
        if (a.type == ACT_REMOVE) return forward ? doRemove(a.prod.id, msg) : doInsert(a.prod, a.pos, msg);
        if (a.type == ACT_STOCK || a.type == ACT_PRICE) {
            Product* p = findProduct(a.id);
            if (!p) { msg = "Product no longer exists."; return false; }
            if (a.type == ACT_PRICE) { p->price = forward ? a.newPrice : a.oldPrice; return true; }
            int ns = p->stockQuantity + (forward ? a.delta : -a.delta);
            if (ns < 0) { msg = "Result would make stock negative."; return false; }
            p->stockQuantity = ns; return true;
        }
        int sign = forward ? -1 : 1;   // ACT_ORDER: validate every line first, then apply
        for (int i = 0; i < a.n; ++i) {
            Product* p = findProduct(a.ids[i]);
            if (!p) { msg = "A product in this order no longer exists."; return false; }
            if (p->stockQuantity + sign * a.qtys[i] < 0) { msg = "Not enough stock to re-apply this order."; return false; }
        }
        for (int i = 0; i < a.n; ++i) findProduct(a.ids[i])->stockQuantity += sign * a.qtys[i];
        return true;
    }
    void recordAction(Action* a) { undoStack.push(a); redoStack.clear(); }   // a new action clears redo
    void moveAction(ActionStack& from, ActionStack& to, bool forward) {      // shared by undo and redo
        const string name = forward ? "redo" : "undo";
        if (from.empty()) { cout << "  Nothing to " << name << ". The " << name << " stack is empty.\n"; return; }
        Action* a = from.peek(); string msg;
        if (applyAction(*a, forward, msg)) {
            to.push(from.pop());
            cout << "  " << cYl << (forward ? "[REDO]" : "[UNDO]") << cRs << (forward ? " Re-applied: " : " Reverted: ") << a->desc << "\n";
        } else printErr("Cannot " + name + " \"" + a->desc + "\": " + msg);
    }
    static void printStack(const ActionStack& s, const string& title) {
        cout << "  " << title << "  -  " << s.size() << " action(s)\n"; if (s.empty()) cout << "    (empty)\n";
        int k = 1;
        for (StackNode* n = s.top(); n; n = n->next, ++k) cout << "    " << (k == 1 ? "TOP > " : "      ") << k << ". " << n->act->desc << "\n";
    }
    Action* newAction(ActionType t, const string& desc) { Action* a = new Action; a->type = t; a->desc = desc; return a; }
    void adminAddProduct() {
        string name = readNonEmpty("  Product name: "), brand; cout << "\n";
        if (!chooseBrand(brand)) { cout << "  Cancelled.\n"; return; }
        cout << "\n"; string cat = readNonEmpty("  Category (e.g. Lips, Face, Eyes, Serum, Hair Care): ");
        double price = readDouble("  Unit price (PHP): ", 0.01, 99999.99);
        int stock = readInt("  Initial stock (0-100000): ", 0, 100000); Product p(nextProductId, name, cat, brand, price, stock);
        string msg; int pos = catalog.size();
        if (!doInsert(p, pos, msg)) { printErr(msg); return; }
        ++nextProductId; Action* a = newAction(ACT_ADD, "Added product #" + to_string(p.id) + " " + p.name);
        a->prod = p; a->pos = pos; recordAction(a); cout << "\n"; printOk(a->desc + " (use Undo to reverse).");
    }
    void adminEditProduct(bool price) {   // true = change price, false = adjust stock
        showAllProducts(); Product* p = askProduct(price ? "  Product ID to reprice" : "  Product ID to restock / adjust");
        if (!p) { cout << "  Cancelled.\n"; return; }
        string tag = "#" + to_string(p->id) + " " + p->name;
        if (price) {
            cout << "  " << p->name << "  current price: " << money(p->price) << "\n\n";
            double np = readDouble("  New unit price (PHP): ", 0.01, 99999.99);
            Action* a = newAction(ACT_PRICE, "Price " + tag + ": " + money(p->price) + " -> " + money(np));
            a->id = p->id; a->oldPrice = p->price; a->newPrice = np; p->price = np; recordAction(a);
            cout << "\n"; printOk("Price updated."); return;
        }
        cout << "  " << p->name << " (" << p->brand << ")  current stock: " << p->stockQuantity << "\n\n";
        int delta = readInt("  Change in stock (+add / -remove): ", -100000, 100000);
        if (delta == 0) { cout << "  No change made.\n"; return; }
        if (p->stockQuantity + delta < 0) { printErr("Stock cannot go below zero."); return; }
        p->stockQuantity += delta;
        Action* a = newAction(ACT_STOCK, "Stock " + string(delta > 0 ? "+" : "") + to_string(delta) + " for " + tag);
        a->id = p->id; a->delta = delta; recordAction(a);
        cout << "\n"; printOk("New stock for " + p->name + ": " + to_string(p->stockQuantity));
    }
    void adminRemoveProduct() {
        showAllProducts(); Product* p = askProduct("  Product ID to remove");
        if (!p) { cout << "  Cancelled.\n"; return; }
        if (!confirm("  Remove \"" + p->name + "\" from the catalog?")) { cout << "  Cancelled.\n"; return; }
        Action* a = newAction(ACT_REMOVE, "Removed product #" + to_string(p->id) + " " + p->name);
        a->prod = *p; a->pos = bst.search(p->id); string msg;
        if (!doRemove(p->id, msg)) { printErr(msg); delete a; return; }
        recordAction(a); cout << "\n"; printOk(a->desc + " (use Undo to restore it).");
    }
    void adminMenu() {
        while (true) {
            int ch = showMenu("Admin Panel  -  undo " + to_string(undoStack.size()) + ", redo " + to_string(redoStack.size()),
                {"Undo last inventory action", "Redo last undone action", "View undo / redo history", "Add new product",
                 "Restock / adjust stock", "Change price", "Remove product", "Back to main menu"}); if (ch == 0) return;
            pageTitle("Admin Panel");
            switch (ch) {
            case 1: moveAction(undoStack, redoStack, false); break;   
            case 2: moveAction(redoStack, undoStack, true); break;
            case 3: printStack(undoStack, "Undo stack  (top = most recent)"); cout << "\n"; printStack(redoStack, "Redo stack"); break;
            case 4: adminAddProduct(); break;       
            case 5: adminEditProduct(false); break;
            case 6: adminEditProduct(true); break;  
            case 7: adminRemoveProduct(); break;
            }
            pauseScreen();
        }
    }
    void viewCatalog() {
        pageTitle("Catalog"); showAllProducts();
        cout << "  " << catalog.size() << " product(s) from " << brands.size() << " partner brands\n";
    }
    void filterByBrand() {
        pageTitle("Filter by Brand"); string brand;
        if (!chooseBrand(brand)) { cout << "  Cancelled.\n"; return; }
        Product* tmp = newTable(); int m = 0, units = 0;
        for (int i = 0; i < catalog.size(); ++i)
            if (toLower(catalog.at(i).brand) == toLower(brand)) { tmp[m++] = catalog.at(i); units += catalog.at(i).stockQuantity; }
        pageTitle("Filter by Brand  /  " + brand); if (m == 0) cout << "  No products found for this brand.\n";
        else { printProductTable(tmp, m); cout << "  " << m << " product(s), " << units << " unit(s) in stock\n"; }
        delete[] tmp;
    }
    // ------------------------------------------------------------ MENU 3: search (Lab 7 linear keyword, Lab 6 BST by ID)
    static bool matchesAllWords(const Product& p, const string& query) {
        string hay = toLower(p.name + " " + p.brand + " " + p.category); size_t i = 0;
        while (i < query.size()) {
            while (i < query.size() && isspace((unsigned char)query[i])) ++i;
            size_t j = i; while (j < query.size() && !isspace((unsigned char)query[j])) ++j;
            if (j > i && hay.find(query.substr(i, j - i)) == string::npos) return false;
            i = j;
        }
        return true;
    }
    void keywordSearch() {
        int ch = showMenu("Search Products", {"Search by keyword", "Search by Product ID", "Back"});
        if (ch == 0) return;
        if (ch == 2) {
            pageTitle("Search Products  /  Product ID"); int id = readInt("  Enter Product ID: ", 1, 2000000000);
            Product* p = findProduct(id); pageTitle("Search Result  /  Product ID " + to_string(id));
            if (p) printProductTable(p, 1); else cout << "  No product with ID " << id << ".\n";
            return;
        }
        string q = toLower(readNonEmpty("  Keyword(s) (name, brand or category): ")); Product* tmp = newTable(); int m = 0;
        for (int i = 0; i < catalog.size(); ++i) if (matchesAllWords(catalog.at(i), q)) tmp[m++] = catalog.at(i);
        pageTitle("Search Result  /  \"" + q + "\""); if (m == 0) cout << "  No products matched \"" << q << "\".\n";
        else { printProductTable(tmp, m); cout << "  " << m << " match(es)\n"; }
        delete[] tmp;
    }
    // ------------------------------------------------------------ MENU 4: price range (Lab 7 binary search on sorted copy)
    void priceRangeFilter() {
        pageTitle("Filter by Price Range");
        if (catalog.size() == 0) { cout << "  The catalog is empty.\n"; return; }
        double lo = readDouble("  Minimum price (PHP): ", 0.0, 99999.99), hi = readDouble("  Maximum price (PHP): ", 0.0, 99999.99);
        if (hi < lo) { double t = lo; lo = hi; hi = t; }   // swap if entered in the wrong order
        int n = catalog.size(); Product* sorted = new Product[n]; for (int i = 0; i < n; ++i) sorted[i] = catalog.at(i);
        insertionSort(sorted, n, KEY_PRICE, false);
        int left = 0, right = n;   // lower bound: first price >= lo
        while (left < right) { int mid = left + (right - left) / 2; if (sorted[mid].price < lo) left = mid + 1; else right = mid; }
        int first = left; right = n;   // upper bound: first price > hi
        while (left < right) { int mid = left + (right - left) / 2; if (sorted[mid].price <= hi) left = mid + 1; else right = mid; }
        pageTitle("Price Range  /  " + money(lo) + " to " + money(hi));
        if (left <= first) cout << "  No products found in this price range.\n";
        else { printProductTable(sorted + first, left - first); cout << "  " << (left - first) << " match(es)\n"; }
        delete[] sorted;
    }
    void showCart() {
        cout << "  Cart  -  " << itemsText(cart.units()) << "\n\n";
        if (cart.empty()) { cout << "  The cart is empty.\n"; return; }
        const string g = "  ";
        cout << "  " << cBd << col("", 3) << col("ID", 4, true) << g << col("Product", 29) << g << col("Brand", 13) << g
             << col("Unit Price", 12, true) << g << col("Qty", 3, true) << g << col("Subtotal", 12, true) << cRs << "\n";
        printRule(); int k = 1;
        for (CartNode* n = cart.first(); n; n = n->next, ++k)
            cout << "  " << cAc << (n == cart.cursor() ? ">" : " ") << cRs << col(to_string(k), 2, true) << col(to_string(n->productId), 4, true) << g
                 << col(n->name, 29) << g << col(n->brand, 13, false, brandColor(n->brand)) << g << col(money(n->price), 12, true) << g
                 << col(to_string(n->qty), 3, true) << g << col(money(n->price * n->qty), 12, true) << "\n"; printRule();
        cout << "  " << cBd << "Total: " << money(cart.total()) << cRs << "\n";
    }
    void cartAddItem() {
        string msg;
        while (true) {
            pageTitle("Cart  /  Add Item"); printProductTable(catalog.raw(), catalog.size());
            printNote("Cart: " + itemsText(cart.units()) + "   |   total " + money(cart.total())); cout << "\n";
            if (!msg.empty()) { cout << msg << "\n\n"; msg.clear(); }
            int id = readInt("  Product ID to add (0 = back): ", 0, 2000000000); if (id == 0) return;
            Product* p = findProduct(id);
            if (!p) { msg = "  " + cRd + "[!]" + cRs + " No product with ID " + to_string(id) + "."; continue; }
            CartNode* ex = cart.find(p->id); int inCart = ex ? ex->qty : 0, room = p->stockQuantity - inCart;
            if (room <= 0) { msg = "  " + cRd + "[!]" + cRs + " No more units of " + p->name + " available to add."; continue; }
            cout << "\n  " << cBd << p->name << cRs << " (" << p->brand << ")  " << money(p->price) << "  |  stock " << p->stockQuantity;
            if (inCart) cout << "  |  already in cart: " << inCart;
            cout << "\n\n"; int q = readInt("  Quantity (1-" + to_string(room) + ", 0 = cancel): ", 0, room); if (q == 0) continue;
            cart.addItem(*p, q); msg = "  " + cGn + "[OK]" + cRs + " Added " + to_string(q) + " x " + p->name + " to the cart.";
        }
    }
    void cartNavigate() {   // walks the doubly linked list with Next / Previous
        string note;
        while (!cart.empty()) {
            pageTitle("Cart  /  Navigate"); showCart(); CartNode* c = cart.cursor();
            cout << "\n  Selected item " << cart.positionOfCursor() << " of " << cart.size() << ": " << cBd << c->name << cRs
                 << "  (" << c->qty << " x " << money(c->price) << ")\n";
            if (!note.empty()) { cout << "\n  " << cYl << note << cRs << "\n"; note.clear(); }
            cout << "\n"; string s = toLower(readLine("  [N]ext  [P]revious  [Q]uantity  [R]emove  [B]ack: "));
            if (s == "n") { if (!cart.moveNext()) note = "Already at the last item."; }
            else if (s == "p") { if (!cart.movePrev()) note = "Already at the first item."; }
            else if (s == "r") { note = "Removed " + c->name + " from the cart."; cart.removeNode(c); }
            else if (s == "q") {
                Product* p = findProduct(c->productId);
                if (!p || p->stockQuantity <= 0) { note = "Product unavailable; it was removed from the cart."; cart.removeNode(c); continue; }
                c->qty = readInt("  New quantity (1-" + to_string(p->stockQuantity) + "): ", 1, p->stockQuantity);
                note = "Quantity updated.";
            }
            else if (s == "b") return;
            else note = "Please type N, P, Q, R or B.";
        }
        pageTitle("Cart  /  Navigate"); cout << "  The cart is empty.\n"; pauseScreen();
    }
    void cartPlaceOrder() {
        if (cart.empty()) { cout << "  The cart is empty. Add items first.\n"; return; }
        for (CartNode* n = cart.first(); n; ) {   // validate every item before creating the order
            CartNode* nx = n->next; Product* p = findProduct(n->productId);
            if (!p) { printErr("\"" + n->name + "\" is no longer sold; removed from cart."); cart.removeNode(n); }
            else if (n->qty > p->stockQuantity) {
                printErr("Only " + to_string(p->stockQuantity) + " unit(s) of \"" + n->name + "\" in stock. Reduce the quantity first (Navigate cart > Quantity).");
                return;
            }
            else if (n->price != p->price) { printNote("Price of \"" + n->name + "\" changed to " + money(p->price) + "."); n->price = p->price; }
            n = nx;
        }
        if (cart.empty()) { cout << "  The cart is now empty.\n"; return; }
        cout << "\n"; showCart(); cout << "\n"; string cust = readLine("  Customer name (Enter = Guest): ");
        if (cust.empty()) cust = "Guest";
        if (!confirm("  Place this order in the checkout queue?")) { cout << "  Order not placed.\n"; return; }
        Order* o = new Order; o->orderNo = nextOrderNo++; o->customer = cust; o->n = cart.size(); o->lines = new OrderLine[o->n]; int i = 0;
        for (CartNode* n = cart.first(); n; n = n->next, ++i) o->lines[i] = OrderLine{n->productId, n->name, n->qty, n->price};
        int no = o->orderNo; double tot = o->total(); orders.enqueue(o); cart.clear(); cout << "\n";
        printOk("Order #" + to_string(no) + " for " + cust + " (" + money(tot) + ") joined the checkout queue. Position in line: " + to_string(orders.size()) + ".");
    }
    void wishlistAction(int ch) {   // 1 = add, 2 = remove, 3 = move to cart
        if (ch == 1) {
            pageTitle("Wishlist  /  Add Product"); showAllProducts(); Product* p = askProduct("  Product ID to wishlist");
            if (!p) cout << "  Cancelled.\n";
            else if (wishlist.add(p->id, p->name)) printOk("Added to wishlist.");
            else printErr("Already on your wishlist.");
            return;
        }
        int id = readInt("  Product ID (0 = cancel): ", 0, 2000000000);
        if (id == 0) { cout << "  Cancelled.\n"; return; }
        if (!wishlist.contains(id)) { printErr("That product is not on the wishlist."); return; }
        Product* p = findProduct(id); CartNode* ex = p ? cart.find(id) : NULL;
        if (ch == 2) { wishlist.remove(id); printOk("Removed from wishlist."); }
        else if (!p) { printErr("Product no longer sold; removed from wishlist."); wishlist.remove(id); }
        else if (p->stockQuantity - (ex ? ex->qty : 0) <= 0) printErr("Out of stock; cannot move to cart.");
        else { cart.addItem(*p, 1); wishlist.remove(id); printOk("Moved to cart (qty 1)."); }
    }
    void wishlistMenu() {
        while (true) {
            pageTitle("Cart  /  Wishlist"); if (wishlist.size() == 0) cout << "  Your wishlist is empty.\n";
            int k = 1;
            for (WishNode* n = wishlist.first(); n; n = n->next, ++k) {
                Product* p = findProduct(n->productId);
                cout << "    " << k << ". #" << n->productId << " " << n->name
                     << (p ? "  -  " + money(p->price) + ", stock " + to_string(p->stockQuantity) : string("  -  (no longer sold)")) << "\n";
            }
            cout << "\n";
            menuItem("1", "Add product"); menuItem("2", "Remove product"); menuItem("3", "Move to cart"); menuItem("0", "Back");
            cout << "\n"; int ch = readInt("  Choose: ", 0, 3); if (ch == 0) return;
            cout << "\n"; if (ch != 1 && wishlist.size() == 0) cout << "  The wishlist is empty.\n"; else wishlistAction(ch);
            pauseScreen();
        }
    }
    void cartMenu() {
        while (true) {
            int ch = showMenu("Shopping Cart  -  " + itemsText(cart.units()),
                {"Add item to cart", "View cart", "Navigate cart", "Place order", "Wishlist", "Back to main menu"});
            if (ch == 0) return;
            if (ch == 1) cartAddItem();
            else if (ch == 5) wishlistMenu();
            else if (ch == 3 && !cart.empty()) cartNavigate();
            else {
                pageTitle(ch == 4 ? "Cart  /  Place Order" : "Cart  /  View"); if (ch == 4) cartPlaceOrder(); else showCart();
                pauseScreen();
            }
        }
    }
    void showQueue() {
        cout << "  " << orders.size() << " order(s) waiting (first in, first out)\n\n";
        if (orders.empty()) { cout << "    (no pending orders)\n"; return; }
        int k = 1;
        for (QueueNode* n = orders.head(); n; n = n->next, ++k) {
            Order* o = n->order;
            cout << "    " << (k == 1 ? "FRONT > " : "        ") << k << ". Order #" << o->orderNo << "  -  " << o->customer
                 << "  -  " << itemsText(o->units()) << "  -  " << money(o->total()) << "\n";
            for (int i = 0; i < o->n; ++i) cout << "              " << cDm << o->lines[i].qty << " x " << o->lines[i].name << cRs << "\n";
        }
    }
    void processNextOrder() {
        if (orders.empty()) { cout << "  The checkout queue is empty. Nothing to process.\n"; return; }
        Order* o = orders.dequeue(); cout << "  " << cBd << "Order #" << o->orderNo << cRs << "  -  " << o->customer << "\n";
        printRule(); int* ids = new int[o->n]; int* qtys = new int[o->n]; int done = 0; double charged = 0;
        for (int i = 0; i < o->n; ++i) {
            const OrderLine& l = o->lines[i]; Product* p = findProduct(l.productId);
            cout << "  " << col(to_string(l.qty), 3, true) << " x " << col(l.name, 29) << "  ";
            if (!p) cout << cRd << "UNAVAILABLE" << cRs << "  (removed from catalog)\n";
            else if (p->stockQuantity < l.qty) cout << cRd << "OUT OF STOCK" << cRs << "  (requested " << l.qty << ", available " << p->stockQuantity << ")\n";
            else {
                p->stockQuantity -= l.qty; ids[done] = l.productId; qtys[done] = l.qty; ++done; charged += l.unitPrice * l.qty;
                cout << col(money(l.unitPrice * l.qty), 12, true) << "  " << cGn << "FULFILLED" << cRs << "\n";
            }
        }
        printRule(); string status = (done == o->n) ? "COMPLETED" : (done == 0 ? "REJECTED" : "PARTIALLY FULFILLED");
        cout << "  Status: " << (done == o->n ? cGn : (done == 0 ? cRd : cYl)) << status << cRs << "      Amount charged: " << cBd << money(charged) << cRs << "\n\n";
        if (done > 0) {
            Action* a = newAction(ACT_ORDER, "Order #" + to_string(o->orderNo) + " processed (" + money(charged) + ")");
            a->n = done; a->ids = ids; a->qtys = qtys;   // Action now owns the arrays
            recordAction(a);
        } else { delete[] ids; delete[] qtys; }
        delete o;
    }
    void checkoutMenu() {
        while (true) {
            int ch = showMenu("Checkout Queue  -  " + to_string(orders.size()) + " pending",
                {"View pending orders", "Process next order", "Process all pending orders", "Back to main menu"});
            if (ch == 0) return;
            pageTitle("Checkout Queue"); if (ch == 1) showQueue();
            else if (ch == 2) processNextOrder();
            else { if (orders.empty()) cout << "  The checkout queue is empty. Nothing to process.\n"; while (!orders.empty()) processNextOrder(); }
            pauseScreen();
        }
    }
    void bannerShowcase() {
        BannerNode* cur = brands.head();
        while (cur) {
            pageTitle("Partner Brands");
            cout << "  " << cBd << brandColor(cur->brand) << toUpper(cur->brand) << cRs << "\n  " << cDm << cur->tagline << cRs << "\n\n";
            int shown = 0;
            for (int i = 0; i < catalog.size(); ++i) {
                const Product& p = catalog.at(i);
                if (toLower(p.brand) == toLower(cur->brand) && p.stockQuantity > 0) {
                    cout << "    " << col(p.name, 30)
                    << col(money(p.price), 14, true)
                    << "   " << cDm
                    << col(to_string(p.stockQuantity), 6, true)
                    << " in stock" << cRs << "\n";
                    ++shown;
                }
            }
            if (shown == 0) cout << "    (no in-stock products for this brand right now)\n";
            cout << "\n"; printRule(); if (readLine("  [Enter] next brand   [0] back: ") == "0") return;
            cur = cur->next;   // wraps from the last brand back to the first
        }
    }
    void sortInventory() {
        pageTitle("Sort Inventory");
        auto pick = [](const string& label, const string& opts, int hi) {   // prints "label 1 A 2 B 0 Cancel"
            cout << "  " << label << opts << cRd << " 0" << cRs << " Cancel\n"; return readInt("  Choose: ", 0, hi); };
        const string o1 = cAc + " 1" + cRs, o2 = cAc + "2" + cRs, o3 = cAc + "3" + cRs;
        int key = pick("Sort by      ", o1 + " Price   " + o2 + " Stock   " + o3 + " Brand  ", 3);
        int ord = key ? pick("Order        ", o1 + " Ascending   " + o2 + " Descending  ", 2) : 0;
        int alg = ord ? pick("Algorithm    ", o1 + " Insertion Sort   " + o2 + " Quick Sort  ", 2) : 0;
        if (!alg) { cout << "  Cancelled.\n"; return; }
        int n = catalog.size(); bool desc = (ord == 2);
        Product* copy = new Product[n > 0 ? n : 1];   // sort a copy; the catalog stays unchanged
        for (int i = 0; i < n; ++i) copy[i] = catalog.at(i);
        if (alg == 1) insertionSort(copy, n, key, desc); else quickSort(copy, 0, n - 1, key, desc);
        const char* kn[] = {"", "Price", "Stock", "Brand"};
        pageTitle(string("Inventory by ") + kn[key] + " (" + (desc ? "high to low" : "low to high") + ")");
        printProductTable(copy, n); delete[] copy;
    }
    void drawMainMenu() {
        const int inner = 68;
        clearScreen();
        auto line = [&]() { cout << "  " << cAc << "+" << string(inner, '-') << "+" << cRs << "\n"; };
        auto row = [&](const string& colored, int plainLen) {
            cout << "  " << cAc << "|" << cRs << colored << string((size_t)(inner > plainLen ? inner - plainLen : 0), ' ') << cAc << "|" << cRs << "\n";
        };
        auto center = [&](const string& colored, const string& plain) { int l = (inner - (int)plain.size()) / 2; row(string((size_t)l, ' ') + colored, l + (int)plain.size()); };
        cout << "\n"; line(); row("", 0); center(cBd + cAc + Config::APP_NAME + cRs, Config::APP_NAME);
        center(cBd + cWh + Config::APP_SUBTITLE + cRs, Config::APP_SUBTITLE); row("", 0); line();
        const char* items[10] = {"View Complete Catalog", "Filter Products by Official Brand", "Search Products", "Filter Products by Price Range",
            "Add Item to Shopping Cart", "Process Checkout Queue", "Perform Admin Undo Action", "Display Partner Brand Banner Showcase",
            "Sort Inventory Display", "Exit"};
        for (int i = 0; i < 10; ++i) {
    bool isExit = (i == 9);
    string number = isExit ? "10" : to_string(i + 1);
    string coloredNumber = cBd + (isExit ? cRd : cAc) + number + cRs;
    string plainText = "   " + number + "   " + string(items[i]);
    string coloredText = "   " + coloredNumber + "   " + string(items[i]);
    row(coloredText, (int)plainText.size());
}
        line(); cout << "\n";
        printNote(to_string(catalog.size()) + " products   |   cart: " + itemsText(cart.units()) + "   |   pending orders: " + to_string(orders.size()));
        cout << "\n";
    }
public:
    StoreApp() : nextProductId(1), nextOrderNo(1001) { seed(); }
    void run() {
        while (true) {
            drawMainMenu(); int ch = readInt("  Enter your choice (1-10): ", 1, 10);
            switch (ch) {
            case 1: viewCatalog(); pauseScreen(); break;      
            case 2: filterByBrand(); pauseScreen(); break; 
            case 3: keywordSearch(); pauseScreen(); break;    
            case 4: priceRangeFilter(); pauseScreen(); break;
            case 5: cartMenu(); break;      
            case 6: checkoutMenu(); break;
            case 7: adminMenu(); break;     
            case 8: bannerShowcase(); break;
            case 9: sortInventory();    pauseScreen(); break;
            case 10:
                if (!orders.empty() && !confirm("  Pending orders exist. Exit anyway?")) break;
                clearScreen();
                cout << "\n  THANK YOU FOR USING BEAUTY & COSMETICS!\n";
                cout << "\n        Program closed safely.\n";
                system("pause");
                return;
            }
        }
    }
};
int main() {
    initConsole();
    try { StoreApp app; app.run(); }   // every dynamic structure is released by destructors
    catch (const EofException&) { cout << "\n  Input closed. Exiting safely...\n"; }
    return 0;
}
