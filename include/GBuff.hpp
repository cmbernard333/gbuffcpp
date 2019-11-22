#pragma once

/*
====================================================

Generic gap buffer

====================================================
*/

template<class type>
class GBuff {
public:
    GBuff(std::size_t len=32);
    ~GBuff();
    
    void Insert(type& element);
    void Delete(std::size_t position);
    std::size_t CursorLeft(std::size_t position);
    std::size_t CursorRight(std::size_t position);
    std::size_t Capacity();
    std::size_t Size();

private:
    type *buffer;
    std::size_t total; // total size of the buffer
    std::size_t cursor; // pointer to the cursor
    std::size_t gap_end; // end of the gap
};

/**
 * Main constructor
**/
template<class type>
GBuff<type>::GBuff(std::size_t len) {
    buffer = new type[len];
    total = len;
    cursor = 0;
    gap_end = len; // gap always starts at the beginning - it is essentially the whole thing
}

/**
 * Main destructor
**/
template <class type>
GBuff<type>::~GBuff() {
    delete[] buffer;
}

/**
 * Insert an element into the gap buffer to the right of the cursor
 * @param element the element to be added
 * @param position the place to start inserting the element 
 * @return std::size_t the new size of the gap buffer if it was grown
**/
template <class type>
void GBuff<type>::Insert(type& element) {
    if (gap_end - cursor <= element.size()) {
        // grow the gap and move the elements to the right
        // double it and fill it with gap
        type* newbuf = new type[total*2];
        // copy left
        std::copy(buffer, buffer+cursor, newbuf);
        // copy right
        std::copy(buffer + gap_end, buffer + total, newbuf + (total*2 - gap_end));
        delete[] buffer;

        buffer = newbuf;
        gap_end = total*2 - (total - gap_end);
        total *= 2;
    }
    // depending on where you are in the buffer, you copy everything to the right of the cursor to the end to make space for the new element
    std::copy(element, element.size(), buffer[cursor]);
    cursor += element.size();
}

/**
 * Delete an element from this gap buffer - there is no need to physically delete it just move the gap so its covered up
 * Move the start of the gap backwards
 * @param position the place to delete from
**/
template <class type>
void GBuff<type>::Delete(std::size_t position) {
    cursor = position;
}

/**
 * Move the cursor to the left - the more we move left the more characters we need to copy to the right of the gap
 * We are essentially adjusting boundaries of the gap and need to worry about the _start_ of the buffer
 * the size of the gap here is important
 * @param position place to move the cursor to
**/
template <class type>
std::size_t GBuff<type>::CursorLeft(std::size_t position) {
    // gap boundaries = cursor, cursor + gap_end
    std::copy(buffer + position, buffer + cursor, buffer);
    cursor = position;
    gap_end = gap_end - cursor;
    return cursor;
}

/**
 * Move the cursor to the right - the more we move right the more characters we need to move to the left of the gap
 * We are essentially adjusting boundaries of the gap and need to worry about the _end_ of the buffer
 * the size of the gap here is important
 * @param position the place to delete from
 * @param the new cursor position
**/
template <class type>
std::size_t GBuff<type>::CursorRight(std::size_t position) {
    // gap boundaries = cursor, cursor + gap_end
    return cursor;
}

/**
 * Return the capacity of this gap buffer
**/
template <class type>
std::size_t GBuff<type>::Capacity() {
    return total;
}

/**
 * Return the size occupied in this buffer (without the gap)
**/
template <class type>
std::size_t GBuff<type>::Size() {
    return total - (gap_end - cursor);
}