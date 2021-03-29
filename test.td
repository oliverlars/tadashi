fn function_one() {
	x = 20;
	return x + 1;
}

fn function_two() {
	return 1 + 2 + 3;
}

fn entry() {
	array = [5];
	string = "test";
	for i = 0; i < 4; i = i + 1 {
		string[i] = string[i] + 5;
	}
	for x = 0; x < 5; x = x + 1 {
		random = random ^ (random << 13);
		random = random ^ (random >> 17);
		random = random ^ (random << 5);
		array[x] = random;
	}
	
	for i = 0; i < 5; i = i +1 {
		for j = i + 1; j < 5; j = j + 1 {
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
// equals
// parameters
// and fix bugs
// add strlen intrinsic
/*

*/