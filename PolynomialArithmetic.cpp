#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

void print(std::ostream &os, std::map<int, int> map) {
    std::map<int, int>::reverse_iterator p;
    if (map.empty()) {
        os << "0";
    }
    for (p = map.rbegin(); p != map.rend(); p++) {
        if (p->second == 0) {
            os << 0;
        } else {
            if (p->first == 0) {
                os << p->second;
            } else {
                if (p->second != 1) {
                    os << p->second << "X^" << p->first;
                } else {
                    os << "X^" << p->first;

                }
            }
            if (p != std::prev(map.rend())) os << " + ";
        }
    }
    os << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1]);
    std::ofstream output_file(argv[2]);

    std::string line;

    std::cout << "Reading " << argv[1] << "..." << std::endl;
    if (input_file.is_open()) {
        if (output_file.is_open()) {
            while (getline(input_file, line)) {
                std::stringstream first_polynomial(line);
                output_file << "Input 1: " << line << std::endl;

                getline(input_file, line);
                std::stringstream second_polynomial(line);
                output_file << "Input 2: " << line << std::endl;

                std::map<int, int> Polynomial, Polynomial2, sum, difference, product;
                int coefficient, exponent;

                output_file << "Polynomial 1 (Canonical): ";

                // Read first polynomial and keep it in reverse canonical form
                while (first_polynomial >> coefficient >> exponent) {
                    Polynomial[exponent] += coefficient;
                    if (Polynomial[exponent] == 0)
                        Polynomial.erase(exponent);
                }

                // print it in canonical form
                print(output_file, Polynomial);

                // Read second polynomial and keep it in reverse canonical form
                output_file << "Polynomial 2 (Canonical): ";
                while (second_polynomial >> coefficient >> exponent) {
                    Polynomial2[exponent] += coefficient;
                    if (Polynomial2[exponent] == 0)
                        Polynomial2.erase(exponent);
                }

                // print it in canonical form
                print(output_file, Polynomial2);

                sum = difference = Polynomial;
                std::map<int, int>::reverse_iterator p;

                // now add poly2
                for (p = Polynomial2.rbegin(); p != Polynomial2.rend(); p++) {
                    sum[p->first] += p->second;
                    if (sum[p->first] == 0)
                        sum.erase(p->first);
                }

                // print the sum of poly1 + poly2
                output_file << "Sum: ";
                print(output_file, sum);

                // now sub poly2
                for (p = Polynomial2.rbegin(); p != Polynomial2.rend(); p++) {
                    difference[p->first] -= p->second;
                    if (difference[p->first] == 0)
                        difference.erase(p->first);
                }

                // print the difference of poly1 - poly2
                output_file << "Difference: ";
                print(output_file, difference);

                // now multiply poly2
                for (p = Polynomial.rbegin(); p != Polynomial.rend(); p++) {
                    for (auto q = Polynomial2.rbegin(); q != Polynomial2.rend(); q++) {
                        product[p->first + q->first] += p->second * q->second;
                    }
                }

                // print the product of poly1 * poly2
                output_file << "Product: ";
                print(output_file, product);

                output_file << std::endl;
            }
            input_file.close();
            output_file.close();
            std::cout << "Successfully wrote to " << argv[2];
        } else std::cout << "Error: Cannot open output file!";
    } else std::cout << "Error: Cannot open input file!";

    return 0;
}