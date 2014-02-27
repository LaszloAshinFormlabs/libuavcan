/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#include <sstream>
#include <iomanip>
#include <cassert>
#include <uavcan/data_type.hpp>

namespace uavcan
{
/*
 * DataTypeSignature
 */
void DataTypeSignature::mixin64(uint64_t x)
{
    DataTypeSignatureCRC crc = DataTypeSignatureCRC::extend(value_);
    for (int i = 0; i < 64; i += 8)   // LSB first
        crc.add((x >> i) & 0xFF);
    value_ = crc.get();
}

void DataTypeSignature::extend(DataTypeSignature dts)
{
    const uint64_t y = value_;
    mixin64(dts.get());
    mixin64(y);
}

TransferCRC DataTypeSignature::toTransferCRC() const
{
    TransferCRC tcrc;
    for (int i = 0; i < 64; i += 8)    // LSB first
        tcrc.add((value_ >> i) & 0xFF);
    return tcrc;
}

/*
 * DataTypeDescriptor
 */
std::string DataTypeDescriptor::toString() const
{
    char kindch = '?';
    switch (kind_)
    {
    case DataTypeKindMessage: kindch = 'm'; break;
    case DataTypeKindService: kindch = 's'; break;
    default: assert(0);
    }

    std::ostringstream os;
    os << name_ << ":" << id_ << kindch << ":" << std::hex << std::setfill('0') << std::setw(16) << signature_.get();
    return os.str();
}

}