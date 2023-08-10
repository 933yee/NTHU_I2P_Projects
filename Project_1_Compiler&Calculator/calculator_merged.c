#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// for lex
#define MAXLEN 256

// Token types
typedef enum {
    UNKNOWN, END, ENDFILE, 
    INT, ID,
    ADDSUB, MULDIV,
    ASSIGN, 
    LPAREN, RPAREN,
    INCDEC, 
    AND, OR, XOR
} TokenSet;

TokenSet getToken(void);
TokenSet curToken = UNKNOWN;
char lexeme[MAXLEN];

// Test if a token matches the current token
int match(TokenSet token);
// Get the next token
void advance(void);
// Get the lexeme of the current token
char *getLexeme(void);

// for parser
#define TBLSIZE 64
// Set PRINTERR to 1 to print error message while calling error()
// Make sure you set PRINTERR to 0 before you submit your code
#define PRINTERR 1

// Call this macro to print error message and exit the program
// This will also print where you called it in your program
#define error(errorNum) { \
    if (PRINTERR) \
        fprintf(stderr, "error() called at %s:%d: ", __FILE__, __LINE__); \
    err(errorNum); \
}

// Error types
typedef enum {
    UNDEFINED, MISPAREN, NOTNUMID, NOTFOUND,
    RUNOUT, NOTLVAL, DIVZERO, SYNTAXERR, INVNAME,REGOUT
} ErrorType;

// Structure of the symbol table
typedef struct {
    int val;
    char name[MAXLEN];
} Symbol;
// Structure of a tree node
typedef struct _Node {
    TokenSet data;
    int val;
    char lexeme[MAXLEN];
    struct _Node *left;
    struct _Node *right;
} BTNode;
int sbcount = 0;
int rCurrent;
int divideValid = 0;
int leftnum = 0, rightnum = 0;
Symbol table[TBLSIZE];
Symbol Reg[8];
// Initialize the symbol table with builtin variables
void initTable(void);
// Get the value of a variable
int getval(char *str, BTNode*);
// Set the value of a variable
int setval(char *str, int val);
// Make a new node according to token type and lexeme
BTNode *makeNode(TokenSet tok, const char *lexe);
// Free the syntax tree
void freeTree(BTNode *root);
BTNode *factor(void);
BTNode *term(void);
BTNode *term_tail(BTNode *left);
BTNode *assign_expr(void);
BTNode *expr_tail(BTNode *left);
// create
BTNode *or_expr();
BTNode *or_expr_tail(BTNode *left);
BTNode *xor_expr();
BTNode *xor_expr_tail(BTNode *left);
BTNode *and_expr();
BTNode *and_expr_tail(BTNode *left);
BTNode *addsub_expr();
BTNode *addsub_expr_tail(BTNode *left);
BTNode *muldiv_expr();
BTNode *muldiv_expr_tail(BTNode *left);
BTNode *unary_expr();

void statement(void);
// Print error message and exit the program
void err(ErrorType errorNum);


// for codeGen
// Evaluate the syntax tree
int evaluateTree(BTNode *root);
// Print the syntax tree in prefix
void printPrefix(BTNode *root);
void TPrefix(BTNode *root);
void NumPrefixCal(BTNode *root, char key);


