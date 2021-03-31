fn dot(a, b){
	result = 0;
	for i = 0; i < 3; i = i + 1 {
		result = result + a[i]*b[i];
	}
	
	return result;
}

fn entry() {
	x = [3];
	x[0] = 1; /* potato 
	is cool and all
	*/
	x[1] = 2;
	x[2] = 3;
	
	y = [3];
	y[0] = 1;
	y[1] = 2;
	y[2] = 3;
	xyz = "afdss";
	xyz[0] = 'A';
	xyz[2] = 'b';
	product_result = dot(x, y);
}






//add support for comments (like this one)
// add unary - and +
// while loops
// add divides!!!!!
// output debug info like where each block starts/ends (functions, ifs, fors, etc)
// equals
// and fix bugs
// try out making a strlen function (make strings c like)

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