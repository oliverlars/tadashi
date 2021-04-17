
fn xorshift(rand_state) {

    sstate = rand_state ^ (rand_state << 13);
    sstate = sstate ^ (sstate >> 17);
    sstate = sstate ^ (sstate << 5);
    
    return sstate;

}

fn mod(x, y){
  div_temp =0;
  div_temp = (x / y);
  return (x - (y * div_temp));
}


fn mod_bit(random, bits) {
  return (random & bits);
}


fn entry() {
    sstate = 12346;
    pack = [52];
    card_one = 0;
    card_two = 0;
    temp = 0;
    rand_modb = 0;
    random_card = 0;
    
    for i = 0; i < 52; i = i + 1 {
        pack[i] = i;
    }
     
    
    for i = 0; i < 52; i = i + 1 {

    sstate = xorshift(sstate);
    rand_modb = mod_bit(sstate, 127);
    card_one = mod(rand_modb, 52);
        
    sstate = xorshift(sstate);
    rand_modb = mod_bit(sstate, 127);
    card_two = mod(rand_modb, 52);

    temp = pack[card_one];
    pack[card_one] = pack[card_two];
    pack[card_two] = temp;
   
    }    
    return 0;
}
