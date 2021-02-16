#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <sstream>
#include "../base64.hpp"

namespace io = boost::iostreams;

int main()
{
    std::stringstream stream("H4sIAAAAAAAA/3OtSMwtyElVKC4pysxLV0jLL1JIzs8tKEotLk5NUUivyixQVFQEAFodDdYlAAAA");

    io::filtering_streambuf<io::input> inbuf;
    inbuf.push(io::gzip_decompressor());
    inbuf.push(io::base64::base64decode());
    inbuf.push(stream);

    std::istream in(&inbuf);
    std::noskipws(in);

    std::stringstream stream_result("");
    std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(),
              std::ostream_iterator<char>(stream_result));
    //Cleanup
    boost::iostreams::close(inbuf); // Don't forget this!

    std::cout << stream_result.str() << std::endl;
}