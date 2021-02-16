//
// Created by shkul on 13.10.2020.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include "../base64.hpp"
#include "lib/base64.h"

const int largeSize = 32*1024*1024+1;
const int smallSize = 1024+1;

TEST_CASE ("large random encode input") {

    std::stringstream stream_result("");
    std::stringstream stream_encoded("");

    std::srand(std::time(0));
    for(int i=0;i<largeSize;i++) {
        char c = std::rand() % 256;
        stream_encoded.write(&c,sizeof(c));
    }

    boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
    inbuf.push(boost::iostreams::base64::base64encode());
    inbuf.push(stream_encoded);

    std::istream in(&inbuf);

    std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(),
              std::ostream_iterator<char>(stream_result));
    //Cleanup
    boost::iostreams::close(inbuf); // Don't forget this!

    std::string checked_result = base64_encode(stream_encoded.str());

    int c_result = checked_result.compare(stream_result.str());

    CHECK_EQ(c_result, 0);
}

TEST_CASE ("small random encode input") {

    std::srand(std::time(0));
    for(int size = 1;size<smallSize;size++) {
        std::stringstream stream_result("");
        std::stringstream stream_encoded("");

        for (int i = 0; i < size; i++) {
            char c = std::rand() % 256;
            stream_encoded.write(&c, sizeof(c));
        }

        boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
        inbuf.push(boost::iostreams::base64::base64encode());
        inbuf.push(stream_encoded);

        std::istream in(&inbuf);

        std::copy(std::istream_iterator<char>(in), std::istream_iterator<char>(),
                  std::ostream_iterator<char>(stream_result));
        //Cleanup
        boost::iostreams::close(inbuf); // Don't forget this!

        std::string checked_result = base64_encode(stream_encoded.str());

        int c_result = checked_result.compare(stream_result.str());
                CHECK_EQ(c_result, 0);
    }
}

TEST_CASE ("large random decode input") {

    std::stringstream stream_encoded("");
    std::srand(std::time(0));
    for(int i=0;i<largeSize;i++) {
        char c = std::rand() % 256;
        stream_encoded.write(&c,sizeof(c));
    }
    std::string checked_result = base64_encode(stream_encoded.str());

    std::stringstream stream_result("");
    std::noskipws(stream_result);
    std::stringstream stream_decoded(checked_result);
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

    int c_result = stream_encoded.str().compare(stream_result.str());

//    std::cout << c_result << std::endl;
//    std::cout << "Len: " << stream_encoded.str().length() << " Encoded: " << checked_result.length() << std::endl;
//    std::cout << "Filter: " << stream_decoded.str().length() << " Decoded: " << stream_result.str().length() << std::endl;

            CHECK_EQ(c_result, 0);


}

TEST_CASE ("small random decode input") {
    std::srand(std::time(0));
    for (int size = 1; size < smallSize; size++) {
        std::stringstream stream_encoded("");
        for (int i = 0; i < size; i++) {
            char c = std::rand() % 256;
            stream_encoded.write(&c, sizeof(c));
        }

        std::string checked_result = base64_encode(stream_encoded.str());

        std::stringstream stream_result("");
        std::noskipws(stream_result);
        std::stringstream stream_decoded(checked_result);
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

        int c_result = stream_encoded.str().compare(stream_result.str());

//        std::cout << "Size:" << size << " Result: " << c_result << std::endl;
//        std::cout << "Len: " << stream_encoded.str().length() << " Encoded: " << checked_result.length() << std::endl;
//        std::cout << "Filter: " << stream_decoded.str().length() << " Decoded: " << stream_result.str().length()
//                  << std::endl;

                CHECK_EQ(c_result, 0);
    }

}

TEST_CASE ("large random encode output") {
    std::srand(std::time(0));

    std::stringstream stream_encoded("");
    for(int i=0;i<largeSize;i++) {
        char c = std::rand() % 256;
        stream_encoded.write(&c,sizeof(c));
    }

    std::stringstream stream_result("");
    // test output filter
    std::noskipws(stream_encoded);

    boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;
    outbuf.push(boost::iostreams::base64::base64encode());
    outbuf.push(stream_result);

    std::ostream out(&outbuf);

    std::copy(std::istream_iterator<char>(stream_encoded), std::istream_iterator<char>(),
              std::ostream_iterator<char>(out));

    boost::iostreams::close(outbuf); // Don't forget this!

    std::string checked_result = base64_encode(stream_encoded.str());

    int c_result = checked_result.compare(stream_result.str());

            CHECK_EQ(c_result, 0);
//        std::cout << "Size:" << largeSize << " Result: " << c_result << std::endl;
//        std::cout << "Len: " << stream_encoded.str().length() << " Encoded: " << checked_result.length() << std::endl;
//        std::cout << "Filter: " << stream_result.str().length() << std::endl;

}

