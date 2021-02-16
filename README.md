# Base64 boost filter
Boost tutorial have next example: 
https://www.boost.org/doc/libs/1_66_0/libs/iostreams/doc/index.html 
```c++
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filtering_stream.hpp>

namespace io = boost::iostreams;

int main()
{   
    io::filtering_ostream out;
    out.push(compressor());
    out.push(base64_encoder());
    out.push(file_sink("my_file.txt"));
    // write to out using std::ostream interface
}
```
But I searched for base64_encoder () in the boost lib or elsewhere and only find an implementation using more memory.

I tried to implement the ability to use a filter in a similar style. 

# Install

You need to install the boost in your project and copy the base64.hpp from this project.

# Using

For example, main.cpp uses input and output filters for base64 encoding and decoding.

Can suggest the following code to save compressed gzip in a string:   
```c++
#include <utility>
#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "base64.hpp"

namespace io = boost::iostreams;

int main()
{   
        std::stringstream stream;
        io::filtering_streambuf<io::output> outbuf;
        outbuf.push(io::base64::base64encode());
        outbuf.push(io::gzip_compressor());
        outbuf.push(stream);
        std::ostream out(&outbuf);
        out << "Example string for compressed gzip!!!";
        boost::iostreams::close(outbuf); // Don't forget this!

        cout << stream.str() << std::endl;
}
```

And next example for saved string gunzip:
```c++
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
```

If you add a buffer, the speed of the filter will increase, but this will be something later. 
