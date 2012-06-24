// Copyright (c) 2012 Plenluno All rights reserved.

#include <gtest/gtest.h>
#include <libnode/buffer.h>
#include <algorithm>
#include <cstring>

namespace libj {
namespace node {

TEST(GTestBuffer, TestCreate1) {
    Buffer::Ptr b = Buffer::create(10);
    ASSERT_EQ(b->length(), 10);
}

TEST(GTestBuffer, TestCreate2) {
    JsTypedArray<UByte>::Ptr a = JsTypedArray<UByte>::create();
    for (int i = 0; i < 11; i++)
        a->add(i);
    Buffer::Ptr b = Buffer::create(a);
    ASSERT_EQ(b->length(), 11);
}

TEST(GTestBuffer, TestCreate3) {
    char c1[] = "This is ASCII";
    char c2[] = {
        0xe3, 0x81, 0x93, 0xe3, 0x82, 0x8c, 0xe3, 0x81,
        0xaf, 0x55, 0x54, 0x46, 0x2d, 0x38, 0xe3, 0x81,
        0xa7, 0xe3, 0x81, 0x99, 0xe3, 0x80, 0x82, 0
    };  // これはUTF-8です。
    String::CPtr s1 = String::create(c1);
    String::CPtr s2 = String::create(c2, String::ASCII);
    String::CPtr s3 = String::create(c2, String::UTF8);
    Buffer::Ptr b1 = Buffer::create(s1);
    Buffer::Ptr b2 = Buffer::create(s2);
    Buffer::Ptr b3 = Buffer::create(s3);
    ASSERT_EQ(b1->length(), strlen(c1));
    ASSERT_EQ(b2->length(), strlen(c2));
    ASSERT_EQ(b3->length(), strlen(c2));
}

TEST(GTestBuffer, TestWrite1) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    for (int i = 0; i < len; i++)
        b->writeUInt8(i + 1, i);

    const char* cs[] = {
        "shoter than buffer",
        "just fit into buffer",
        "loooooonger than buffer"
    };
    for (int i = 0; i < 3; i++) {
        const char* c = cs[i];
        String::CPtr s = String::create(c);
        b->write(s);
        for (int j = 0; j < len; j++) {
            UByte d;
#ifdef LIBJ_USE_EXCEPTION
            d = b->readUInt8(i);
#else
            ASSERT_TRUE(b->readUInt8(&d, i));
#endif
            if (i < strlen(c)) {
                ASSERT_EQ(c[i], d);
            } else {
                ASSERT_EQ(i + 1, d);
            }
        }
    }
}

TEST(GTestBuffer, TestWrite2) {
    const int len = 20;
    char init[len];
    for (int i = 0; i < len; i++) init[i] = i + 1;
    String::CPtr init_str = String::create(init, String::ASCII, len);
    Buffer::Ptr b = Buffer::create(len);
    String::CPtr pstr = String::create("partical write");

    for (int plen = pstr->length(); plen; plen /= 2) {
        for (int ofs = 0; ofs + plen <= len; ofs++) {
            b->write(init_str);
            b->write(pstr, ofs);
            for (int i = 0; i < len; i++) {
                UByte d;
#ifdef LIBJ_USE_EXCEPTION
                d = b->readUInt8(ofs);
#else
                ASSERT_TRUE(b->readUInt8(&d, ofs));
#endif
                if (i < ofs || i > ofs + plen) {
                    ASSERT_EQ(i + 1, d);
                } else {
                    ASSERT_EQ(pstr->charAt(i - ofs), d);
                }
            }
        }
    }
}

TEST(GTestBuffer, TestWrite3) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    char u[] = {
        0xe3, 0x81, 0x84, 0xe3, 0x82, 0x8d, 0xe3, 0x81,
        0xaf, 0xe3, 0x81, 0xab, 0xe3, 0x81, 0xbb, 0xe3,
        0x81, 0xb8, 0xe3, 0x81, 0xa8, 0xe3, 0x81, 0xa1,
        0xe3, 0x82, 0x8a, 0xe3, 0x81, 0xac, 0xe3, 0x82,
        0x8b, 0xe3, 0x82, 0x92, 0   // enough longer than buffer
    };
    String::CPtr s = String::create(u, String::UTF8);
    b->write(s, 0, NO_POS, String::UTF8);
    for (int i = 0; i < len; i++) {
        UByte d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt8(ofs);
#else
        ASSERT_TRUE(b->readUInt8(&d, i));
#endif
        ASSERT_EQ(u[i], d);
    }
}

TEST(GTestBuffer, TestUInt8) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    for (int i = 0; i < len; i++) {
        ASSERT_TRUE(b->writeUInt8(3 * (i + 1), i));
    }
    for (int i = 0; i < len; i++) {
        UByte d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt8(i);
#else
        ASSERT_TRUE(b->readUInt8(&d, i));
#endif
        ASSERT_EQ(3 * (i + 1), d);
    }
}