TEST_CASE ("small random encode output") {
    std::srand(std::time(0));
    for (int size = 1; size < smallSize; size++) {
        std::stringstream stream_encoded("");
        for (int i = 0; i < size; i++) {
            char c = std::rand() % 256;
            stream_encoded.write(&c, sizeof(c));
        }

        std::stringstream stream_result("");
        // test output filter
        std::noskipws(stream_encoded);

        boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;
        outbuf.push(boost::iostreams::base64::base64encode());
        outbuf.push(stream_result);

        std::ostream out(&outbuf);

        std::copy(std::istream_iterator<char>(stream_encoded), std::istream_iterator<char>(),
                  std::ostream_iterator<char>(out));

        boost::iostreams::close(outbuf); // Don't forget this!

        std::string checked_result = base64_encode(stream_encoded.str());

        int c_result = checked_result.compare(stream_result.str());

                CHECK_EQ(c_result, 0);
//        std::cout << "Size:" << largeSize << " Result: " << c_result << std::endl;
//        std::cout << "Len: " << stream_encoded.str().length() << " Encoded: " << checked_result.length() << std::endl;
//        std::cout << "Filter: " << stream_result.str().length() << std::endl;
    }
}

TEST_CASE ("large random decode output") {
    std::srand(std::time(0));
    std::stringstream stream_encoded("");
    for(int i=0;i<largeSize;i++) {
        char c = std::rand() % 256;
        stream_encoded.write(&c,sizeof(c));
    }
    std::string checked_result = base64_encode(stream_encoded.str());

    std::stringstream stream_result("");
    std::noskipws(stream_result);
    std::stringstream stream_decoded(checked_result);
    std::noskipws(stream_decoded);

    boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;
    outbuf.push(boost::iostreams::base64::base64decode());
    outbuf.push(stream_result);

    std::ostream out(&outbuf);
    std::noskipws(out);

    std::copy(std::istream_iterator<char>(stream_decoded), std::istream_iterator<char>(),
              std::ostream_iterator<char>(out));

    boost::iostreams::close(outbuf); // Don't forget this!

    int c_result = stream_encoded.str().compare(stream_result.str());

//        std::cout << "Size:" << largeSize << " Result: " << c_result << std::endl;
//        std::cout << "Len: " << stream_encoded.str().length() << " Encoded: " << checked_result.length() << std::endl;
//        std::cout << "Filter: " << stream_decoded.str().length() << " Decoded: " << stream_result.str().length()
//                  << std::endl;

            CHECK_EQ(c_result, 0);


}

TEST_CASE ("small random decode output") {
    std::srand(std::time(0));
    for (int size = 1; size < smallSize; size++) {
        std::stringstream stream_encoded("");
        for (int i = 0; i < size; i++) {
            char c = std::rand() % 256;
            stream_encoded.write(&c, sizeof(c));
        }

        std::string checked_result = base64_encode(stream_encoded.str());

        std::stringstream stream_result("");
        std::noskipws(stream_result);
        std::stringstream stream_decoded(checked_result);
        std::noskipws(stream_decoded);

        boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;
        outbuf.push(boost::iostreams::base64::base64decode());
        outbuf.push(stream_result);

        std::ostream out(&outbuf);
        std::noskipws(out);

        std::copy(std::istream_iterator<char>(stream_decoded), std::istream_iterator<char>(),
                  std::ostream_iterator<char>(out));

        boost::iostreams::close(outbuf); // Don't forget this!

        int c_result = stream_encoded.str().compare(stream_result.str());

//        std::cout << "Size:" << size << " Result: " << c_result << std::endl;
//        std::cout << "Len: " << stream_encoded.str().length() << " Encoded: " << checked_result.length() << std::endl;
//        std::cout << "Filter: " << stream_decoded.str().length() << " Decoded: " << stream_result.str().length()
//                  << std::endl;

                CHECK_EQ(c_result, 0);
    }
}

