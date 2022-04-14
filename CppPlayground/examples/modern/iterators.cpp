#include "register_items.h"
#include "categories.h"
#include <cassert>
#include <fmt/core.h>
#include <typeinfo>


struct list_node {
    int data;
    list_node* next;
};

template <bool UseBoost>
class list_of_ints_t;

template<bool Const>
class list_of_ints_iterator {
    friend class list_of_ints;
    friend class list_of_ints_t<false>;
    friend class list_of_ints_t<true>;
    friend class list_of_ints_iterator<!Const>;

    using node_pointer = std::conditional_t<Const, const list_node*, list_node*>;
    node_pointer ptr_;

    explicit list_of_ints_iterator(node_pointer p) : ptr_(p) {}
public:
    // Member typedefs required by std::iterator_traits
    using difference_type = std::ptrdiff_t;
    using value_type = int;
    using pointer = std::conditional_t<Const, const int*, int*>;
    using reference = std::conditional_t<Const, const int&, int&>;
    using iterator_category = std::forward_iterator_tag;

    reference operator*() const { return ptr_->data; }
    auto& operator++() { ptr_ = ptr_->next; return *this; }
    auto operator++(int) { auto result = *this; ++* this; return result; }

    // Support comparison between iterator and const_iterator types
    template<bool R>
    bool operator==(const list_of_ints_iterator<R>& rhs) const { return ptr_ == rhs.ptr_; }

    template<bool R>
    bool operator!=(const list_of_ints_iterator<R>& rhs) const { return ptr_ != rhs.ptr_; }

    // Support implicit conversion of iterator to const_iterator (but not vice versa)
    operator list_of_ints_iterator<true>() const { return list_of_ints_iterator<true>{ptr_}; }
};


#if __has_include(<boost/iterator/iterator_facade.hpp>)
#define HAS_BOOST
#include <boost/iterator/iterator_facade.hpp>

template<bool Const>
class list_of_ints_iterator_boost : public boost::iterator_facade<
    list_of_ints_iterator_boost<Const>,
    std::conditional_t<Const, const int, int>,
    std::forward_iterator_tag
>
{
    friend class boost::iterator_core_access;
    friend class list_of_ints_t<true>;
    friend class list_of_ints_t<false>;
    friend class list_of_ints_iterator_boost<!Const>;

    using node_pointer = std::conditional_t<Const, const list_node*, list_node*>;
    node_pointer ptr_;

    explicit list_of_ints_iterator_boost(node_pointer p) : ptr_(p) {}

    auto& dereference() const { return ptr_->data; }
    void increment() { ptr_ = ptr_->next; }

    // Support comparison between iterator and const_iterator types
    template<bool R>
    bool equal(const list_of_ints_iterator_boost<R>& rhs) const { return ptr_ == rhs.ptr_; }

public:
    // Support implicit conversion of iterator to const_iterator (but not vice versa)
    operator list_of_ints_iterator_boost<true>() const { return list_of_ints_iterator_boost<true>{ptr_}; }
};

#endif

template <bool UseBoost = false>
class list_of_ints_t {
    list_node* head_ = nullptr;
    list_node* tail_ = nullptr;
    int size_ = 0;
public:

#ifdef HAS_BOOST
    using const_iterator = std::conditional_t<UseBoost, list_of_ints_iterator_boost<true>, list_of_ints_iterator<true>>;
    using iterator = std::conditional_t<UseBoost, list_of_ints_iterator_boost<false>, list_of_ints_iterator<false>>;
#else
    using const_iterator = list_of_ints_iterator<true>;
    using iterator = list_of_ints_iterator<false>;
#endif

    // Begin and end member functions
    iterator begin() { return iterator{ head_ }; }
    iterator end() { return iterator{ nullptr }; }
    const_iterator begin() const { return const_iterator{ head_ }; }
    const_iterator end() const { return const_iterator{ nullptr }; }

    // Other member operations
    int size() const { return size_; }
    void push_back(int value) {
        list_node* new_tail = new list_node{ value, nullptr };
        if (tail_) {
            tail_->next = new_tail;
        }
        else {
            head_ = new_tail;
        }
        tail_ = new_tail;
        size_ += 1;
    }
    ~list_of_ints_t() {
        for (list_node* next, *p = head_; p != nullptr; p = next) {
            next = p->next;
            delete p;
        }
    }
};

template<typename Iterator>
auto distance(Iterator begin, Iterator end)
{
    using Traits = std::iterator_traits<Iterator>;
    if constexpr (std::is_base_of_v<std::random_access_iterator_tag, typename Traits::iterator_category>) {
        return (end - begin);
    }
    else {
        auto result = typename Traits::difference_type{};
        for (auto it = begin; it != end; ++it) {
            ++result;
        }
        return result;
    }
}

template<typename Iterator, typename Predicate>
auto count_if(Iterator begin, Iterator end, Predicate pred)
{
    using Traits = std::iterator_traits<Iterator>;
    auto sum = typename Traits::difference_type{};
    for (auto it = begin; it != end; ++it) {
        if (pred(*it)) {
            ++sum;
        }
    }
    return sum;
}

template <typename List>
void test_list(List& lst)
{
    fmt::print("Iterator type = {}\n", typeid(List::iterator).name());

    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);

    int s = count_if(lst.begin(), lst.end(), [](int i) {
        return i >= 2;
        });

    fmt::print("count_if(i >= 2) = {}\n", s);

    int d = distance(lst.begin(), lst.end());
    fmt::print("distance = {}\n", d);

    fmt::print("Range for loop\n");
    for (auto&& i : lst) {
        fmt::print("{}\n", i);
    }
}

ADD_MENU_ITEM(iterators, "Iterator implementation", category_stl)
{
    list_of_ints_t lst_std;
    fmt::print("== Running test on std list\n");
    test_list(lst_std);

#ifdef HAS_BOOST
    list_of_ints_t<true> lst_boost;
    fmt::print("== Running test on boost list\n");
    test_list(lst_boost);
#endif
}