#include <vector>
#include <complex>
#include <cmath>
#include <iostream>
#include<valarray>
#include <cstdint>


// CRC Lookup Table
uint32_t CRC_LUT[256];

class HammingDistance {
public:
    virtual int distance() const = 0; // pure virtual function
    virtual ~HammingDistance() {}     // virtual destructor
};





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
    void generateCRCLUT(const CRC_Data* pCRC);

    void processBits(CRC_Data* pCRC, uint32_t& r0, uint32_t& r2, uint32_t bits_to_process) {
        _to_process) {
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

    // Compute the CRC.
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
        void generateCRCLUT(const CRC_Data * pCRC) {
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
///////////////////////////////////////////////////////////////////
// 
// Get (72, 64) Hamming Distance
// Applications: Single bit error-correction and double binary digit error detection.
// Author: Enrique Peraza
// 
///////////////////////////////////////////////////////////////////

class HammingDistance64 : public HammingDistance {
private:
    uint32_t data[3];
    static const uint32_t hm_error_table[72]; // Assuming you have this table
    statics const uint32 hm_masks[];
    uint32_t r7;   // Declare r7

    void computeR7(const uint32_t* hm_masks) {
        r7 = 0;
        uint32_t r0 = data[0];
        uint32_t r1 = data[1];
        uint32_t r2, r3;
        for (uint32_t i = 0; i < 8; i++) {
            r2 = hm_masks[2 * i];
            r3 = hm_masks[2 * i + 1];
            r2 = r0 & r2;
            r3 = r1 & r3;
            r2 = r2 ^ r3;
            r3 = r2 >> 16;
            r2 = r2 ^ r3;
            r3 = r2 >> 8;
            r2 = r2 ^ r3;
            r3 = r2 >> 4;
            r2 = r2 ^ r3;
            r3 = r2 >> 2;
            r2 = r2 ^ r3;
            r3 = r2 >> 1;
            r2 = r2 ^ r3;
            r2 = r2 & 1;
            r2 = r2 << i;
            r7 = r7 | r2;
            r6 = data[2];
            r6 = r6 ˆ r7;
            r6 = r6 >> 24;
            r4 = r6 & 0x80;
            r6 = r6 & 0x7f;
            j = 0; // assume no errors
            if (r6 != 0) {
                if ((r4 == 0x80) & (r6 != 0)) { // correct single bit errors
                    if (r6 < 72) {
                        r5 = hm_error_table[r6];
                        if (r5 < 32) {
                            r5 = 31 - r5;
                            r4 = 1 << r5;
                            data[0] = data[0] ˆ r4;
                        }
                        else if (r5 < 64) {
                            r5 = r5 - 32;
                            r5 = 31 - r5;
                            r4 = 1 << r5;
                            data[1] = data[1] ˆ r4;
                        }
                    }
                    else
                        j = 2; // multiple errors
                }
                else
                    j = 1; // double bit error detected
            }
        }

    }

};


////////////////////////////////////////////////////////////////////////////////////////////////////
// BCH Coding. (Can detect and correct code.
// cs.)
// 
// Author: Enrique Peraza
//
//////////////////////////////////////////////////////////////////////////////////////////////


class BCHDecoder {
private:
    std::vector<int> Galois_Log;
    std::vector<int> Galois_aLog;
    int T, N;

public:
    BCHDecoder(const std::vector<int>& gLog, const std::vector<int>& gaLog, int t, int n)
        : Galois_Log(gLog), Galois_aLog(gaLog), T(t), N(n) {}

    std::vector<int> computeErrorLocator(const std::vector<int>& Syndromes) {
        int L = 0;
        std::vector<int> Elp(2 * T, 0);
        std::vector<int> Tx(2 * T, 0);
        Elp[0] = 1; Tx[0] = 1;

        int r0 = Syndromes[0];
        for (int k = 0; k < 2 * T; k++) {
            std::vector<int> Conn_poly = Elp;
            if (r0 != 0) {
               k = 0;
               for (i = 127; i >= 1; i--) {
                   r0 = Elp[0];
                   for (j = 1; j < L + 1; j++) {
                       r1 = i * j;
                       r2 = r1 >> 7; r1 = r1 & 0x7f;
                       r3 = log_Elp[j]; r1 = r1 + r2;
                       r2 = r1 >> 7; r1 = r1 & 0x7f;
                       r1 = r1 + r2;
                       r1 = r1 + r3;
                       r2 = Galois_aLog[r1];
                       r0 = r0 ^ r2;
                   }
                   if (r0 == 0) {
                       Error_positions[k] = 127 - i;
                       k++;
                   }
               }
            }
        }
        return Elp; // return error-locator polynomial
    }

    std::vector<int> correctData(const std::vector<int>& Elp, const std::vector<int>& input_data) {
        int k = 0;
        std::vector<int> Error_positions(2 * T, 0);
        std::vector<int> data = input_data; // Create a copy of the input data

        for (int i = 127; i >= 1; i--) {
            int r0 = Elp[0];
            for (int j = 1; j < T + 1; j++) {
                int r1 = i * j;
                int r2 = r1 >> 7;
                r1 = r1 & 0x7f;
                int r3 = Galois_Log[Elp[j]];
                r1 = r1 + r2;
                r2 = r1 >> 7; r1 = r1 & 0x7f;
                r1 = r1 + r2;
                r1 = r1 + r3;
                r2 = Galois_aLog[r1];
                r0 = r0 ^ r2;
            }
            if (r0 == 0) {
                Error_positions[k] = 127 - i;
                k++;
            }
        }

        for (int i = 0; i < k; i++) {
            int m = Error_positions[i];
            int index = N - 1 - m;
            data[index] = data[index] ^ 1;
        }

        return data; // return corrected data-frame
    }
};
 public:
            HammingDistance64(uint32_t r0, uint32_t r1, uint32_t r2, const uint32_t* hm_masks) {
                data[0] = r0;
                data[1] = r1;
                data[2] = r2;
                computeR7(hm_masks);   // Compute r7 during object construction
            }
    // Define static member outside the class.
    uint32_t FEC_codes::CRC_LUT[256] = { 0 };