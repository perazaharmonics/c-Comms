
#include <vector>
#include <complex>
#include <cmath>
#include <iostream>
#include<valarray>
#include <cstdint>
#include "FEC_codes.h"

using namespace::std

int main() {
	FEC_codes fec;

	// CRC processing
	uint32_t data[] = { 0x12345678, 0x9ABCDEF0 };  // Example data
	CRC_Data crcData;
	crcData.message_length_bytes = 8;  // Example: 8 bytes of data
	crcData.data_in = data;
	crcData.enc_flag = true;           // For encoding
	crcData.gen_poly = 0x04C11DB7;     // Example generator polynomial

	fec.processCRC(&crcData);
	std::cout << "Generated CRC: " << std::hex << crcData.crc_bits << std::endl;

	return 0;
}