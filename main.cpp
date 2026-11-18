#include <iostream>
#include "Field281.hpp"

int main() {
        FieldElement281 z = FieldElement281::zero();
        FieldElement281 o = FieldElement281::one();

        std::cout << "Zero (bin):  " << z.to_string_bin() << "\n";
        std::cout << "One  (bin):  " << o.to_string_bin() << "\n";

        std::string sample_bin = "1011";
        FieldElement281 a = FieldElement281::from_string_bin(sample_bin);

        std::cout << "\na (bin): " << a.to_string_bin() << "\n";
        std::cout << "a (hex): "; 
        a.print_hex();

        std::string sample_bin2 = "1101";
        FieldElement281 b = FieldElement281::from_string_bin(sample_bin2);

        std::cout << "b (bin): " << b.to_string_bin() << "\n";
        std::cout << "b (hex): "; 
        b.print_hex();

        FieldElement281 sum = a + b;
        FieldElement281 mul = a * b;
        FieldElement281 sq  = a.square();

        std::cout << "a + b (bin): " << sum.to_string_bin() << "\n";
        std::cout << "a * b (bin): " << mul.to_string_bin() << "\n";
        std::cout << "a^2   (bin): " << sq.to_string_bin() << "\n";

        int tr = a.trace();
        std::cout << "\nTr(a): " << tr << "\n";

        FieldElement281 pow_u = a.pow_uint(5);
        std::cout << "a^5   (bin): " << pow_u.to_string_bin() << "\n";

        if (!a.is_zero()) {
                FieldElement281 inv = a.inverse();
                std::cout << "a^(-1)  (bin): " << inv.to_string_bin() << "\n";
                
                FieldElement281 check = a * inv;
                std::cout << "a * a^(-1) (bin): " << check.to_string_bin() << "\n";
        }

	std::string str_x280(281, '0');
        str_x280[0] = '1';
        FieldElement281 x280 = FieldElement281::from_string_bin(str_x280);

        std::string str_x(281, '0');
        str_x[279] = '1';
        FieldElement281 x_elem = FieldElement281::from_string_bin(str_x);

        FieldElement281 red_test = x280 * x_elem;
        std::cout << "\nx^280 * x (bin): " << red_test.to_string_bin() << "\n";

        std::string long_str1 = "1" + std::string(270, '0') + "1010101" + std::string(3, '1');
        std::string long_str2 = "1" + std::string(150, '0') + "111100001111" + std::string(115, '0') + "1";

        FieldElement281 a_long = FieldElement281::from_string_bin(long_str1);
        FieldElement281 b_long = FieldElement281::from_string_bin(long_str2);

	std::cout << "a_long (bin): " << a_long.to_string_bin() << "\n";
	std::cout << "b_long (bin): " << b_long.to_string_bin() << "\n";

        FieldElement281 prod_long = a_long * b_long;
        std::cout << "Long a * b (bin):\n" << prod_long.to_string_bin() << "\n";

        FieldElement281 fermat_exp = FieldElement281::from_string_bin(std::string(FieldElement281::M, '1'));
        FieldElement281 fermat_check = a_long.pow(fermat_exp);
        std::cout << "a_long^(2^281 - 1) (bin):\n" << fermat_check.to_string_bin() << "\n";

	return 0;
}
