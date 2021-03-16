

struct Lexer {
    char* at;
};

enum Token_Type {
    TOKEN_INVALID,
    TOKEN_EOF,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_EQUALS,
    TOKEN_RIGHT_BRACKET,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_PAREN,
    TOKEN_FORWARD_SLASH,
    TOKEN_ASTERISK,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
};

struct Token {
    char* at;
    int length;
    Token_Type type;
};

internal bool
tokens_equal(Token a, Token b){
    if(a.length != b.length) return false;
    for(int i = 0; i < a.length; i++){
        if(a.at[i] != b.at[i]) return false;
    }
    return true;
}

#define is_newline(x) ((x) == '\n' || (x) == '\r')
#define is_whitespace(x) ((x) == ' ' || (x) == '\t' || (x) == '\v' || (x) == '\f' || is_newline(x))
#define is_digit(x) ((x) <= '9' && (x) >= '0')
#define is_upper_hex(x) (is_digit(x) || ((x) <= 'F' && (x) >= 'A'))
#define is_lower_hex(x) (is_digit(x) || ((x) <= 'f' && (x) >= 'a'))
#define is_hex(x) (is_upper(x)  || is_lower_hex(x))
#define is_lower_alpha(x) ((x) <= 'z' && (x) >= 'a')
#define is_upper_alpha(x) ((x) <= 'Z' && (x) >= 'A')
#define is_alpha(x) ((is_lower_alpha(x) || is_upper_alpha(x)))