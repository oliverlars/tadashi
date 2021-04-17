
fn entry() {

    sieve = [52];
    elements = 52;

    for element = 0; element < elements; element = element + 1 {
        sieve[element] = element;   
    }    
    
    step = 2;
    for step = 2; step < 52; step = step + 1 {
        for check = step+step; check < elements; check = check + step {
           sieve[check] = -1;
        }
   
    }
    
    
    return 0;
}
