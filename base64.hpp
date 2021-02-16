// (C) Copyright 2021 Sergey Shkuliov (sergey dot sckuliov at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Contains the definitions of the class templates base64decode and
// base64encode filter for reading and writing data in the base64 format.
// Based in part on work of Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Created by Sergey Shkuliov on 08.02.2021.
//

#ifndef FILTERBASE64_BASE64_HPP
#define FILTERBASE64_BASE64_HPP

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/operations.hpp>

namespace boost {
namespace iostreams {
namespace base64 {

    using namespace std;
    namespace io = boost::iostreams;

    char const* get_alphabet() {
        static char constexpr tab[] = {
                "ABCDEFGHIJKLMNOP"
                "QRSTUVWXYZabcdef"
                "ghijklmnopqrstuv"
                "wxyz0123456789+/"
        };
        return &tab[0];
    }

    signed char const* get_inverse() {
        static signed char constexpr tab[] = {
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //   0-15
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //  16-31
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, //  32-47
                52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, //  48-63
                -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, //  64-79
                15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, //  80-95
                -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, //  96-111
                41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // 112-127
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 128-143
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 144-159
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 160-175
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 176-191
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 192-207
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 208-223
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 224-239
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  // 240-255
        };
        return &tab[0];
    }

    class base64encode {
    public:
        typedef char char_type;
        struct category : multichar_dual_use_filter_tag, closable_tag {};

        template<typename Source>
        streamsize read(Source &src, char_type *s, streamsize n) {
            streamsize result = 0;
            if (n < 4) {
                return result;
            }
            auto const tab = get_alphabet();
            while (n - result >= 4) {
                streamsize readed = io::read(src, &c3[part_size_c3], 3 - part_size_c3);
                if (readed <= 0) {
                    if (readed < 0) {
                        result += finished_read(s);
                    }
                    return (result == 0 && readed < 0) ? readed : result;
                }
                if (part_size_c3 + readed < 3) {
                    part_size_c3 += readed;
                    if(io::detail::true_eof(src)) {
                        result += finished_read(s);
                    }
                    return result;
                }
                *s++ = tab[(c3[0] & 0xfc) >> 2];
                *s++ = tab[((c3[0] & 0x03) << 4) + ((c3[1] & 0xf0) >> 4)];
                *s++ = tab[((c3[2] & 0xc0) >> 6) + ((c3[1] & 0x0f) << 2)];
                *s++ = tab[c3[2] & 0x3f];
                part_size_c3 = 0;
                result += 4;
            }
            return result;
        }

        template<typename Sink>
        streamsize write(Sink &snk, const char_type *s, streamsize n) {
            streamsize result = 0;
            auto const tab = get_alphabet();
            if (part_size_c3 + n >= 3) {
                if (part_size_c3 > 0) {
                    memcpy(&c3[part_size_c3], s, 3 - part_size_c3);
                    c4[0] = tab[(c3[0] & 0xfc) >> 2];
                    c4[1] = tab[((c3[0] & 0x03) << 4) + ((c3[1] & 0xf0) >> 4)];
                    c4[2] = tab[((c3[2] & 0xc0) >> 6) + ((c3[1] & 0x0f) << 2)];
                    c4[3] = tab[c3[2] & 0x3f];
                    //TODO maybe check write size
                    result += 3 - part_size_c3;
                    part_size_c3 = 0;
                    io::write(snk, c4, 4);
                    s += result;
                }
            }
            while (n - result >= 3) {
                c4[0] = tab[(s[0] & 0xfc) >> 2];
                c4[1] = tab[((s[0] & 0x03) << 4) + ((s[1] & 0xf0) >> 4)];
                c4[2] = tab[((s[2] & 0xc0) >> 6) + ((s[1] & 0x0f) << 2)];
                c4[3] = tab[s[2] & 0x3f];
                s += 3;
                io::write(snk, c4, 4);
                result += 3;
            }
            if (n - result > 0) {
                memcpy(&c3[part_size_c3], s, n - result);
                part_size_c3 += n - result;
                result += n - result;
            }
            return result;
        }

        template<typename Sink>
        void close(Sink &snk, std::ios_base::openmode mode) {
            if (mode != std::ios_base::in) {
                streamsize result = finished_read(c4);
                if (result > 0) {
                    io::write(snk, c4, result);
                }
            }
        }

    private:
        streamsize finished_read(char_type *s) {
            streamsize result = 0;
            auto const tab = get_alphabet();
            switch (part_size_c3) {
                case 2:
                    *s++ = tab[(c3[0] & 0xfc) >> 2];
                    *s++ = tab[((c3[0] & 0x03) << 4) + ((c3[1] & 0xf0) >> 4)];
                    *s++ = tab[(c3[1] & 0x0f) << 2];
                    *s++ = '=';
                    result += 4;
                    break;
                case 1:
                    *s++ = tab[(c3[0] & 0xfc) >> 2];
                    *s++ = tab[((c3[0] & 0x03) << 4)];
                    *s++ = '=';
                    *s++ = '=';
                    result += 4;
                    break;
                case 0:
                    break;
            }
            part_size_c3 = 0;
            return result;
        }

