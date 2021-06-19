#pragma once
#include <cstdint>
#include <iosfwd>
#include <vector>

using Byte = std::uint8_t;
using Bytes = std::vector<Byte>;

// utiles
inline void appendBytes(Bytes source, Bytes &dest) {
	dest.insert(std::end(dest), std::begin(source), std::end(source));
}

// no se pueden tipos no especializados
template <typename T> Bytes toBytes(T val) = delete;
template <typename T> T fromBytes(Bytes &bytes) = delete;

// genericos
template <typename int_t, size_t bytes_n> inline Bytes toBytes(int_t val);
template <typename int_t, size_t bytes_n> inline int_t fromBytes(Bytes &bytes);
template <> inline Bytes toBytes<Byte>(Byte val);
template <> inline Byte fromBytes<Byte>(Bytes &bytes);

// toBytes
// genericos

// especializaciones de primitivos
template <> Bytes toBytes<int8_t>(int8_t val);
template <> Bytes toBytes<int16_t>(int16_t val);
template <> Bytes toBytes<int32_t>(int32_t val);
template <> Bytes toBytes<int64_t>(int64_t val);
template <> Bytes toBytes<float>(float val);
template <> Bytes toBytes<double>(double val);
template <> Bytes toBytes<bool>(bool val);
template <> Bytes toBytes<char>(char val);

// fromBytes
// genericos

// especializaciones de primitivos
template <> int8_t fromBytes<int8_t>(Bytes &bytes);
template <> int16_t fromBytes<int16_t>(Bytes &bytes);
template <> int32_t fromBytes<int32_t>(Bytes &bytes);
template <> int64_t fromBytes<int64_t>(Bytes &bytes);
template <> float fromBytes<float>(Bytes &bytes);
template <> double fromBytes<double>(Bytes &bytes);
template <> bool fromBytes<bool>(Bytes &bytes);
template <> char fromBytes<char>(Bytes &bytes);

// sobrecarga de operador de ostream
std::ostream &operator<<(std::ostream &os, const Bytes &bytes);

// sobrecarga de operador de istream
std::istream &operator>>(std::istream &is, Bytes &bytes);