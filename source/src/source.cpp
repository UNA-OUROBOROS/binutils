#include <BytesSerializer.hpp>
#include <fstream>
#include <iostream>
#include <string>

struct Foo {
	int32_t numero;
	char caracter;
};

template <> Bytes toBytes(Foo val) {
	Bytes bytes;
	appendBytes(toBytes(val.numero), bytes);
	appendBytes(toBytes(val.caracter), bytes);
	return bytes;
}

template <> Foo fromBytes(BytesIterator &first, BytesIterator &last) {
	return {fromBytes<int32_t>(first, last), fromBytes<char>(first, last)};
}

int main(int, char **) {
	std::string nombreArchivo = "datos.bin";
	int32_t b = 1;
	char c = 'Z';
	std::cout << "b: " << b << "\n"
	          << "c: " << c << "\n";
	{

		std::ofstream archivo(nombreArchivo, std::ofstream::out |
		                                         std::ofstream::trunc |
		                                         std::ofstream::binary);

		if (archivo) {
			Foo a{b, c};

			archivo << toBytes(a);
		} else {
			std::cerr << "no se pudo abrir el archivo\n";
			return 0;
		}
	}
	{
		std::ifstream entrada(nombreArchivo,
		                      std::ifstream::in | std::ifstream::binary);

		if (entrada.is_open()) {
			Bytes bytes;
			entrada >> bytes;
			BytesIterator begin = bytes.cbegin();
			BytesIterator end = bytes.cend();
			Foo val = fromBytes<Foo>(begin, end);
			std::cout << "val {" << val.numero << ", " << val.caracter
			          << "}.\n";
		}
	}
}
