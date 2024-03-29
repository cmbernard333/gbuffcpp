#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>

#include "GBuff.hpp"

char alpha[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                  'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                  's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int test_gap_buffer() {
  // this is faking a doubled total buffer
  char *gap_buffer = new char[52];
  std::size_t cursor = 13;
  std::size_t gap_end = cursor;
  std::size_t end = 26;
  std::size_t new_end = 52;

  // copy left
  std::copy(alpha, alpha + cursor, gap_buffer);
  std::copy(alpha + gap_end, alpha + end, gap_buffer + (new_end - gap_end));

  // print

  std::cout << "PreGap:" << std::endl;
  for (char *p = gap_buffer; p != &gap_buffer[cursor]; p++) {
    std::cout << *p << ",";
  }
  std::cout << std::endl;

  std::size_t new_gap_end = new_end - (end - gap_end);
  std::cout << "PostGap:" << std::endl;
  for (char *p = gap_buffer + new_gap_end; p != gap_buffer + new_end; p++) {
    std::cout << *p << ",";
  }
  std::cout << std::endl;

  delete[] gap_buffer;

  return 0;
}

int main(int argc, char **argv) { 
    GBuff<char> gbuff(16);
    std::string garbage("garbage");
    std::string day("day");
    // check initial capacity
    assert(gbuff.Capacity()==16);
    assert(gbuff.Size()==0);
    for(auto it = garbage.begin(); it != garbage.end(); it++) {
        gbuff.Insert(*it);
    }
    // check new size and capacity
    assert(gbuff.Capacity()==16);
    assert(gbuff.Size()==garbage.length());

    // check some values - start, end
    assert(gbuff[0]=='g');
    assert(gbuff[6]=='e');

    // let's make this complex now - move the cursor
    gbuff.CursorLeft(0);

    // verify start, end are same
    assert(gbuff[0]=='g');
    assert(gbuff[6]=='e');

    // let's add some stuff
    for(auto it = day.begin(); it != day.end(); it++) {
      gbuff.Insert(*it);
    }
    gbuff.Insert(' ');
    assert(gbuff.Capacity()==16);
    assert(gbuff.Size()==garbage.length()+day.length()+1);

    // let's check the contents and see if we get the right thing
    char buffer_contents[gbuff.Size()];
    std::size_t index = 0;
    for(auto it = gbuff.begin(); it != gbuff.end(); ++it) {
      buffer_contents[index++] = *it;
    }
    for(index = 0; index < gbuff.Size(); index++) {
      std::cout<<buffer_contents[index]<<std::endl;
    }

    return 0; 
}