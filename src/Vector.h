#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
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

public:
  Vector(): csize(0), maxsize(2)
  {
    tab = new value_type[maxsize];
  }

  Vector(std::initializer_list<Type> l) : Vector()
  {
    realloc(2*l.size());
    csize = l.size();
    int temp = 0;
    for(auto&& i : l)
    {
        tab[temp++] = std::move(i);
    }
  }

  Vector(const Vector& other): csize(other.csize), maxsize(other.maxsize)
  {
    tab = new value_type[other.maxsize];
    for(size_type i=0; i<other.csize; i++)
    {
        tab[i] = other.tab[i];
    }
  }

  Vector(Vector&& other): Vector()
  {
    delete[] tab;
    this->csize = std::move(other.csize);
    this->maxsize = std::move(other.maxsize);
    this->tab = std::move(other.tab);
    other.tab = nullptr;
  }

  ~Vector()
  {
    if(tab != nullptr)
    {
        delete[] tab;
    }
  }

  Vector& operator=(const Vector& other)
  {
    if(maxsize <= other.csize)
        realloc(other.maxsize);
    for(size_type i=0; i<other.csize; i++)
    {
        tab[i] = other.tab[i];
    }
    csize = other.csize;
    return *this;
  }

  Vector& operator=(Vector&& other)
  {
    if(tab!=nullptr) delete[] tab;
    this->csize = std::move(other.csize);
    this->maxsize = std::move(other.maxsize);
    this->tab = std::move(other.tab);
    other.tab = nullptr;
    return *this;
  }

  bool isEmpty() const
  {
    return csize == 0;
  }

  size_type getSize() const
  {
    return csize;
  }

  void realloc(size_type size=0)
  {
    if(size<0) throw std::bad_array_new_length();
    if(size==0) size = 2*maxsize;
    else if(size<=csize) throw std::bad_array_new_length();
    pointer newtab = new value_type[size];
    for(size_type i=0; i<csize; i++)
    {
        newtab[i] = tab[i];
    }
    maxsize = size;
    delete[] tab;
    tab = newtab;
  }

  void append(const Type& item)
  {
    if(csize>=maxsize)
    {
        realloc();
    }
    tab[csize] = item;
    csize++;
  }

  void prepend(const Type& item)
  {
    const ConstIterator i(this, 0);
            insert(i, item);

  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(csize>=maxsize)
    {
        realloc();
    }
    for(size_type i=csize; i>insertPosition.index; i--)
    {
        tab[i] = tab[i-1];
    }
    tab[insertPosition.index] = item;
    csize++;
  }

  Type popFirst()
  {
    if(isEmpty()) throw std::out_of_range("Vector empty");
    Type w = tab[0];
    csize--;
    for(size_type i=0; i<csize; i++)
    {
        tab[i] = tab[i+1];
    }
    return w;
  }

  Type popLast()
  {
    if(isEmpty()) throw std::out_of_range("Vector empty");
    Type w = tab[csize-1];
    csize--;
    return w;
  }

  void erase(const const_iterator& position)    //usunalem drugie s
  {
    if(csize<=position.index) throw std::out_of_range("erasing out of range");
    csize--;
    for(size_type i=position.index; i<csize; i++)
    {
        tab[i] = tab[i+1];
    }
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if (lastExcluded.index <= firstIncluded.index) return;
    if (firstIncluded.index >= csize || isEmpty()) throw std::out_of_range("erasing out of range");
    for(size_type i=lastExcluded.index; i<csize; i++)
    {
        tab[i+firstIncluded.index-lastExcluded.index] = tab[i];
    }
    csize -= (lastExcluded.index-firstIncluded.index);
  }

  iterator begin()
  {
    return Iterator(ConstIterator(this,0));
  }

  iterator end()
  {
    return Iterator(ConstIterator(this,csize));
  }

  const_iterator cbegin() const
  {
    return ConstIterator(this,0);
  }

  const_iterator cend() const
  {
    return ConstIterator(this,csize);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

private:
  size_type csize;
  size_type maxsize;
  pointer tab;

};

template <typename Type>
class Vector<Type>::ConstIterator
{


public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

  const Vector* vptr;
  size_type index;

  explicit ConstIterator(): index(0), vptr(nullptr)
  {}

  ConstIterator(const Vector<value_type> *v, int i): vptr(v), index(i)
  {}

  reference operator*() const
  {
    if(vptr==nullptr) throw std::out_of_range("iterator out of range");
    if(index>=vptr->csize || index<0) throw std::out_of_range("iterator out of range");
    return vptr->tab[index];
  }

  ConstIterator& operator++()
  {
    if(index+1>vptr->csize) throw std::out_of_range("iterator out of range");
    index++;
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator i(vptr, index);
    operator++();
    return i;
  }

  ConstIterator& operator--()
  {
    if(((int)index)-1<0) throw std::out_of_range("iterator out of range");
    index--;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator i(vptr, index);
    operator--();
    return i;
  }

  ConstIterator operator+(difference_type d) const
  {
    if(index+d>vptr->csize) throw std::out_of_range("iterator out of range");
    return ConstIterator(vptr, index+d);
  }

  ConstIterator operator-(difference_type d) const
  {
    if(index-d<0) throw std::out_of_range("iterator out of range");
    return ConstIterator(vptr, index-d);
  }

  bool operator==(const ConstIterator& other) const
  {
    return (vptr==other.vptr && index==other.index);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !operator==(other);
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(): ConstIterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator(const Vector<value_type> *v, size_type i): ConstIterator(v, i)
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

#endif // AISDI_LINEAR_VECTOR_H