TEST(GTestBuffer, TestInt8) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    for (int i = 0; i < len; i++) {
        ASSERT_TRUE(b->writeInt8(-4 * (i + 1), i));
    }
    for (int i = 0; i < len; i++) {
        Byte d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readInt8(i);
#else
        ASSERT_TRUE(b->readInt8(&d, i));
#endif
        ASSERT_EQ(-4 * (i + 1), d);
    }
}

TEST(GTestBuffer, TestUInt16LE) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    // write 1, 2, 3, ...
    b->writeUInt8(0x01, 0);
    UShort e = 0x0302;
    for (int i = 1; i + 1 < len; i += 2) {
        b->writeUInt16LE(i, e);
        e += 0x0202;
    }
    b->writeUInt8(e & 0xff, len - 1);
    // aligned read
    e = 0x0201;
    for (int i = 0; i + 1 < len; i += 2) {
        UShort d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt16LE(i);
#else
        ASSERT_TRUE(b->readUInt16LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e += 0x0202;
    }
    // non-aligned read
    e = 0x0302;
    for (int i = 1; i + 1 < len; i += 2) {
        UShort d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt16LE(i);
#else
        ASSERT_TRUE(b->readUInt16LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e += 0x0202;
    }
}

TEST(GTestBuffer, TestUInt16BE) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    // write 1, 2, 3, ...
    b->writeUInt8(0x01, 0);
    UShort e = 0x0203;
    for (int i = 1; i + 1 < len; i += 2) {
        b->writeUInt16LE(i, e);
        e += 0x0202;
    }
    b->writeUInt8(e >> 8, len - 1);
    // aligned read
    e = 0x0102;
    for (int i = 0; i + 1 < len; i += 2) {
        UShort d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt16LE(i);
#else
        ASSERT_TRUE(b->readUInt16LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e += 0x0202;
    }
    // non-aligned read
    e = 0x0203;
    for (int i = 1; i + 1 < len; i += 2) {
        UShort d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt16LE(i);
#else
        ASSERT_TRUE(b->readUInt16LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e += 0x0202;
    }
}

TEST(GTestBuffer, TestInt16LE) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    // write 0xff, 0xfe, 0xfd, ...
    b->writeInt8(0xff, 0);
    Short e = 0xfdfe;
    for (int i = 1; i + 1 < len; i += 2) {
        b->writeInt16LE(i, e);
        e -= 0x0202;
    }
    b->writeInt8(e & 0xff, len - 1);
    // aligned read
    e = 0xfeff;
    for (int i = 0; i + 1 < len; i += 2) {
        Short d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readInt16LE(i);
#else
        ASSERT_TRUE(b->readInt16LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e -= 0x0202;
    }
    // non-aligned read
    e = 0xfdfe;
    for (int i = 1; i + 1 < len; i += 2) {
        Short d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readInt16LE(i);
#else
        ASSERT_TRUE(b->readInt16LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e -= 0x0202;
    }
}

TEST(GTestBuffer, TestInt16BE) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    // write 0xff, 0xfe, 0xfd, ...
    b->writeInt8(0xff, 0);
    Short e = 0xfefd;
    for (int i = 1; i + 1 < len; i += 2) {
        b->writeInt16BE(i, e);
        e -= 0x0202;
    }
    b->writeInt8(e & 0xff, len - 1);
    // aligned read
    e = 0xfffe;
    for (int i = 0; i + 1 < len; i += 2) {
        Short d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readInt16BE(i);
#else
        ASSERT_TRUE(b->readInt16BE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e -= 0x0202;
    }
    // non-aligned read
    e = 0xfefd;
    for (int i = 1; i + 1 < len; i += 2) {
        Short d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readInt16BE(i);
#else
        ASSERT_TRUE(b->readInt16BE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e -= 0x0202;
    }
}

TEST(GTestBuffer, TestUInt32LE) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    // write 1, 2, 3, ...
    b->writeUInt16LE(0x0201, 0);
    UInt e = 0x06050403;
    for (int i = 2; i + 3 < len; i += 4) {
        b->writeUInt32LE(i, e);
        e += 0x04040404;
    }
    b->writeUInt16LE(e & 0xffff, len - 2);
    // aligned read
    e = 0x04030201;
    for (int i = 0; i + 3 < len; i += 4) {
        UInt d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt32LE(i);
#else
        ASSERT_TRUE(b->readUInt32LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e += 0x04040404;
    }
    // non-aligned read
    e = 0x05040302;
    for (int i = 1; i + 3 < len; i += 4) {
        UInt d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt32LE(i);
#else
        ASSERT_TRUE(b->readUInt32LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e += 0x04040404;
    }
}

TEST(GTestBuffer, TestUInt32BE) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    // write 1, 2, 3, ...
    b->writeUInt16BE(0x0102, 0);
    UInt e = 0x03040506;
    for (int i = 2; i + 3 < len; i += 4) {
        b->writeUInt32LE(i, e);
        e += 0x04040404;
    }
    b->writeUInt16BE(e >> 16, len - 2);
    // aligned read
    e = 0x01020304;
    for (int i = 0; i + 3 < len; i += 4) {
        UInt d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt32BE(i);
#else
        ASSERT_TRUE(b->readUInt32BE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e += 0x04040404;
    }
    // non-aligned read
    e = 0x02030405;
    for (int i = 1; i + 3 < len; i += 4) {
        UInt d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readUInt32BE(i);
#else
        ASSERT_TRUE(b->readUInt32BE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e += 0x04040404;
    }
}

TEST(GTestBuffer, TestInt32LE) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    // write 0xff, 0xfe, 0xfd, ...
    b->writeInt16LE(0xfeff, 0);
    Int e = 0xfafbfcfd;
    for (int i = 2; i + 3 < len; i += 4) {
        b->writeInt32LE(i, e);
        e -= 0x04040404;
    }
    b->writeInt16LE(e & 0xffff, len - 2);
    // aligned read
    e = 0xfcfdfeff;
    for (int i = 0; i + 3 < len; i += 4) {
        Int d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readInt32LE(i);
#else
        ASSERT_TRUE(b->readInt32LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e -= 0x04040404;
    }
    // non-aligned read
    e = 0xfbfcfdfe;
    for (int i = 1; i + 3 < len; i += 4) {
        Int d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readInt32LE(i);
#else
        ASSERT_TRUE(b->readInt32LE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e -= 0x04040404;
    }
}

TEST(GTestBuffer, TestInt32BE) {
    const int len = 20;
    Buffer::Ptr b = Buffer::create(len);
    // write 0xff, 0xfe, 0xfd, ...
    b->writeInt16BE(0xfffe, 0);
    Int e = 0xfdfcfbfa;
    for (int i = 2; i + 3 < len; i += 4) {
        b->writeInt32BE(i, e);
        e -= 0x04040404;
    }
    b->writeInt16BE(e >> 16, len - 2);
    // aligned read
    e = 0xfffefdfc;
    for (int i = 0; i + 3 < len; i += 4) {
        Int d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readInt32BE(i);
#else
        ASSERT_TRUE(b->readInt32BE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e -= 0x04040404;
    }
    // non-aligned read
    e = 0xfefdfcfb;
    for (int i = 1; i + 3 < len; i += 4) {
        Int d;
#ifdef LIBJ_USE_EXCEPTION
        d = b->readInt32BE(i);
#else
        ASSERT_TRUE(b->readInt32BE(&d, i));
#endif
        ASSERT_EQ(e, d);
        e -= 0x04040404;
    }
}

template <typename T>
static T byteSwap(T v) {
    union {
        T t;
        UByte b[sizeof(T)];
    } d;
    d.t = v;
    for (int i = 0; i < sizeof(T) / 2; i++) {
        std::swap(d.b[i], d.b[sizeof(T) - 1 - i]);
    }
    return d.t;
}

TEST(GTestBuffer, TestFloat1) {
    const int len = 20;
    char init[len];
    for (int i = 0; i < len; i++) init[i] = i + 1;
    String::CPtr init_str = String::create(init, String::ASCII, len);
    Buffer::Ptr b = Buffer::create(len);
    Float f = 1.2345;
    for (int i = 0; i + 3 < len; i++) {
        b->write(init_str);
        ASSERT_TRUE(b->writeFloatLE(f, i));
        for (int j = 0; j < len; j++) {
            if (j == i) {
                Float d;
#ifdef LIBJ_USE_EXCEPTION
                d = b->readFloatBE(j);
#else
                ASSERT_TRUE(b->readFloatBE(&d, j));
#endif
                ASSERT_EQ(byteSwap<Float>(f), d);
            } else if (j < i || j > i + 3) {
                UByte d;
#ifdef LIBJ_USE_EXCEPTION
                d = b->readInt8(j);
#else
                ASSERT_TRUE(b->readUInt8(&d, j));
#endif
                ASSERT_EQ(j + 1, d);
            }
        }
        f *= -6.789;
    }
}

TEST(GTestBuffer, TestFloat2) {
    const int len = 20;
    char init[len];
    for (int i = 0; i < len; i++) init[i] = i + 1;
    String::CPtr init_str = String::create(init, String::ASCII, len);
    Buffer::Ptr b = Buffer::create(len);
    Float f = 12.345;
    for (int i = 0; i + 3 < len; i++) {
        b->write(init_str);
        ASSERT_TRUE(b->writeFloatBE(f, i));
        for (int j = 0; j < len; j++) {
            if (j == i) {
                Float d;
#ifdef LIBJ_USE_EXCEPTION
                d = b->readFloatLE(j);
#else
                ASSERT_TRUE(b->readFloatLE(&d, j));
#endif
                ASSERT_EQ(byteSwap<Float>(f), d);
            } else if (j < i || j > i + 3) {
                UByte d;
#ifdef LIBJ_USE_EXCEPTION
                d = b->readInt8(j);
#else
                ASSERT_TRUE(b->readUInt8(&d, j));
#endif
                ASSERT_EQ(j + 1, d);
            }
        }
        f *= -0.6789;
    }
}

TEST(GTestBuffer, TestDouble1) {
    const int len = 20;
    char init[len];
    for (int i = 0; i < len; i++) init[i] = i + 1;
    String::CPtr init_str = String::create(init, String::ASCII, len);
    Buffer::Ptr b = Buffer::create(len);
    Double f = -123.45;
    for (int i = 0; i + 7 < len; i++) {
        b->write(init_str);
        ASSERT_TRUE(b->writeDoubleLE(f, i));
        for (int j = 0; j < len; j++) {
            if (j == i) {
                Double d;
#ifdef LIBJ_USE_EXCEPTION
                d = b->readDoubleBE(j);
#else
                ASSERT_TRUE(b->readDoubleBE(&d, j));
#endif
                ASSERT_EQ(byteSwap<Double>(f), d);
            } else if (j < i || j > i + 7) {
                UByte d;
#ifdef LIBJ_USE_EXCEPTION
                d = b->readInt8(j);
#else
                ASSERT_TRUE(b->readUInt8(&d, j));
#endif
                ASSERT_EQ(j + 1, d);
            }
        }
        f *= -0.6789;
    }
}

TEST(GTestBuffer, TestDouble2) {
    const int len = 20;
    char init[len];
    for (int i = 0; i < len; i++) init[i] = i + 1;
    String::CPtr init_str = String::create(init, String::ASCII, len);
    Buffer::Ptr b = Buffer::create(len);
    Double f = 0.00012345;
    for (int i = 0; i + 7 < len; i++) {
        b->write(init_str);
        ASSERT_TRUE(b->writeDoubleBE(f, i));
        for (int j = 0; j < len; j++) {
            if (j == i) {
                Double d;
#ifdef LIBJ_USE_EXCEPTION
                d = b->readDoubleLE(j);
#else
                ASSERT_TRUE(b->readDoubleLE(&d, j));
#endif
                ASSERT_EQ(byteSwap<Double>(f), d);
            } else if (j < i || j > i + 7) {
                UByte d;
#ifdef LIBJ_USE_EXCEPTION
                d = b->readInt8(j);
#else
                ASSERT_TRUE(b->readUInt8(&d, j));
#endif
                ASSERT_EQ(j + 1, d);
            }
        }
        f *= -678.9;
    }
}

TEST(GTestBuffer, TestSlice) {
    Buffer::Ptr org = Buffer::create(10);
    for (int i = 0; i < org->length(); i++)
        org->writeUInt8(i + 1, i);

    Buffer::Ptr sl = org->slice(0);
    ASSERT_EQ(org->length(), sl->length());
    for (int i = 0; i < sl->length(); i++) {
        UByte d;
#ifdef LIBJ_USE_EXCEPTION
        d = sl->readInt8(i);
#else
        ASSERT_TRUE(sl->readUInt8(&d, i));
#endif
        ASSERT_EQ(i + 1, d);
    }
    sl = org->slice(5);
    ASSERT_EQ(5, sl->length());
    for (int i = 0; i < sl->length(); i++) {
        UByte d;
#ifdef LIBJ_USE_EXCEPTION
        d = sl->readInt8(i);
#else
        ASSERT_TRUE(sl->readUInt8(&d, i));
#endif
        ASSERT_EQ(i + 6, d);
    }
    sl = org->slice(3, 9);
    ASSERT_EQ(6, sl->length());
    for (int i = 0; i < sl->length(); i++) {
        UByte d;
#ifdef LIBJ_USE_EXCEPTION
        d = sl->readInt8(i);
#else
        ASSERT_TRUE(sl->readUInt8(&d, i));
#endif
        ASSERT_EQ(i + 4, d);
        ASSERT_TRUE(sl->writeUInt8(d + 10, i));     // modify org
    }
    for (int i = 0; i < org->length(); i++) {
        UByte d;
#ifdef LIBJ_USE_EXCEPTION
        d = org->readInt8(i);
#else
        ASSERT_TRUE(org->readUInt8(&d, i));
#endif
        if (i < 3 || i >= 9) {
            ASSERT_EQ(i + 1, d);
        } else {
            ASSERT_EQ(i + 11, d);
        }
    }
}

}  // namespace node
}  // namespace libj
