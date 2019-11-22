#pragma once

/*
====================================================

Generic gap buffer

====================================================
*/

template <class type> class GBuff {
public:
  GBuff(std::size_t len = 32);
  ~GBuff();

  void Insert(type element);
  void Delete(std::size_t position);
  std::size_t CursorLeft(std::size_t position);
  std::size_t CursorRight(std::size_t position);
  std::size_t Capacity();
  std::size_t Size();
  type &operator[](std::size_t position);

  /*
  ====================================================

  Gap Buffer Iterator - only supports forward iteration and dereference

  ====================================================
  */
  class GBuffIterator {
  public:
    typedef std::input_iterator_tag iterator_category; // or another tag

    // Note: Instances build with this constructor should
    // be used only after copy-assigning from other iterator!
    GBuffIterator();
    ~GBuffIterator();
    type operator*() const;
    GBuffIterator &operator++();
    GBuffIterator operator++(int);

  protected:
    GBuffIterator(GBuff<type> *gbuff, std::size_t index = 0);

  private:
    GBuff<type> *gbuff;
    std::size_t index;
  };

public:
  typedef GBuff<type>::GBuffIterator iterator;
  iterator begin();

private:
  type *buffer;
  std::size_t total;   // total size of the buffer
  std::size_t cursor;  // pointer to the cursor
  std::size_t gap_end; // end of the gap

  std::size_t gapSize();

  friend class GBuffIterator;
};

/**
 * Main constructor
 **/
template <class type> GBuff<type>::GBuff(std::size_t len) {
  buffer = new type[len];
  total = len;
  cursor = 0;
  gap_end = len; // gap always starts at the beginning - it is essentially the
                 // whole thing
}

/**
 * Main destructor
 **/
template <class type> GBuff<type>::~GBuff() { delete[] buffer; }

/**
 * Insert an element into the gap buffer to the right of the cursor
 * @param element the element to be added
 * @param position the place to start inserting the element
 * @return std::size_t the new size of the gap buffer if it was grown
 **/
template <class type> void GBuff<type>::Insert(type element) {
  if (gapSize() == 0) {
    // grow the gap and move the elements to the right
    // double it and fill it with gap
    type *newbuf = new type[total * 2];
    // copy left
    std::copy(buffer, buffer + cursor, newbuf);
    // copy right
    std::copy(buffer + gap_end, buffer + total, newbuf + (total * 2 - gap_end));
    delete[] buffer;

    buffer = newbuf;
    gap_end = total * 2 - (total - gap_end);
    total *= 2;
  }
  // depending on where you are in the buffer, you copy everything to the right
  // of the cursor to the end to make space for the new element
  buffer[cursor++] = element;
}

/**
 * Delete an element from this gap buffer - there is no need to physically
 *delete it just move the gap so its covered up Move the start of the gap
 *backwards
 * @param position the place to delete from
 **/
template <class type> void GBuff<type>::Delete(std::size_t position) {
  cursor = position;
}

/**
 * Move the cursor to the left - the more we move left the more characters we
 *need to copy to the right of the gap We are essentially adjusting boundaries
 *of the gap and need to worry about the _start_ of the buffer the size of the
 *gap here is important
 * @param position place to move the cursor to
 **/
template <class type>
std::size_t GBuff<type>::CursorLeft(std::size_t position) {
  // gap boundaries = cursor, cursor + gap_end
  std::copy(buffer + position, buffer + cursor, (buffer + gap_end) - (cursor - position));
  gap_end = gapSize();
  cursor = position;
  return cursor;
}

/**
 * Move the cursor to the right - the more we move right the more characters we
 *need to move to the left of the gap We are essentially adjusting boundaries of
 *the gap and need to worry about the _end_ of the buffer the size of the gap
 *here is important
 * @param position the place to delete from
 * @param the new cursor position
 **/
template <class type>
std::size_t GBuff<type>::CursorRight(std::size_t position) {
  // gap boundaries = cursor, cursor + gap_end
  std::copy(buffer + cursor, buffer + position, buffer);
  std::size_t gap_size = gapSize();
  cursor = position;
  gap_end = cursor + gap_size;
  return cursor;
}

/**
 * Return the capacity of this gap buffer
 **/
template <class type> std::size_t GBuff<type>::Capacity() { return total; }

/**
 * Return the size occupied in this buffer (without the gap)
 **/
template <class type> std::size_t GBuff<type>::Size() {
  return total - (gapSize());
}

/**
 * Return the element at the position
 * The position is not absolute as we have to account for the gap size
 **/
template <class type> type &GBuff<type>::operator[](std::size_t position) {
  if (position >= cursor) {
    position += (gapSize());
  }
  return buffer[position];
}

/**
 * Private function for calculating the gap size - save some code in some places
**/
template<class type> std::size_t GBuff<type>::gapSize() {
  return gap_end -cursor;
}

/*
====================================================

Iterator - only supports forward iteration and dereference

====================================================
*/
template <typename type> GBuff<type>::GBuffIterator::GBuffIterator() {}

template <typename type> GBuff<type>::GBuffIterator::~GBuffIterator() {}

template <typename type>
GBuff<type>::GBuffIterator::GBuffIterator(GBuff<type> *gbuff,
                                          std::size_t index) {
  this->gbuff = gbuff;
  this->index = index;
}

template <typename type> type GBuff<type>::GBuffIterator::operator*() const {}

template <typename type>
typename GBuff<type>::GBuffIterator &GBuff<type>::GBuffIterator::operator++() {}

template <typename type>
typename GBuff<type>::GBuffIterator GBuff<type>::GBuffIterator::operator++(int) {}