/*============================================================================================
lex implementation
============================================================================================*/
TokenSet getToken(void)
{
    int i = 0;
    char c = '\0';

    while ((c = fgetc(stdin)) == ' ' || c == '\t');
    if (isdigit(c)) {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isdigit(c) && i < MAXLEN) {
            lexeme[i] = c;
            ++i;
            c = fgetc(stdin);
        }
        if (isalpha(c) || c == '_') {
            error(INVNAME);
        }
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return INT;
    } else if (c == '+') {
        lexeme[0] = c;
        c = fgetc(stdin);
        if(c == '+'){
            lexeme[1] = c;
            lexeme[2] = '\0';
            return INCDEC;
        }
        ungetc(c, stdin);
        lexeme[1]='\0';
        return ADDSUB;
    } else if (c == '-') {
        lexeme[0] = c;
        c = fgetc(stdin);
        if(c == '-'){
            lexeme[1] = c;
            lexeme[2] = '\0';
            return INCDEC;
        }
        ungetc(c, stdin);
        lexeme[1]='\0';
        return ADDSUB;
    }else if (c == '*' || c == '/') {
        lexeme[0] = c;
        lexeme[1] = '\0';
        return MULDIV;
    } else if (c == '\n') {
        lexeme[0] = '\0';
        return END;
    } else if (c == '=') {
        strcpy(lexeme, "=");
        return ASSIGN;
    } else if (c == '(') {
        strcpy(lexeme, "(");
        return LPAREN;
    } else if (c == ')') {
        strcpy(lexeme, ")");
        return RPAREN;
    } else if (isalpha(c) || c=='_') {
        lexeme[0] = c;
        c = fgetc(stdin);
        i = 1;
        while (isalpha(c) || isdigit(c) || c == '_') {
            lexeme[i++] = c;
            c = fgetc(stdin);
        }
        char valid[12]=" &|^+-*/)=\n";
        int flag=1;
        for(int j=0; j<11; j++){
            if(c==valid[j]){
                flag = 0;
                break;
            }
        }
        if(flag)err(INVNAME);
        ungetc(c, stdin);
        lexeme[i] = '\0';
        return ID;
    } else if (c == EOF) {
        return ENDFILE;
    } else if (c == '|'){
        strcpy(lexeme, "|");
        return OR;
    } else if (c == '&'){
        strcpy(lexeme, "&");
        return AND;
    } else if (c == '^'){
        strcpy(lexeme, "^");
        return XOR;
    }else {
        err(SYNTAXERR);
        return UNKNOWN;
    }
}

void advance(void) {
    curToken = getToken();
}

int match(TokenSet token) {
    if (curToken == UNKNOWN)
        advance();
    return token == curToken;
}

char *getLexeme(void) {
    return lexeme;
}


/*============================================================================================
parser implementation
============================================================================================*/

void initTable(void) {
    strcpy(table[0].name, "x");
    table[0].val = 0;
    strcpy(table[1].name, "y");
    table[1].val = 0;
    strcpy(table[2].name, "z");
    table[2].val = 0;
    sbcount = 3;
}

int getval(char *str, BTNode* root) {
    int i = 0;

    for (i = 0; i < sbcount; i++)
        if (strcmp(str, table[i].name) == 0){
            return table[i].val;
        }
    //if (sbcount >= TBLSIZE)
    //    error(RUNOUT);
    error(NOTFOUND);
    /*printf("this var doesn't exist");
    strcpy(table[sbcount].name, str);
    table[sbcount].val = 0;
    sbcount++;*/
    return 0;
}

int setval(char *str, int val) {
    int i = 0;

    for (i = 0; i < sbcount; i++) {
        if (strcmp(str, table[i].name) == 0) {
            table[i].val = val;
            return val;
        }
    }

    if (sbcount >= TBLSIZE)
        error(RUNOUT);

    strcpy(table[sbcount].name, str);
    table[sbcount].val = val;
    sbcount++;
    return val;
}

