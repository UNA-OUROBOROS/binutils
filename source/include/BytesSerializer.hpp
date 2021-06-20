#pragma once
#include <cstdint>
#include <iosfwd>
#include <vector>

using Byte = std::uint8_t;
using Bytes = std::vector<Byte>;
using BytesIterator = Bytes::const_iterator;

// utiles
inline void appendBytes(Bytes source, Bytes &dest) {
	dest.insert(std::end(dest), std::begin(source), std::end(source));
}

// no se pueden tipos no especializados
template <typename T> Bytes toBytes(T val) = delete;
template <typename T>
T fromBytes(BytesIterator &first, BytesIterator &last) = delete;

// genericos
template <typename int_t, size_t bytes_n> inline Bytes toBytes(int_t val);
template <typename int_t, size_t bytes_n>
inline int_t fromBytes(BytesIterator &first, BytesIterator &last);
template <> inline Bytes toBytes<Byte>(Byte val);
template <>
inline Byte fromBytes<Byte>(BytesIterator &first, BytesIterator &Last);

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
template <> int8_t fromBytes<int8_t>(BytesIterator &first, BytesIterator &last);
template <>
int16_t fromBytes<int16_t>(BytesIterator &first, BytesIterator &last);
template <>
int32_t fromBytes<int32_t>(BytesIterator &first, BytesIterator &last);
template <>
int64_t fromBytes<int64_t>(BytesIterator &first, BytesIterator &last);
template <> float fromBytes<float>(BytesIterator &first, BytesIterator &last);
template <> double fromBytes<double>(BytesIterator &first, BytesIterator &last);
template <> bool fromBytes<bool>(BytesIterator &first, BytesIterator &last);
template <> char fromBytes<char>(BytesIterator &first, BytesIterator &last);

// sobrecarga de operador de ostream
std::ostream &operator<<(std::ostream &os, const Bytes &bytes);

// sobrecarga de operador de istream
std::istream &operator>>(std::istream &is, Bytes &bytes);