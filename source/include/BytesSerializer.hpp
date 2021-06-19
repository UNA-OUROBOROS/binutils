#pragma once
#include <cstdint>
#include <iosfwd>
#include <vector>

using Byte = std::uint8_t;
using Bytes = std::vector<Byte>;

// no se pueden tipos no especializados
template <typename T> Bytes toBytes(T val) = delete;
template <typename T> T fromBytes(Bytes &bytes) = delete;

// especializaciones de primitivos
template <> Bytes toBytes<int32_t>(int32_t val);
template <> Bytes toBytes<char>(char val);
template <> Bytes toBytes<Byte>(Byte val);

// especializaciones de primitivos
template <> int32_t fromBytes<int32_t>(Bytes &bytes);
template <> char fromBytes<char>(Bytes &bytes);
template <> Byte fromBytes<Byte>(Bytes &bytes);

// sobrecarga de operador de ostream
std::ostream &operator<<(std::ostream &os, const Bytes &bytes);

// sobrecarga de operador de istream
std::istream &operator>>(std::istream &is, Bytes &bytes);