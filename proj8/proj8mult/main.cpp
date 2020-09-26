#include <iostream>
#include <vector>

using namespace std;

#include "header.h"

int main() {

	Matrix my_instance_1(2, 2);
	my_instance_1.set(0, 0, 2);
	my_instance_1.set(0, 1, 4);
	my_instance_1.set(1, 0, 7);
	my_instance_1.set(1, 1, 8);
	my_instance_1.print();

	Matrix my_instance_2(2, 2);
	my_instance_2.set(0, 0, 2);
	my_instance_2.set(0, 1, 4);
	my_instance_2.set(1, 0, 7);
	my_instance_2.set(1, 1, 8);
	my_instance_2.print();

	Matrix product = multiplication(my_instance_1, my_instance_2);
	product.print();
}