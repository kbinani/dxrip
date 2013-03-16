#ifndef ONULLSTREAM_
#define ONULLSTREAM_

#include <iostream>

template<class Ch,class Tr = std::char_traits<Ch> >
class basic_null_streambuf : public std::basic_streambuf<Ch, Tr> {
public:
    basic_null_streambuf() {
        setbuf(0, 0);
    }

    ~basic_null_streambuf() {
    }

    protected:
    std::streampos seekoff(std::streamoff off, std::ios::seek_dir dir, int nMode = std::ios::in | std::ios::out) {
        return EOF;
    }

    std::streampos seekpos(std::streampos pos, int nMode = std::ios::in | std::ios::out) {
        return EOF;
    }

    int overflow(int nCh = EOF) {
        return 0;
    }

    int underflow() {
        return EOF;
    }
};

template<class Ch,class Tr = std::char_traits<Ch> >
class basic_null_stream : public std::basic_iostream<Ch, Tr> {
public:
    basic_null_stream()
        : std::basic_iostream<Ch,Tr>(new basic_null_streambuf<Ch,Tr>()) {
    }

    ~basic_null_stream() {
    }
};

typedef basic_null_streambuf<char> nullstreambuf;
typedef basic_null_stream<char> nullstream;

#endif
