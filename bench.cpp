#include <iostream>
#include <chrono>
#include <x86intrin.h>
#include <string>
#include "Field281.hpp"

FieldElement281 random_field_element() {
        std::string s = "";
        for (int i = 0; i < FieldElement281::M; ++i) {
                s += (rand() % 2 ? '1' : '0');
        }
        return FieldElement281::from_string_bin(s);
}

int main() {
        srand(2026);

        FieldElement281 a = random_field_element();
        FieldElement281 b = random_field_element();
        FieldElement281 exp_elem = random_field_element(); 
        FieldElement281 res;
        int trace_res = 0;

        {
                int iterations = 1000000;
                uint64_t c1 = __rdtsc();
                auto t1 = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < iterations; ++i) {
                        res = a + b;
                }
                auto t2 = std::chrono::high_resolution_clock::now();
                uint64_t c2 = __rdtsc();
                std::cout << "Field Add (+): " << (c2 - c1) / iterations << " cycles | " << std::chrono::duration<double, std::micro>(t2 - t1).count() / iterations << " us\n";
        }

        {
                int iterations = 10000;
                uint64_t c1 = __rdtsc();
                auto t1 = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < iterations; ++i) {
                        res = a * b;
                }
                auto t2 = std::chrono::high_resolution_clock::now();
                uint64_t c2 = __rdtsc();
                std::cout << "Field Mul (*): " << (c2 - c1) / iterations << " cycles | " << std::chrono::duration<double, std::micro>(t2 - t1).count() / iterations << " us\n";
        }

        {
                int iterations = 10000;
                uint64_t c1 = __rdtsc();
                auto t1 = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < iterations; ++i) {
                        res = a.square();
                }
                auto t2 = std::chrono::high_resolution_clock::now();
                uint64_t c2 = __rdtsc();
                std::cout << "Field Square (^2): " << (c2 - c1) / iterations << " cycles | " << std::chrono::duration<double, std::micro>(t2 - t1).count() / iterations << " us\n";
        }

        {
                int iterations = 1000;
                uint64_t c1 = __rdtsc();
                auto t1 = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < iterations; ++i) {
                        trace_res = a.trace();
                }
                auto t2 = std::chrono::high_resolution_clock::now();
                uint64_t c2 = __rdtsc();
                std::cout << "Field Trace (Tr): " << (c2 - c1) / iterations << " cycles | " << std::chrono::duration<double, std::micro>(t2 - t1).count() / iterations << " us\n";
        }

        {
                int iterations = 100;
                uint64_t c1 = __rdtsc();
                auto t1 = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < iterations; ++i) {
                        res = a.pow(exp_elem);
                }
                auto t2 = std::chrono::high_resolution_clock::now();
                uint64_t c2 = __rdtsc();
                std::cout << "Field Pow (^exp): " << (c2 - c1) / iterations << " cycles | " << std::chrono::duration<double, std::micro>(t2 - t1).count() / iterations << " us\n";
        }

        {
                int iterations = 100;
                if (a.is_zero()) {
                        a = FieldElement281::one();
                }
                uint64_t c1 = __rdtsc();
                auto t1 = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < iterations; ++i) {
                        res = a.inverse();
                }
                auto t2 = std::chrono::high_resolution_clock::now();
                uint64_t c2 = __rdtsc();
                std::cout << "Field Inverse: " << (c2 - c1) / iterations << " cycles | " << std::chrono::duration<double, std::micro>(t2 - t1).count() / iterations << " us\n";
        }

        return 0;
}
