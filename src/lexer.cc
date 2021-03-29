
internal void
advance_lexer(Lexer* l){
    l->at++;
}

internal void
gobble_whitespace(Lexer* l){
    while(is_whitespace(*l->at)){
        advance_lexer(l);
    }
}

internal Token
peek_token(Lexer* l);

internal Token
get_token(Lexer* l){
    
    gobble_whitespace(l);
    char c = *l->at;
    Token token = {};
    token.at = l->at;
    token.length = 1;
    advance_lexer(l);
    
    switch(c){
        case 0:{
            token.type = TOKEN_EOF;
        }break;
        case '!': {
            if(peek_token(l).type == TOKEN_EQUALS){
                get_token(l);
                token.type = TOKEN_BANG_EQUALS;
            }else {
                token.type = TOKEN_BANG;
            }
        }break;
        case '<': {
            if(peek_token(l).type == TOKEN_EQUALS){
                get_token(l);
                token.type = TOKEN_LEFT_ANGLE_EQUAL;
            }else {
                token.type = TOKEN_LEFT_ANGLE;
            }
        }break;
        case '>': {
            if(peek_token(l).type == TOKEN_EQUALS){
                get_token(l);
                token.type = TOKEN_RIGHT_ANGLE_EQUAL;
            }else {
                token.type = TOKEN_LEFT_ANGLE;
            }
        }break;
        case '=':{
            if(peek_token(l).type == TOKEN_EQUALS){
                get_token(l);
                token.type = TOKEN_EQUALS_EQUALS;
            }else {
                token.type = TOKEN_EQUALS;
            }
        }break;
        case ',':{
            token.type = TOKEN_COMMA;
        }break;
        case ';':{
            token.type = TOKEN_SEMICOLON;
        }break;
        case '}':{
            token.type = TOKEN_RIGHT_BRACE;
        }break;
        case '{':{
            token.type = TOKEN_LEFT_BRACE;
        }break;
        case '(':{
            token.type = TOKEN_LEFT_PAREN;
        }break;
        case ')':{
            token.type = TOKEN_RIGHT_PAREN;
        }break;
        case ']':{
            token.type = TOKEN_RIGHT_BRACKET;
        }break;
        case '[':{
            token.type = TOKEN_LEFT_BRACKET;
        }break;
        case '+':{
            token.type = TOKEN_PLUS;
        }break;
        case '/':{
            token.type = TOKEN_FORWARD_SLASH;
        }break;
        case '*':{
            token.type = TOKEN_ASTERISK;
        }break;
        case '-':{
            token.type = TOKEN_MINUS;
        }break;
        default: {
            if(is_alpha(c) || c == '_'){
                while(is_alpha(*l->at) || *l->at == '_'){
                    advance_lexer(l);
                }
                token.length = l->at  - token.at;
                token.type = TOKEN_IDENTIFIER;
            }else if(is_digit(c)){
                while(is_digit(*l->at)){
                    advance_lexer(l);
                }
                token.length = l->at  - token.at;
                token.type = TOKEN_NUMBER;
            }
        }break;
    }
    
    return token;
}

internal Token
peek_token(Lexer* l){
    Lexer l2 = *l;
    auto token = get_token(&l2);
    return token;
}

internal Token
expect_token(Lexer* l, Token_Type type){
    Token peek = peek_token(l);
    if(peek.type != type){
        printf("expected token %s but got %.*s", token_types[type], peek.length, peek.at);
        exit(0);
    }
    return get_token(l);
}

internal Token
expect_keyword(Lexer* l, char* str){
    Token peek = peek_token(l);
    if(!token_equals_string(peek, str)){
        printf("expected keyword %s but got %.*s", str, peek.length, peek.at);
        exit(0);
    }
    return get_token(l);
}
