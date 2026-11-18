#ifndef FIELD_281_HPP
#define FIELD_281_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <stdexcept>

class FieldElement281 {
public:
        static constexpr int M = 281;
        static constexpr int WORD_COUNT = (M + 31) / 32;

        static constexpr uint32_t TOP_MASK = (M % 32 == 0) ? 0xFFFFFFFFu : ((1u << (M % 32)) - 1u);

        uint32_t words[WORD_COUNT];

        FieldElement281() {
                for (int i = 0; i < WORD_COUNT; ++i) {
                        words[i] = 0;
                }
        }

        static FieldElement281 zero() {
                return FieldElement281();
        }

        static FieldElement281 one() {
                FieldElement281 res;
                res.words[0] = 1;
                return res;
        }

        void mask_top() {
                words[WORD_COUNT - 1] &= TOP_MASK;
        }

        bool get_bit(int i) const {
                if (i < 0 || i >= M) return false;
                int word_idx = i / 32;
                int bit_idx = i % 32;
                return (words[word_idx] >> bit_idx) & 1u;
        }

        void set_bit(int i, bool val) {
                if (i < 0 || i >= M) return;
                int word_idx = i / 32;
                int bit_idx = i % 32;
                if (val) {
                        words[word_idx] |= (1u << bit_idx);
                } else {
                        words[word_idx] &= ~(1u << bit_idx);
                }
        }

        FieldElement281 add(const FieldElement281& other) const {
                FieldElement281 res;
                for (int i = 0; i < WORD_COUNT; ++i) {
                        res.words[i] = words[i] ^ other.words[i];
                }
                res.mask_top();
                return res;
        }

        FieldElement281 operator+(const FieldElement281& other) const {
                return add(other);
        }

        FieldElement281 multiply(const FieldElement281& other) const {
                const int PROD_WORDS = WORD_COUNT * 2;
                uint32_t prod[PROD_WORDS];
                for (int i = 0; i < PROD_WORDS; ++i) {
                        prod[i] = 0;
                }

                for (int i = 0; i < M; ++i) {
                        if (get_bit(i)) {
                                for (int j = 0; j < M; ++j) {
                                        if (other.get_bit(j)) {
                                                int dest_bit = i + j;
                                                int w_idx = dest_bit / 32;
                                                int b_idx = dest_bit % 32;
                                                prod[w_idx] ^= (1u << b_idx);
                                        }
                                }
                        }
                }

                for (int k = 560; k >= 281; --k) {
                        int w_idx = k / 32;
                        int b_idx = k % 32;
                        if ((prod[w_idx] >> b_idx) & 1u) {
                                prod[w_idx] ^= (1u << b_idx);

                                int shift = k - 281;
                                int positions[4] = {shift + 9, shift + 4, shift + 1, shift};
                                for (int p : positions) {
                                        int pw_idx = p / 32;
                                        int pb_idx = p % 32;
                                        prod[pw_idx] ^= (1u << pb_idx);
                                }
                        }
                }

                FieldElement281 res;
                for (int i = 0; i < WORD_COUNT; ++i) {
                        res.words[i] = prod[i];
                }
                res.mask_top();
                return res;
        }

        FieldElement281 operator*(const FieldElement281& other) const {
                return multiply(other);
        }

        FieldElement281 square() const {
                return multiply(*this);
        }

        int trace() const {
                FieldElement281 t = *this;
                FieldElement281 sum = *this;
                for (int i = 1; i < M; ++i) {
                        t = t.square();
                        sum = sum + t;
                }
                return sum.is_zero() ? 0 : 1;
        }

        FieldElement281 pow(const FieldElement281& exponent) const {
                int highest_bit = -1;
                for (int i = M - 1; i >= 0; --i) {
                        if (exponent.get_bit(i)) {
                                highest_bit = i;
                                break;
                        }
                }

                if (highest_bit == -1) {
                        return one();
                }

                FieldElement281 res = *this;
                for (int i = highest_bit - 1; i >= 0; --i) {
                        res = res.square();
                        if (exponent.get_bit(i)) {
                                res = res * (*this);
                        }
                }
                return res;
        }

        FieldElement281 pow_uint(uint64_t exp) const {
                FieldElement281 res = one();
                FieldElement281 base = *this;
                while (exp > 0) {
                        if (exp & 1) {
                                res = res * base;
                        }
                        base = base.square();
                        exp >>= 1;
                }
                return res;
        }

        FieldElement281 inverse() const {
                if (is_zero()) {
                        throw std::domain_error("Inverse of zero element does not exist!");
                }
                if (*this == one()) {
                        return one();
                }

                FieldElement281 x = *this;
                for (int i = 0; i < M - 2; ++i) {
                        x = x.square() * (*this);
                }
                return x.square();
        }

        bool operator==(const FieldElement281& other) const {
                for (int i = 0; i < WORD_COUNT; ++i) {
                        if (words[i] != other.words[i]) return false;
                }
                return true;
        }

        bool operator!=(const FieldElement281& other) const {
                return !(*this == other);
        }

        bool is_zero() const {
                for (int i = 0; i < WORD_COUNT; ++i) {
                        if (words[i] != 0) return false;
                }
                return true;
        }

        std::string to_string_bin() const {
                std::string s = "";
                s.reserve(M);
                for (int bit = M - 1; bit >= 0; --bit) {
                        int word_idx = bit / 32;
                        int bit_idx = bit % 32;
                        bool val = (words[word_idx] >> bit_idx) & 1;
                        s += (val ? '1' : '0');
                }
                return s;
        }

        static FieldElement281 from_string_bin(const std::string& s) {
                FieldElement281 res;
                int len = s.length();

                for (char c : s) {
                        if (c != '0' && c != '1') {
                                throw std::invalid_argument("Invalid character!");
                        }
                }

                if (len > M) {
                        throw std::out_of_range("Binary string is too long for M = 281");
                }

                for (int i = 0; i < len; ++i) {
                        char c = s[len - 1 - i];
                        if (c == '1') {
                                int word_idx = i / 32;
                                int bit_idx = i % 32;
                                res.words[word_idx] |= (1u << bit_idx);
                        }
                }
                return res;
        }

	static FieldElement281 from_string_hex(const std::string& hex_s) {
		std::string bin_s = "";
		for (char c : hex_s) {
			int val = 0;
			if (c >= '0' && c <= '9') val = c - '0';
			else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
			else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
			else throw std::invalid_argument("Invalid HEX character!");

			for (int bit = 3; bit >= 0; --bit) {
                		bin_s += ((val >> bit) & 1) ? '1' : '0';
            		}
        	}
	        if (bin_s.length() > M) {
        		bin_s = bin_s.substr(bin_s.length() - M);
        	} else if (bin_s.length() < M) {
            		bin_s = std::string(M - bin_s.length(), '0') + bin_s;
        	}
        	return from_string_bin(bin_s);
    	}

        void print_hex() const {
                std::cout << "0x";
                for (int i = WORD_COUNT - 1; i >= 0; --i) {
                        std::cout << std::hex << std::setw(8) << std::setfill('0') << words[i];
                }
                std::cout << std::dec << std::setfill(' ') << std::endl;
        }
};

#endif
