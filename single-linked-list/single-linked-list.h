#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

public:  

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        if (values.size() > 0) {
            SingleLinkedList tmp;
            auto it_tmp = &tmp.head_;
            Node* next_tmp;
            
            for (auto it : values) {
                next_tmp = new Node(it, nullptr);
                it_tmp->next_node = next_tmp;
                it_tmp = next_tmp;
                ++tmp.size_;
            }
            swap(tmp);
        }
    }

    SingleLinkedList(const SingleLinkedList& other) { 
        assert(size_ == 0 && head_.next_node == nullptr);
        *this = other;
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (&rhs == &*this) {
            return *this;
        }
        SingleLinkedList tmp;
        auto it_tmp = &tmp.head_;
        Node* next_tmp;
        
        for (auto it : rhs) {
            next_tmp = new Node(it, nullptr);
            it_tmp->next_node = next_tmp;
            it_tmp = next_tmp;
            ++tmp.size_;
        }
        swap(tmp);

        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        Node* tmp = head_.next_node;
        head_.next_node = other.head_.next_node;
        other.head_.next_node = tmp;
        size_t size_tmp = size_;
        size_ = other.size_;
        other.size_ = size_tmp;
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return head_.next_node == nullptr;
    }
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;

    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node* buf = head_.next_node;
            head_.next_node = buf->next_node;
            delete buf;
            --size_;
        }
    }

    ~SingleLinkedList() {
        Clear();
    }

    template <typename ValueType>
    class BasicIterator {

        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            if ((node_ == nullptr) && (rhs.node_ == nullptr)) {
                return true;
            }
            if ((node_ == nullptr) || (rhs.node_ == nullptr)) {
                return false;
            }
            return &*rhs.node_ == &*node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            if ((node_ == nullptr) && (rhs.node_ == nullptr)) {
                return true;
            }
            if ((node_ == nullptr) || (rhs.node_ == nullptr)) {
                return false;
            }
            return &*rhs.node_ == &*node_;            
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator buf = *this;
            node_ = node_->next_node;
            return buf;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return reference(*node_);
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return pointer(node_);
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept { 
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        Iterator it = begin();
        while (it.node_ != nullptr) {
            ++it;
        }
        return Iterator(it.node_);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        ConstIterator it = begin();
        while (it.node_ != nullptr) {
            ++it;
        }
        return ConstIterator(it.node_);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        ConstIterator it = begin();
        while (it.node_ != nullptr) {
            ++it;
        }
        return ConstIterator(it.node_);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* tmp = new Node(value, pos.node_->next_node);
        pos.node_->next_node = tmp;
        ++size_;
        tmp = nullptr;
        return Iterator(pos.node_->next_node);
    }

    void PopFront() noexcept {
        Node* tmp = head_.next_node;
        head_.next_node = tmp->next_node;
        delete tmp;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* tmp = pos.node_->next_node;
        pos.node_->next_node = tmp->next_node;
        delete tmp;
        return Iterator(pos.node_->next_node);
    }

private:
    Node head_ = Node();
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());

}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs <= lhs;
} 


