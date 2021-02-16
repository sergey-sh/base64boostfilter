#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <sstream>
#include "../base64.hpp"

namespace io = boost::iostreams;

int main()
{
    std::stringstream stream;
    io::filtering_streambuf<io::output> outbuf;
    outbuf.push(io::gzip_compressor());
    outbuf.push(io::base64::base64encode());
    outbuf.push(stream);
    std::ostream out(&outbuf);
    out << "Example string for compressed gzip!!!";
    boost::iostreams::close(outbuf); // Don't forget this!

    std::cout << stream.str() << std::endl;
}