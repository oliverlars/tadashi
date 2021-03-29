fn function_one() {
	x = 20;
	return x + 1;
}

fn function_two() {
	return 1 + 2 + 3;
}

fn entry() {
	array = [10];
	array[0] = function_one();
	array[1] = 67;
	array[2] = 49;
	array[3] = 1;
	array[4] = 512;
	array[5] = 102;
	array[6] = 1256;
	array[7] = 991;
	array[8] = 2;
	array[9] = function_two();
	for i = 0; i < 10; i = i +1 {
		for j = i + 1; j < 10; j = j + 1 {
			if array[i] > array[j] {
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}

//add support for comments (like this one)
// and also strings (syntactic sugar around arrays)
// add unary - and +
//parse all functions (currently is hard coded to the above 3)
// while loops
// add divides!!!!!
// output debug info like where each block starts/ends (functions, ifs, fors, etc)
// maybe change for loops to c like (for init; cond; stmt)
// bug: for loops seem to run indefinitely when using variables
// bug: make else statements optional

/*

*/