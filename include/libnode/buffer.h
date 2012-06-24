// Copyright (c) 2012 Plenluno All rights reserved.

#ifndef LIBNODE_BUFFER_H_
#define LIBNODE_BUFFER_H_

#include <libj/js_typed_array.h>

namespace libj {
namespace node {

class Buffer : LIBJ_MUTABLE(Buffer)
 public:
    static Ptr create(Size length);
    static Ptr create(JsTypedArray<UByte>::CPtr array);
    static Ptr create(String::CPtr str, String::Encoding enc = String::UTF8);

    virtual Size length() const = 0;

    virtual Ptr slice(Size begin) = 0;
    virtual Ptr slice(Size begin, Size end) = 0;

    virtual void write(
        String::CPtr str,
        Size offset = 0,
        Size length = NO_POS,
        String::Encoding enc = String::UTF8) = 0;

#ifdef LIBJ_USE_EXCEPTION
    virtual UByte readUInt8(Size offset) const = 0;
    virtual UShort readUInt16LE(Size offset) const = 0;
    virtual UShort readUInt16BE(Size offset) const = 0;
    virtual UInt readUInt32LE(Size offset) const = 0;
    virtual UInt readUInt32BE(Size offset) const = 0;
    virtual Byte readInt8(Size offset) const = 0;
    virtual Short readInt16LE(Size offset) const = 0;
    virtual Short readInt16BE(Size offset) const = 0;
    virtual Int readInt32LE(Size offset) const = 0;
    virtual Int readInt32BE(Size offset) const = 0;
    virtual Float readFloatLE(Size offset) const = 0;
    virtual Float readFloatBE(Size offset) const = 0;
    virtual Double readDoubleLE(Size offset) const = 0;
    virtual Double readDoubleBE(Size offset) const = 0;
#endif  // LIBJ_USE_EXCEPTION

    virtual Boolean readUInt8(UByte* value, Size offset) const = 0;
    virtual Boolean readUInt16LE(UShort* value, Size offset) const = 0;
    virtual Boolean readUInt16BE(UShort* value, Size offset) const = 0;
    virtual Boolean readUInt32LE(UInt* value, Size offset) const = 0;
    virtual Boolean readUInt32BE(UInt* value, Size offset) const = 0;
    virtual Boolean readInt8(Byte* value, Size offset) const = 0;
    virtual Boolean readInt16LE(Short* value, Size offset) const = 0;
    virtual Boolean readInt16BE(Short* value, Size offset) const = 0;
    virtual Boolean readInt32LE(Int* value, Size offset) const = 0;
    virtual Boolean readInt32BE(Int* value, Size offset) const = 0;
    virtual Boolean readFloatLE(Float* value, Size offset) const = 0;
    virtual Boolean readFloatBE(Float* value, Size offset) const = 0;
    virtual Boolean readDoubleLE(Double* value, Size offset) const = 0;
    virtual Boolean readDoubleBE(Double* value, Size offset) const = 0;

    virtual Boolean writeUInt8(UByte value, Size offset) = 0;
    virtual Boolean writeUInt16LE(UShort value, Size offset) = 0;
    virtual Boolean writeUInt16BE(UShort value, Size offset) = 0;
    virtual Boolean writeUInt32LE(UInt value, Size offset) = 0;
    virtual Boolean writeUInt32BE(UInt value, Size offset) = 0;
    virtual Boolean writeInt8(Byte value, Size offset) = 0;
    virtual Boolean writeInt16LE(Short value, Size offset) = 0;
    virtual Boolean writeInt16BE(Short value, Size offset) = 0;
    virtual Boolean writeInt32LE(Int value, Size offset) = 0;
    virtual Boolean writeInt32BE(Int value, Size offset) = 0;
    virtual Boolean writeFloatLE(Float value, Size offset) = 0;
    virtual Boolean writeFloatBE(Float value, Size offset) = 0;
    virtual Boolean writeDoubleLE(Double value, Size offset) = 0;
    virtual Boolean writeDoubleBE(Double value, Size offset) = 0;
};

}   // namespace node
}   // namespace libj

#endif  // LIBNODE_BUFFER_H_