BTNode *makeNode(TokenSet tok, const char *lexe) {
    BTNode *node = (BTNode*)malloc(sizeof(BTNode));
    strcpy(node->lexeme, lexe);
    node->val=0;
    node->data = tok;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeTree(BTNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// factor := INT | ADDSUB INT |
//		   	 ID  | ADDSUB ID  |
//		   	 ID ASSIGN expr |
//		   	 LPAREN expr RPAREN |
//		   	 ADDSUB LPAREN expr RPAREN
BTNode *factor(void) {
    BTNode *retp = NULL, *left = NULL;

    if (match(INT)) {
        retp = makeNode(INT, getLexeme());
        advance();
    } else if (match(ID)) {
        retp = makeNode(ID, getLexeme());
        advance();
    } else if (match(LPAREN)) {
        advance();
        retp = assign_expr();
        if (match(RPAREN))
            advance();
        else
            error(MISPAREN);
    }else if (match(INCDEC)) {
        retp = makeNode(ASSIGN, "="); 
        char tmp[10];
        strcpy(tmp, getLexeme());
        advance();
        if(match(ID)){
            retp -> left = makeNode(ID, getLexeme());
            if(!strcmp(tmp, "++"))retp -> right = makeNode(ADDSUB, "+");
            else retp -> right = makeNode(ADDSUB, "-");
            retp -> right -> left = makeNode(ID, getLexeme());
            retp -> right -> right = makeNode(INT, "1");
            advance();
        }else{
            err(SYNTAXERR);
        }
    } else {
        error(NOTNUMID);
    }
    return retp;
}


BTNode *or_expr(void){
    BTNode *node = xor_expr();    
    return or_expr_tail(node);
}
BTNode *or_expr_tail(BTNode *left) {
    BTNode *node = NULL;
    if (match(OR)) {
        node = makeNode(OR, getLexeme());
        advance();
        node->left = left;
        node->right = xor_expr();
        return or_expr_tail(node);
    } else {
        return left;
    }
}
BTNode *xor_expr(void){
    BTNode *node = and_expr();    
    return xor_expr_tail(node);
}
BTNode *xor_expr_tail(BTNode *left) {
    BTNode *node = NULL;
    if (match(XOR)) {
        node = makeNode(XOR, getLexeme());
        advance();
        node->left = left;
        node->right = and_expr();
        return xor_expr_tail(node);
    } else {
        return left;
    }
}
BTNode *and_expr(void){
    BTNode *node = addsub_expr();    
    return and_expr_tail(node);
}
BTNode *and_expr_tail(BTNode *left) {
    BTNode *node = NULL;
    if (match(AND)) {
        node = makeNode(AND, getLexeme());
        advance();
        node->left = left;
        node->right = addsub_expr();
        return and_expr_tail(node);
    } else {
        return left;
    }
}
BTNode *addsub_expr(void){
    BTNode *node = muldiv_expr();    
    return addsub_expr_tail(node);
}
BTNode *addsub_expr_tail(BTNode *left) {
    BTNode *node = NULL;
    if (match(ADDSUB)) {
        node = makeNode(ADDSUB, getLexeme());
        advance();
        node->left = left;
        node->right = muldiv_expr();
        return addsub_expr_tail(node);
    } else {
        return left;
    }
}
BTNode *muldiv_expr(void){
    BTNode *node = unary_expr();    
    return muldiv_expr_tail(node);
}
BTNode *muldiv_expr_tail(BTNode *left) {
    BTNode *node = NULL;
    if (match(MULDIV)) {
        node = makeNode(MULDIV, getLexeme());
        advance();
        node->left = left;
        node->right = unary_expr();
        return muldiv_expr_tail(node);
    } else {
        return left;
    }
}
BTNode *unary_expr(void){
    BTNode* node = NULL;
    if(match(ADDSUB)){
        if(strcmp(getLexeme(), "+")==0)node = makeNode(ADDSUB, "+");
        else node = makeNode(ADDSUB, "-");     
        node->left = makeNode(INT, "0");
        advance();
        node->right = unary_expr();
        return node;         
    }else{
        return factor();
    }
}
// expr := term expr_tail
BTNode *assign_expr(void) {
    BTNode* node = NULL;
    BTNode* left = NULL;
    if(match(ID)){
        char rec[1000]={0}, rec2[1000]={0};
        strcpy(rec, getLexeme());
        advance();
        strcpy(rec2, getLexeme());
        if(match(ASSIGN)){
            left = makeNode(ID, rec);
            node = makeNode(ASSIGN, getLexeme());
            node->left = left;
            advance();
            node->right= assign_expr();
            return node;
        }else if(match(END)){
            left = makeNode(ID, rec);
            return left;
        }else{
            strcpy(lexeme,rec);
            // for(int i=strlen(rec2)-1; i>=0; i--) ungetc(rec2[i], stdin);
            for(int i=0; i<strlen(rec2); i++) ungetc(rec2[i], stdin);
            curToken=ID;
            return or_expr();
        }
    }
    return or_expr();
}

// expr_tail := ADDSUB term expr_tail | NiL


// statement := ENDFILE | END | expr END
void statement(void) {
    BTNode *retp = NULL;

    if (match(ENDFILE)) {
        printf("MOV r0 [0]\n");
        printf("MOV r1 [4]\n");
        printf("MOV r2 [8]\n");
        printf("EXIT 0\n");
        //printf("x: %d, y: %d, z: %d\n",table[0].val, table[1].val, table[2].val);
        exit(0);
    } else if (match(END)) {
        //printf(">> ");
        advance();
    } else {
        retp = assign_expr();
        if (match(END)) {
            rCurrent=0;
            int x =  evaluateTree(retp);
            //printf("%d\n",x);
            //printf("Prefix traversal: ");
            //printPrefix(retp);
            //printf("\n");
            freeTree(retp);
            //printf(">> ");
            advance();
        } else {
            error(SYNTAXERR);
        }
    }
}

void err(ErrorType errorNum) {
    if (PRINTERR) {
        fprintf(stderr, "error: ");
        switch (errorNum) {
            case MISPAREN:
                fprintf(stderr, "mismatched parenthesis\n");
                break;
            case NOTNUMID:
                fprintf(stderr, "number or identifier expected\n");
                break;
            case NOTFOUND:
                fprintf(stderr, "variable not defined\n");
                break;
            case RUNOUT:
                fprintf(stderr, "out of memory\n");
                break;
            case NOTLVAL:
                fprintf(stderr, "lvalue required as an operand\n");
                break;
            case DIVZERO:
                fprintf(stderr, "divide by constant zero\n");
                break;
            case SYNTAXERR:
                fprintf(stderr, "syntax error\n");
                break;
            case INVNAME:
                fprintf(stderr, "invalid variable name\n");
                break;
            case REGOUT:
                fprintf(stderr, "too many register\n");
                break;
            default:
                fprintf(stderr, "undefined error\n");
                break;
        }
    }
    printf("EXIT 1\n");
    exit(0);
}


/*============================================================================================
codeGen implementation
============================================================================================*/
int evaluateTree(BTNode *root) {
    int retval = 0, lv = 0, rv = 0;
    if (root != NULL) {
        int key=0;
        switch (root->data) {
            case ID:
                retval = getval(root->lexeme, root);
                if(rCurrent>=8) err(REGOUT);
                Reg[rCurrent].val=retval;
                int k=0, debug=1;
                for (int i = 0; i < sbcount; i++, k+=4)
                    if (strcmp(root->lexeme, table[i].name) == 0){
                        printf("MOV r%d [%d]\n", rCurrent, k);         
                        strcpy(Reg[rCurrent].name,root->lexeme);
                        debug=0;
                        break;
                    }
                if(debug){
                    err(SYNTAXERR);
                }
                rCurrent++;
                break;
            case INT:
                retval = atoi(root->lexeme);
                if(rCurrent>=8) err(REGOUT);
                Reg[rCurrent].val=retval;
                strcpy(Reg[rCurrent].name,root->lexeme);
                printf("MOV r%d %d\n", rCurrent, retval);
                rCurrent++;
                break;
            case ASSIGN:
                rv = evaluateTree(root->right);
                retval = setval(root->left->lexeme, rv);
                int k1=0;
                for (int i = 0; i < sbcount; i++, k1+=4)
                    if (strcmp(root->left->lexeme, table[i].name) == 0){
                        table[i].val = rv;
                        break;
                    }
                printf("MOV [%d] r%d\n", k1, rCurrent-1);
                break;
            case ADDSUB:
            case OR:
            case XOR:
            case AND:
            case MULDIV:
                
                leftnum = 0;
                rightnum = 0;
                NumPrefixCal(root->left, 'L');
                NumPrefixCal(root->right, 'R');
                if(leftnum>=rightnum){
                    lv = evaluateTree(root->left);
                    rv = evaluateTree(root->right);        
                }else{
                    key=1;
                    rv = evaluateTree(root->right);
                    lv = evaluateTree(root->left);            
                }
                if (strcmp(root->lexeme, "+") == 0) {
                    retval = lv + rv;
                    Reg[rCurrent-2].val=retval;
                    printf("ADD r%d r%d\n", rCurrent-2, rCurrent-1);
                    rCurrent--;
                } else if (strcmp(root->lexeme, "-") == 0) {
                    retval = lv - rv;
                    Reg[rCurrent-2].val=retval;
                    if(key){
                        printf("SUB r%d r%d\n", rCurrent-1, rCurrent-2);
                        printf("MOV r%d r%d\n", rCurrent-2, rCurrent-1);
                    }else printf("SUB r%d r%d\n", rCurrent-2, rCurrent-1);
                    rCurrent--;
                } else if (strcmp(root->lexeme, "*") == 0) {
                    retval = lv * rv;   
                    Reg[rCurrent-2].val=retval;
                    printf("MUL r%d r%d\n", rCurrent-2, rCurrent-1);
                    rCurrent--;
                } else if (strcmp(root->lexeme, "/") == 0) {
                    if (rv == 0){
                        divideValid = 0;
                        TPrefix(root->right);
                        if(divideValid){
                            if(key){
                                printf("DIV r%d r%d\n", rCurrent-1, rCurrent-2);
                                printf("MOV r%d r%d\n", rCurrent-2, rCurrent-1);
                            }else printf("DIV r%d r%d\n", rCurrent-2, rCurrent-1);
                            return 1;
                        }
                        error(DIVZERO);
                    }
                    retval = lv / rv;
                    Reg[rCurrent-2].val=retval;
                    if(key){
                        printf("DIV r%d r%d\n", rCurrent-1, rCurrent-2);
                        printf("MOV r%d r%d\n", rCurrent-2, rCurrent-1);
                    }else printf("DIV r%d r%d\n", rCurrent-2, rCurrent-1);
                    rCurrent--;
                } else if (strcmp(root->lexeme, "|") == 0) {
                    retval = lv | rv;
                    Reg[rCurrent-2].val=retval;
                    printf("OR r%d r%d\n", rCurrent-2, rCurrent-1);
                    rCurrent--;
                } else if (strcmp(root->lexeme, "^") == 0) {
                    retval = lv ^ rv;
                    Reg[rCurrent-2].val=retval;
                    printf("XOR r%d r%d\n", rCurrent-2, rCurrent-1);
                    rCurrent--;
                } else if (strcmp(root->lexeme, "&") == 0) {
                    retval = lv & rv;
                    Reg[rCurrent-2].val=retval;
                    printf("AND r%d r%d\n", rCurrent-2, rCurrent-1);
                    rCurrent--;
                } 
                break;
            default:
                retval = 0;
        }
    }
    return retval;
}

void printPrefix(BTNode *root) {
    if (root != NULL) {
        printf("%s ", root->lexeme);
        printPrefix(root->left);
        printPrefix(root->right);
    }
}
void TPrefix(BTNode *root) {
    if (root != NULL) {
        for (int i = 0; i < sbcount; i++)
            if (strcmp(table[i].name, root->lexeme) == 0){
                divideValid = 1;
                return;
            }
        TPrefix(root->left);
        TPrefix(root->right);
    }
}
void NumPrefixCal(BTNode *root, char key) {
    if(root->left==NULL && root->right==NULL){
        if(key=='L')leftnum++;
        else rightnum++;
        return;
    }
    if(strcmp(root->lexeme, "=")==0){
        if(key=='L')leftnum--;
        else rightnum--;
    }
    NumPrefixCal(root->left, key);
    NumPrefixCal(root->right, key);
}

/*============================================================================================
main
============================================================================================*/

// This package is a calculator
// It works like a Python interpretor
// Example:
// >> y = 2
// >> z = 2
// >> x = 3 * y + 4 / (2 * z)
// It will print the answer of every line
// You should turn it into an expression compiler
// And print the assembly code according to the input

// This is the grammar used in this package
// You can modify it according to the spec and the slide
// statement  :=  ENDFILE | END | expr END
// expr    	  :=  term expr_tail
// expr_tail  :=  ADDSUB term expr_tail | NiL
// term 	  :=  factor term_tail
// term_tail  :=  MULDIV factor term_tail| NiL
// factor	  :=  INT | ADDSUB INT |
//		   	      ID  | ADDSUB ID  |
//		   	      ID ASSIGN expr |
//		   	      LPAREN expr RPAREN |
//		   	      ADDSUB LPAREN expr RPAREN

int main() {
    freopen("input.txt", "w", stdout);
    initTable();
    //printf(">> ");
    while (1) {
        statement();
    }
    return 0;
}
