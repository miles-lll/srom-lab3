#include <iostream>
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

        FieldElement281 o = FieldElement281::one();
        FieldElement281 z = FieldElement281::zero();

        FieldElement281 a = random_field_element();
        FieldElement281 b = random_field_element();
        FieldElement281 c = random_field_element();
        FieldElement281 d = random_field_element();

        std::cout << "a = " << a.to_string_bin() << "\n";
        std::cout << "b = " << b.to_string_bin() << "\n";
        std::cout << "c = " << c.to_string_bin() << "\n";
        std::cout << "d = " << d.to_string_bin() << "\n\n";

        FieldElement281 left1  = (a + b) * c;
        FieldElement281 right1 = (b * c) + (c * a);

        std::cout << "(a + b) * c (bin)       = " << left1.to_string_bin() << "\n";
        std::cout << "b * c + c * a (bin)     = " << right1.to_string_bin() << "\n";
        if (left1 == right1) {
                std::cout << "Modular distributivity test passed!\n\n";
        } else {
                std::cout << "Modular distributivity test failed!\n\n";
        }


        if (!d.is_zero()) {
                FieldElement281 fermat_exp = FieldElement281::from_string_bin(std::string(FieldElement281::M, '1'));
                FieldElement281 fermat_res = d.pow(fermat_exp);

                std::cout << "Fermat test (d^(2^281 - 1) == 1):\n";
                std::cout << "Result: " << fermat_res.to_string_bin() << "\n";
                if (fermat_res == o) {
                        std::cout << "Fermat's Little Theorem test passed!\n\n";
                } else {
                        std::cout << "Fermat's Little Theorem test failed!\n\n";
                }
        }


        if (!a.is_zero()) {
                FieldElement281 inv_a = a.inverse();
                FieldElement281 inv_check = a * inv_a;

                std::cout << "Inverse test (a * a^-1 == 1):\n";
                std::cout << "a^-1     = " << inv_a.to_string_bin() << "\n";
                std::cout << "a * a^-1 = " << inv_check.to_string_bin() << "\n";
                if (inv_check == o) {
                        std::cout << "Inverse identity test passed!\n\n";
                } else {
                        std::cout << "Inverse identity test failed!\n\n";
                }
        }


        int tr_a = a.trace();
        int tr_b = b.trace();
        int tr_sum = (a + b).trace();

        std::cout << "Trace linearity test (Tr(a + b) == Tr(a) ^ Tr(b)):\n";
        std::cout << "Tr(a) = " << tr_a << ", Tr(b) = " << tr_b << ", Tr(a+b) = " << tr_sum << "\n";
        if (tr_sum == (tr_a ^ tr_b)) {
                std::cout << "Trace linearity test passed!\n\n";
        } else {
                std::cout << "Trace linearity test failed!\n\n";
        }


        int tr_sq = a.square().trace();
        std::cout << "Trace of square test (Tr(a^2) == Tr(a)):\n";
        std::cout << "Tr(a) = " << tr_a << ", Tr(a^2) = " << tr_sq << "\n";
        if (tr_sq == tr_a) {
                std::cout << "Trace square identity test passed!\n\n";
        } else {
                std::cout << "Trace square identity test failed!\n\n";
        }


        FieldElement281 sum_sq = (a + b).square();
        FieldElement281 sq_sum = a.square() + b.square();

        std::cout << "(a + b)^2 == a^2 + b^2:\n";
        std::cout << "(a + b)^2 = " << sum_sq.to_string_bin() << "\n";
        std::cout << "a^2 + b^2 = " << sq_sum.to_string_bin() << "\n";
        if (sum_sq == sq_sum) {
                std::cout << "Test passed!\n\n";
        } else {
                std::cout << "Test failed!\n\n";
        }

        return 0;
}
