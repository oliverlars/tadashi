fn dot(a, b) {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

fn entry() {
	x = [3];
	y = [3];
	x[0] = 0;
	x[1] = 0;
	x[2] = 0;
	
	y[0] = 0;
	y[1] = 0;
	y[2] = 0;
	z = 1*1 + 2*2 + 3*3;
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
// try out making a strlen function
// separate declarations from assignments (to catch more errors)

// a = [10]; make array of 10 numbers
// a == address of array
// a[0] first element of array
// pass by value to functions
// take a address and add offset
// dereference a + offset to read/write
/*
	string = "zdsfjlasfjlk;";
	for x = 0; x < 5; x = x + 1 {
		random = random ^ (random << 13);
		random = random ^ (random >> 17);
		random = random ^ (random << 5);
		array[x] = random;
	}
	
	for i = 0; i < 13; i = i +1 {
		for j = i + 1; j < 13; j = j + 1 {
			if string[i] > string[j] {
				temp = string[i];
				string[i] = string[j];
				string[j] = temp;
			}
		}
	}
*/