#pragma once
#include <cstdint>
#include <iosfwd>
#include <iterator>
#include <string>
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
template <typename T> Bytes rawToBytes(T val);
template <typename T> T rawFromBytes(BytesIterator &first, BytesIterator &last);
template <> inline Bytes toBytes<Byte>(Byte val);
template <>
inline Byte fromBytes<Byte>(BytesIterator &first, BytesIterator &Last);

// toBytes
// genericos

// especializaciones de escalares
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

// especializaciones de escalares
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

// arreglos y cadenas

// toBytes
template <typename T> inline Bytes vectorToBytes(std::vector<T> val) {
	Bytes bytes;
	appendBytes(rawToBytes<size_t>(val.size()), bytes);
	for (T v : val) {
		appendBytes(toBytes(v), bytes);
	}
	return bytes;
}
template <> Bytes toBytes(std::string val);

// fromBytes

template <typename T>
inline std::vector<T> vectorFromBytes(BytesIterator &first,
                                      BytesIterator &last) {
	size_t len = rawFromBytes<size_t>(first, last);
	std::vector<T> vec;
	vec.reserve(len);
	for (size_t i = 0; i < len; i++) {
		vec.push_back(fromBytes<T>(first, last));
	}
	return vec;
}
template <> std::string fromBytes(BytesIterator &first, BytesIterator &last);

// sobrecarga de operador de ostream
std::ostream &operator<<(std::ostream &os, const Bytes &bytes);

// sobrecarga de operador de istream
std::istream &operator>>(std::istream &is, Bytes &bytes);