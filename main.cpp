#include <iostream>
#include <sstream>
#include "base64.hpp"
#include <boost/iostreams/filtering_streambuf.hpp>


int main() {

    {
        // test output filter
        std::stringstream stream_result("");
        std::noskipws(stream_result);
        std::stringstream stream_decoded("QQ");
        std::noskipws(stream_decoded);

        boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;
        outbuf.push(boost::iostreams::base64::base64decode());
        outbuf.push(stream_result);

        std::ostream out(&outbuf);
        std::noskipws(out);

        std::copy(std::istream_iterator<char>(stream_decoded), std::istream_iterator<char>(),
                  std::ostream_iterator<char>(out));

        boost::iostreams::close(outbuf); // Don't forget this!

        std::cout << stream_result.str() << std::endl;
    }
    {
        // test input filter
        std::stringstream stream_result("");
        std::noskipws(stream_result);
        std::stringstream stream_decoded("QQ");
        std::noskipws(stream_decoded);

        boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
        inbuf.push(boost::iostreams::base64::base64decode());
        inbuf.push(stream_decoded);

        std::istream in(&inbuf);
        std::noskipws(in);

        std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(),
                  std::ostream_iterator<char>(stream_result));
        //Cleanup
        boost::iostreams::close(inbuf); // Don't forget this!

        std::cout << stream_result.str() << std::endl;
    }
    {
        // test input filter
        std::stringstream stream_result("");
        std::stringstream stream_encoded("A");

        boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
        inbuf.push(boost::iostreams::base64::base64encode());
        inbuf.push(stream_encoded);

        std::istream in(&inbuf);

        std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(),
                  std::ostream_iterator<char>(stream_result));
        //Cleanup
        boost::iostreams::close(inbuf); // Don't forget this!

        std::cout << stream_result.str() << std::endl;
    }
    {
        // test output filter
        std::stringstream stream_result("");
        std::stringstream stream_encoded("A");
        std::noskipws(stream_encoded);

        boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;
        outbuf.push(boost::iostreams::base64::base64encode());
        outbuf.push(stream_result);

        std::ostream out(&outbuf);

        std::copy(std::istream_iterator<char>(stream_encoded), std::istream_iterator<char>(),
                  std::ostream_iterator<char>(out));

        boost::iostreams::close(outbuf); // Don't forget this!

        std::cout << stream_result.str() << std::endl;
    }

    return 0;
}
