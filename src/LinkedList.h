#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  class Node{
  public:
    Node *prev;
    Node *next;
    value_type data;
    Node(): prev(nullptr), next(nullptr), data(0)
    {}
    Node(value_type d, Node* p=nullptr, Node* n=nullptr): prev(p), next(n)
    {
        data = d;
    }
    ~Node()
    {
        next = nullptr;
        prev = nullptr;
    }
  };

  LinkedList(): csize(0)
  {
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
  }

  LinkedList(std::initializer_list<Type> l): LinkedList()
  {
    for(auto&& i:l)
        append(i);
  }

  LinkedList(const LinkedList& other): LinkedList()
  {
    for(auto&& i:other)
        append(i);
  }

  LinkedList(LinkedList&& other): LinkedList()
  {
    Node* temphead = std::move(head);
    Node* temptail = std::move(tail);
    head = std::move(other.head);
    tail = std::move(other.tail);
    csize = std::move(other.csize);
    other.head = std::move(temphead);
    other.tail = std::move(temptail);
    other.csize = 0;
  }

  ~LinkedList()
  {
    clear();
    delete head;
    delete tail;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    if(this==&other) return *this;
    clear();
    for(auto&& i:other)
        append(i);
    csize = other.csize;
    return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
    if(this->head==other.head) return *this;
    clear();
    Node* temphead = head;
    Node* temptail = tail;
    head = std::move(other.head);
    tail = std::move(other.tail);
    csize = std::move(other.csize);
    other.head = temphead;
    other.tail = temptail;
    other.csize = 0;
    return *this;
  }

  bool isEmpty() const
  {
    return head->next==tail;
  }

  size_type getSize() const
  {
    return csize;
  }

  void append(const Type& item)
  {
    insert(cend(), item);
  }

  void prepend(const Type& item)
  {
    insert(cbegin(), item);
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    Node* prenode = insertPosition.cnode;
    Node* node = new Node(item, prenode->prev, prenode);
    prenode->prev->next = node;
    prenode->prev = node;
    csize++;
    if(csize!=1 && head->next==tail->prev)throw std::runtime_error("insert się zjebał xdd");
  }

  Type popFirst()
  {
    if(isEmpty()) throw std::out_of_range("List empty");
    value_type w = head->next->data;
    erase(begin());
    return w;
  }

  Type popLast()
  {
    if(isEmpty()) throw std::out_of_range("List empty");
    value_type w = tail->prev->data;
    erase(--end());
    return w;
  }

  void erase(const const_iterator& position)    //USUNALEM DRUGIE S
  {
    Node* node = position.cnode;
    if(node==tail || node==head) throw std::out_of_range("Trying to erase guard");
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
    csize--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(firstIncluded==cbegin() && lastExcluded==cend())
    {
        clear();
        return;
    }
    Node* first = firstIncluded.cnode;
    Node* last = lastExcluded.cnode;
    if(first==last) return;
    Node* node = first;
    while(node!=last)
    {
        node = node->next;
        if(node==last) break;
        if(node==tail) throw std::out_of_range("lastExcluded isn't past firstIncluded");
    }
    while(first!=last)
    {
        node = first;
        first = first->next;
        first->prev = node->prev;
        node->prev->next = first;
        delete node;
        csize--;
    }
  }

  iterator begin()
  {
    return Iterator(this, head->next);
  }

  iterator end()
  {
    return Iterator(this, tail);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(this, head->next);
  }

  const_iterator cend() const
  {
    return ConstIterator(this, tail);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

  void clear()
    {
        if(isEmpty()) return;
        Node* cnode = head->next;
        Node* temp;
        while(cnode!=tail)
        {
            temp = cnode;
            cnode = cnode->next;
            delete temp;
        }
        head->next = tail;
        tail->prev = head;
        csize = 0;
    }

private:
    Node* head;
    Node* tail;
    size_type csize;

};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

  const LinkedList *list;
  Node *cnode;

  explicit ConstIterator(): list(nullptr), cnode(nullptr)
  {}

  ConstIterator(const LinkedList<value_type> *l, Node* n): list(l), cnode(n)
  {}

  reference operator*() const
  {
    if(cnode==list->tail || cnode==list->head) throw std::out_of_range("Can't dereference guardian");
    return cnode->data;
  }

  ConstIterator& operator++()
  {
    if(cnode==list->tail) throw std::out_of_range("trying to ++tail");
    cnode = cnode->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator temp(*this);
    operator++();
    return temp;
  }

  ConstIterator& operator--()
  {
    if(cnode->prev == list->head) throw std::out_of_range("trying to geet into head");
    cnode = cnode->prev;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator temp(*this);
    operator--();
    return temp;
  }

  ConstIterator operator+(difference_type d) const
  {
    Node* node = cnode;
    for(int i=0; i<std::abs(d); i++)
    {
        if(node==list->tail)
        {
            throw std::out_of_range("trying to go past tail");
        }
        node = node->next;
    }
    return ConstIterator(list, node);
  }

  ConstIterator operator-(difference_type d) const
  {
    Node* node = cnode;
    for(int i=0; i<std::abs(d); i++)
    {
        if(node->prev==list->head) throw std::out_of_range("trying to get into head");
        node = node->prev;
    }
    return ConstIterator(list, node);
  }

  bool operator==(const ConstIterator& other) const
  {
    return cnode==other.cnode;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !operator==(other);
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(): ConstIterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator(const LinkedList<value_type>* l, Node* n): ConstIterator(l, n)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
