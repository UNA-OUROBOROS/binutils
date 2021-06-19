#include <BytesSerializer.hpp>
#include <iostream>
#include <fstream>


struct Foo {
	int32_t a;
	int32_t b;
};

int main(int, char **) {
	std::string nombreArchivo = "datos.bin";
	int32_t b = 0x0A1B2C3D;
	char c = 'Z';
	std::cout << "b: " << b << "\n"
	          << "c: " << c << "\n";
	{

		std::ofstream archivo(nombreArchivo, std::ofstream::out |
		                                         std::ofstream::trunc |
		                                         std::ofstream::binary);

		if (archivo) {
			Foo a;

			archivo << toBytes(b) << toBytes(c);
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
			char c1 = fromBytes<char>(bytes);
			int32_t b1 = fromBytes<int32_t>(bytes);
			std::cout << "b1: " << b1 << "\n"
			          << "c1: " << c1 << "\n";
		}
	}
}
