// Copyright (c) 2012 Plenluno All rights reserved.

#include "libnode/buffer.h"
#include <libj/js_array_buffer.h>
#include <libj/error.h>
#include <libj/exception.h>

namespace libj {
namespace node {

class BufferImpl : public Buffer {
 private:
    JsArrayBuffer::Ptr buf_;
    Size offset_;
    Size length_;

    template <typename T>
    Size adjustOffset(Size offset) const {
        if (offset + sizeof(T) - 1 < length_)
            return offset + offset_;
        else
            return buf_->length();   // out of range
    }

 public:
    BufferImpl(
        JsArrayBuffer::Ptr buf,
        Size begin = 0,
        Size end = NO_POS)
        : buf_(buf)
        , offset_(buf->length())
        , length_(0) {
        if (end > buf->length())
            end = buf->length();
        if (begin < end) {
            offset_ = begin;
            length_ = end - begin;
        }
    }

    Size length() const {
        return length_;
    }

    Ptr slice(Size begin) {
        return slice(begin, length_);
    }

    Ptr slice(Size begin, Size end) {
        Ptr p(new BufferImpl(buf_, begin + offset_, end + offset_));
        return p;
    }

    void write(
        String::CPtr str,
        Size offset,
        Size length,
        String::Encoding enc) {
        if (offset < length_) {
            if (offset + length > length_)
                length = length_ - offset;
            if (enc == String::UTF8) {
                std::string s = str->toStdString();
                if (length > s.length())
                    length = s.length();
                for (int i = 0; i < length; i++)
                    writeUInt8(s[i], offset + i);
            }
#if 0
            else if (enc == String::UTF16) {
                // TODO
            } else if (enc == String::UTF32) {
                // TODO
            }
#endif
            else {
                if (length > str->length())
                    length = str->length();
                for (int i = 0; i < length; i++)
                    writeUInt8(str->charAt(i), offset + i);
            }
        }
    }

#ifdef LIBJ_USE_EXCEPTION
    UByte readUInt8(Size offset) const {
        UByte d;
        if (readUInt8(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    UShort readUInt16LE(Size offset) const {
        UShort d;
        if (readUInt16LE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    UShort readUInt16BE(Size offset) const {
        UShort d;
        if (readUInt16BE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    UInt readUInt32LE(Size offset) const {
        UInt d;
        if (readUInt32LE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    UInt readUInt32BE(Size offset) const {
        UInt d;
        if (readUInt32BE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    Byte readInt8(Size offset) const {
        Byte d;
        if (readInt8(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    Short readInt16LE(Size offset) const {
        Short d;
        if (readInt16LE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    Short readInt16BE(Size offset) const {
        Short d;
        if (readInt16BE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    Int readInt32LE(Size offset) const {
        Int d;
        if (readInt32LE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    Int readInt32BE(Size offset) const {
        Int d;
        if (readInt32BE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    Float readFloatLE(Size offset) const {
        Float d;
        if (readFloatLE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    Float readFloatBE(Size offset) const {
        Float d;
        if (readFloatBE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    Double readDoubleLE(Size offset) const {
        Double d;
        if (readDoubleLE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }

    Double readDoubleBE(Size offset) const {
        Double d;
        if (readDoubleBE(&d, offset)) {
            return d;
        } else {
            LIBJ_THROW(Error::INDEX_OUT_OF_BOUNDS);
        }
    }
#endif  // LIBJ_USE_EXCEPTION

    Boolean readUInt8(UByte* value, Size offset) const {
        return buf_->getUInt8(adjustOffset<UShort>(offset), value);
    }

    Boolean readUInt16LE(UShort* value, Size offset) const {
        return buf_->getUInt16(adjustOffset<UShort>(offset), value, true);
    }

    Boolean readUInt16BE(UShort* value, Size offset) const {
        return buf_->getUInt16(adjustOffset<UShort>(offset), value, false);
    }

    Boolean readUInt32LE(UInt* value, Size offset) const {
        return buf_->getUInt32(adjustOffset<UInt>(offset), value, true);
    }

    Boolean readUInt32BE(UInt* value, Size offset) const {
        return buf_->getUInt32(adjustOffset<UInt>(offset), value, false);
    }

    Boolean readInt8(Byte* value, Size offset) const {
        return buf_->getInt8(adjustOffset<Byte>(offset), value);
    }

    Boolean readInt16LE(Short* value, Size offset) const {
        return buf_->getInt16(adjustOffset<Short>(offset), value, true);
    }

    Boolean readInt16BE(Short* value, Size offset) const {
        return buf_->getInt16(adjustOffset<Short>(offset), value, false);
    }

    Boolean readInt32LE(Int* value, Size offset) const {
        return buf_->getInt32(adjustOffset<Int>(offset), value, true);
    }

    Boolean readInt32BE(Int* value, Size offset) const {
        return buf_->getInt32(adjustOffset<Int>(offset), value, false);
    }

    Boolean readFloatLE(Float* value, Size offset) const {
        return buf_->getFloat32(adjustOffset<Float>(offset), value, true);
    }

    Boolean readFloatBE(Float* value, Size offset) const {
        return buf_->getFloat32(adjustOffset<Float>(offset), value, false);
    }

    Boolean readDoubleLE(Double* value, Size offset) const {
        return buf_->getFloat64(adjustOffset<Double>(offset), value, true);
    }

    Boolean readDoubleBE(Double* value, Size offset) const {
        return buf_->getFloat64(adjustOffset<Double>(offset), value, false);
    }

    Boolean writeUInt8(UByte value, Size offset) {
        return buf_->setUInt8(adjustOffset<UByte>(offset), value);
    }

    Boolean writeUInt16LE(UShort value, Size offset) {
        return buf_->setUInt16(adjustOffset<UShort>(offset), value, true);
    }

    Boolean writeUInt16BE(UShort value, Size offset) {
        return buf_->setUInt16(adjustOffset<UShort>(offset), value, false);
    }

    Boolean writeUInt32LE(UInt value, Size offset) {
        return buf_->setUInt32(adjustOffset<UInt>(offset), value, true);
    }

    Boolean writeUInt32BE(UInt value, Size offset) {
        return buf_->setUInt32(adjustOffset<UInt>(offset), value, false);
    }

    Boolean writeInt8(Byte value, Size offset) {
        return buf_->setInt8(adjustOffset<Byte>(offset), value);
    }

    Boolean writeInt16LE(Short value, Size offset) {
        return buf_->setInt16(adjustOffset<Short>(offset), value, true);
    }

    Boolean writeInt16BE(Short value, Size offset) {
        return buf_->setInt16(adjustOffset<Short>(offset), value, false);
    }

    Boolean writeInt32LE(Int value, Size offset) {
        return buf_->setInt32(adjustOffset<Int>(offset), value, true);
    }

    Boolean writeInt32BE(Int value, Size offset) {
        return buf_->setInt32(adjustOffset<Int>(offset), value, false);
    }

    Boolean writeFloatLE(Float value, Size offset) {
        return buf_->setFloat32(adjustOffset<Float>(offset), value, true);
    }

    Boolean writeFloatBE(Float value, Size offset) {
        return buf_->setFloat32(adjustOffset<Float>(offset), value, false);
    }

    Boolean writeDoubleLE(Double value, Size offset) {
        return buf_->setFloat64(adjustOffset<Double>(offset), value, true);
    }

    Boolean writeDoubleBE(Double value, Size offset) {
        return buf_->setFloat64(adjustOffset<Double>(offset), value, false);
    }

    String::CPtr toString() const {
        // TODO
        String::CPtr s = String::create("");
        return s;
    }
};  // class BufferImpl

Buffer::Ptr Buffer::create(Size length) {
    JsArrayBuffer::Ptr buf = JsArrayBuffer::create(length);
    Buffer::Ptr p(new BufferImpl(buf, 0, length));
    return p;
}

Buffer::Ptr Buffer::create(JsTypedArray<UByte>::CPtr array) {
    JsArrayBuffer::Ptr buf = JsArrayBuffer::create(array->size());
    Iterator::Ptr it = array->iterator();
    for (int i = 0; it->hasNext(); i++) {
        UByte b;
        to<UByte>(it->next(), &b);
        buf->setUInt8(i, b);
    }
    Buffer::Ptr p(new BufferImpl(buf, 0, buf->length()));
    return p;
}

Buffer::Ptr Buffer::create(
    String::CPtr str, String::Encoding enc) {
    JsArrayBuffer::Ptr buf;
    if (enc == String::UTF8) {
        std::string s = str->toStdString();
        buf = JsArrayBuffer::create(s.length());
        for (int i = 0; i < s.length(); i++)
            buf->setInt8(i, s[i]);
    }
#if 0
    else if (enc == String::UTF16) {
        std::u16string s = str->toStdU16String();
        buf = JsArrayBuffer::create(s.length() * 2);
        for (int i = 0; i < s.length(); i++) 
            buf->setInt16(i * 2, s[i], true);
    else if (enc == String::UTF32) {
        std::u32string s = str->toStdU32String();
        buf = JsArrayBuffer::create(s.length() * 4);
        copy
        for (int i = 0; i < s.length(); i++) 
            buf->setInt32(i * 4, s[i], true);
    }
#endif
    else {
        buf = JsArrayBuffer::create(str->length());
        for (int i = 0; i < str->length(); i++) {
            UByte c = str->charAt(i);   // use only lower 8 bits
            buf->setUInt8(i, c);
        }
    }
    Buffer::Ptr p(new BufferImpl(buf, 0, buf->length()));
    return p;
}

}   // namespace node
}   // namespace libj
