#include <vector>
#include <complex>
#include <cmath>
#include <iostream>
#include<valarray>
#include <cstdint>

struct CRC_Data {
    uint32_t message_length;
    uint32_t crc_length;
    uint32_t gen_poly;
    const uint32_t* in_data;
    uint32_t extract_crc;
    bool enc_flag;
    uint32_t crc_bits;
};
// CRC Lookup Table
uint32_t CRC_LUT[256];

class FEC_Codes {

    // #32 bit CRC Verify and 32 bit CRC Compute function
    struct CRC_Data {
        uint32_t message_length;  // The length of the input message in bits.
        uint32_t crc_length;      // The number of CRC bits.
        uint32_t gen_poly;        // The generator polynomial.
        const uint32_t* in_data;  // Pointer to the input data.
        uint32_t extract_crc;     // A mask for extracting CRC bits.
        uint32_t crc_bits;        // The resulting CRC bits.
    };

    // Forward declarations of helper functions.
    void processBits(CRC_Data* pCRC, uint32_t& r0, uint32_t& r2, uint32_t bits_to_process);
    uint32_t computeCRC(CRC_Data* pCRC);
    uint32_t verifyCRC(CRC_Data* pCRC);

    // Helper function to process individual bits.
    void processBits(CRC_Data* pCRC, uint32_t& r0, uint32_t& r2, uint32_t bits_to_process) {
        uint32_t r1, r3, r4 = pCRC->gen_poly, tb = 1 << (pCRC->crc_length - 1);
        for (uint32_t i = 0; i < bits_to_process; i++) {
            r1 = r0 >> 31;            // Get the most significant bit of r0.
            r3 = r2 & tb;             // Check if the most significant bit of LFSR is set.
            r2 <<= 1;                 // Left shift the LFSR.
            r2 |= r1;                 // Append the MSB of r0 to the LFSR.
            if (r3) r2 ^= r4;         // If the MSB of LFSR was set, XOR with the generator polynomial.
            r0 <<= 1;                 // Left shift the message bit.
        }
    }

    // Function to compute the CRC.
    uint32_t computeCRC(CRC_Data* pCRC) {
        uint32_t r0 = *pCRC->in_data++, r2 = 0;
        uint32_t k = pCRC->message_length, m = k >> 5;
        // Process 32-bit chunks.
        for (uint32_t j = 0; j < m; j++) {
            processBits(pCRC, r0, r2, 32);
            r0 = *pCRC->in_data++;
        }
        // Process any remaining bits.
        processBits(pCRC, r0, r2, k - 32 * m);
        // Pass n zero bits for CRC computation.
        r0 = 0;
        processBits(pCRC, r0, r2, pCRC->crc_length);
        return r2 & pCRC->extract_crc;
    }

    // Function to verify the CRC.
    uint32_t verifyCRC(CRC_Data* pCRC) {
        uint32_t r0 = *pCRC->in_data++, r2 = 0, m = 0;
        uint32_t k = pCRC->message_length, total_bits = k + pCRC->crc_length;
        for (uint32_t i = 0; i < total_bits; i++) {
            processBits(pCRC, r0, r2, 1);
            m++;
            // If processed 32 bits, fetch the next chunk.
            if (m == 32) {
                r0 = *pCRC->in_data++;
                m = 0;
            }
        }
        return r2 & pCRC->extract_crc;



        // Function to generate the CRC Lookup Table
        void generateCRCLookupTable(const CRC_Data * pCRC) {
            uint32_t r0, r1, r2;

            r2 = pCRC->gen_poly; // Load generator polynomial

            // Generate CRC values for all 256 possible byte values
            for (uint32_t i = 0; i < 256; i++) {
                r0 = (i << 24); // Place the byte value in the MSB position

                // Process each bit of the byte
                for (uint32_t j = 0; j < 8; j++) {
                    r1 = r0 >> 31;    // Extract the most significant bit
                    r0 = r0 << 1;     // Shift left to process the next bit

                    // If the extracted MSB was 1, XOR with the generator polynomial
                    if (r1) r0 = r0 ^ r2;
                }

                // Store the resulting CRC in the lookup table
                CRC_LUT[i] = r0;
            }
        }



};

    // Define static member outside the class.
    uint32_t FEC_codes::CRC_LUT[256] = { 0 };