    private:
        streamsize part_size_c3{0};
        char c3[3]{0, 0, 0};
        char c4[4]{0, 0, 0,0};
    };

    class base64decode {
    public:
        typedef char char_type;
        struct category : multichar_dual_use_filter_tag, closable_tag {};

        template<typename Source>
        streamsize read(Source &src, char_type *s, streamsize n) {
            streamsize result = 0;
            auto const inverse = base64::get_inverse();
            while (n - result >= 3) {
                streamsize readed = io::read(src, &c4[part_size_c4], 4 - part_size_c4);
                if (readed <= 0) {
                    if (readed < 0) {
                        result += finished_read(s);
                    }
                    return (result == 0 && readed < 0) ? readed : result;
                }
                if (part_size_c4 + readed < 4) {
                    part_size_c4 += readed;
                    if(io::detail::true_eof(src)) {
                        result += finished_read(s);
                    }
                    return result;
                }
                for(int i=0;i<4;i++) {
                    if(c4[i]=='=') {
                        part_size_c4 = i;
                        result += finished_read(s);
                        return result;
                    }
                    if(inverse[c4[i]]<0) {
                        return result;
                    }
                }
                *s++ = (inverse[c4[0]]        << 2) + ((inverse[c4[1]] & 0x30) >> 4);
                *s++ = ((inverse[c4[1]] & 0xf) << 4) + ((inverse[c4[2]] & 0x3c) >> 2);
                *s++ = ((inverse[c4[2]] & 0x3) << 6) +   inverse[c4[3]];
                part_size_c4 = 0;
                result += 3;
            }
            return result;
        }

        template<typename Sink>
        streamsize write(Sink &snk, const char_type *s, streamsize n) {
            streamsize result = 0;
            auto const inverse = base64::get_inverse();
            if (part_size_c4 + n >= 4) {
                if (part_size_c4 > 0) {
                    memcpy(&c4[part_size_c4], s, 4 - part_size_c4);
                    result += 4 - part_size_c4;
                    streamsize c = finished_read(c3);
                    //TODO may be check write size
                    io::write(snk, c3, c);
                    s += result;
                }
            }
            while (n - result >= 4) {
                memcpy(&c4[part_size_c4], s, 4);
                part_size_c4 = 4;
                streamsize c = finished_read(c3);
                s += 4;
                io::write(snk, c3, c);
                result += 4;
            }
            if (n - result > 0) {
                memcpy(&c4[part_size_c4], s, n - result);
                part_size_c4 += n - result;
                result += n - result;
            }
            return result;
        }

        template<typename Sink>
        void close(Sink &snk, std::ios_base::openmode mode) {
            if (mode != std::ios_base::in) {
                streamsize result = finished_read(c4);
                if (result > 0) {
                    io::write(snk, c4, result);
                }
            }
        }

    private:
        streamsize finished_read(char_type *s) {
            streamsize result = 0;
            auto const inverse = base64::get_inverse();
            int i;
            for (i = 0; i < part_size_c4; i++) {
                if ((c4[i] == '=') || (inverse[c4[i]] < 0)) {
                    part_size_c4 = i;
                    break;
                }
            }
            switch (part_size_c4) {
                case 4:
                    *s++ = (inverse[c4[0]]        << 2) + ((inverse[c4[1]] & 0x30) >> 4);
                    *s++ = ((inverse[c4[1]] & 0xf) << 4) + ((inverse[c4[2]] & 0x3c) >> 2);
                    *s++ = ((inverse[c4[2]] & 0x3) << 6) +   inverse[c4[3]];
                    result += 3;
                    break;
                case 3:
                    *s++ =  (inverse[c4[0]]        << 2) + ((inverse[c4[1]] & 0x30) >> 4);
                    *s++ = ((inverse[c4[1]] & 0xf) << 4) + ((inverse[c4[2]] & 0x3c) >> 2);
//                    *s++ = ((inverse[c4[2]] & 0x3) << 6);
//                    result += 3;
                    result += 2;
                    break;
                case 2:
                    *s++ =  (inverse[c4[0]]        << 2) + ((inverse[c4[1]] & 0x30) >> 4);
//                    *s++ = ((inverse[c4[1]] & 0xf) << 4);
//                    result += 2;
                    result += 1;
                    break;
//                case 1:
//                    *s++ =  (inverse[c4[0]]        << 2);
//                    result += 1;
//                    break;
            }
            part_size_c4 = 0;
            return result;
        }

    private:
        streamsize part_size_c4{0};
        char c3[3]{0, 0, 0};
        char c4[4]{0, 0, 0,0};
    };

} // base64
} // iostreams
} // boost

#endif //FILTERBASE64_BASE64_HPP
