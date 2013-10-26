grammar FX;

options {
    output       = AST;
    backtrack    = true;
    memoize      = true;
    k            = 2;
}

tokens {
    T_TRANSLATION_UNIT;
    T_EXTERNAL_DECLARATION;
    T_FUNCTION_DEFINITION;
    T_TECHNIQUE_DEFINITION;
    T_PASS_DEFINITION;
    T_ARGUMENTS_LIST;
    T_ARGUMENT;
    T_SEMANTIC;
    T_SET_SHADER;
}

// R u l e s

translation_unit
    : external_declaration* EOF
        -> ^( T_TRANSLATION_UNIT
                ^( T_EXTERNAL_DECLARATION external_declaration )*
            )
    ;

external_declaration
    : function_definition
    | technique_definition
    ;

function_definition
    : type_specifier T_ID T_LPAREN arguments_list? T_RPAREN semantic? compound_statement
        -> ^( T_FUNCTION_DEFINITION type_specifier T_ID T_LPAREN arguments_list? T_RPAREN semantic? compound_statement )
    ;

arguments_list
    : argument ( T_COMMA argument )*
        -> ^( T_ARGUMENTS_LIST argument ( T_COMMA argument )* )
    ;

argument
    : type_specifier T_ID semantic?
        -> ^( T_ARGUMENT type_specifier T_ID semantic? )
    ;

semantic
    : T_COLON semantic_specifier
        -> ^( T_SEMANTIC semantic_specifier )
    ;

semantic_specifier
    : T_ID
    ;

technique_definition
    : T_TECHNIQUE T_ID T_LCURLY pass* T_RCURLY
        -> ^( T_TECHNIQUE_DEFINITION T_ID pass* )
    ;

pass
    : T_PASS T_ID T_LCURLY ( set_state T_SEMI )* T_RCURLY
        -> ^( T_PASS_DEFINITION T_ID set_state* )
    ;

set_state
    : set_shader
    ;

set_shader
    : set_shader_cmd T_LPAREN compile_shader T_RPAREN
        -> ^( T_SET_SHADER set_shader_cmd compile_shader )
    ;

set_shader_cmd
    : T_SET_VERTEX_SHADER |
      T_SET_PIXEL_SHADER
    ;

compile_shader
    : T_COMPILE_SHADER T_LPAREN shader_profile T_COMMA T_ID T_LPAREN T_RPAREN T_RPAREN
        -> ^( T_COMPILE_SHADER shader_profile T_ID )
    ;

shader_profile
    : T_VS40
    | T_PS40
    ;

type_specifier
    : scalar_type_specifier
    | vector_type_specifier
    | T_FLOAT4X4
    | T_VOID
    | type_id
    ;

scalar_type_specifier
    : T_VOID
    | T_BOOL
    | T_INT
    | T_UINT
    | T_FLOAT
    ;

vector_type_specifier
    : T_INT2
    | T_INT3
    | T_INT4
    | T_UINT2
    | T_UINT3
    | T_UINT4
    | T_FLOAT2
    | T_FLOAT3
    | T_FLOAT4
    ;

type_id
    : T_ID
    ;

// E x p r e s s i o n s

argument_expression_list
    :   assignment_expression ( T_COMMA assignment_expression )*
    ;

additive_expression
    : multiplicative_expression ( T_PLUS multiplicative_expression | T_MINUS multiplicative_expression )*
    ;

multiplicative_expression
    : cast_expression ( T_MUL cast_expression | T_DIV cast_expression | T_MOD cast_expression )*
    ;

cast_expression
    : T_LPAREN type_specifier T_RPAREN cast_expression
    | unary_expression
    ;

unary_expression
    : ( T_PLUS | T_MINUS ) unary_expression
    | postfix_expression
    ;

postfix_expression
    :   primary_expression
        (   T_LBRACKET expression T_RBRACKET
        |   T_LPAREN T_RPAREN
        |   T_LPAREN argument_expression_list T_RPAREN
        |   T_DOT T_ID
        |   T_PLUSPLUS
        |   T_MINUSMINUS
        )*
    ;

primary_expression
    : T_ID
    | constant
    | constructor
    | T_LPAREN expression T_RPAREN
    ;

constant
    : T_HEX_LITERAL
    | T_OCTAL_LITERAL
    | T_DECIMAL_LITERAL
    | T_FLOATING_POINT_LITERAL
    ;

constructor
    : vector_type_specifier T_LPAREN expression ( T_COMMA expression )* T_RPAREN
    ;

/////

expression
    : assignment_expression ( T_COMMA assignment_expression )*
    ;

constant_expression
    : conditional_expression
    ;

assignment_expression
    : lvalue assignment_operator assignment_expression
    | conditional_expression
    ;
    
lvalue
    :   unary_expression
    ;

assignment_operator
    : T_ASSIGN
    | T_MUL_ASSIGN
    | T_DIV_ASSIGN
    | T_MOD_ASSIGN
    | T_ADD_ASSIGN
    | T_SUB_ASSIGN
    | T_BW_SHIFTL_ASSIGN
    | T_BW_SHIFTR_ASSIGN
    | T_BW_AND_ASSIGN
    | T_BW_XOR_ASSIGN
    | T_BW_OR_ASSIGN
    ;

conditional_expression
    : logical_or_expression ( T_QUESTION expression T_COLON conditional_expression )?
    ;

logical_or_expression
    : logical_and_expression ( T_OR logical_and_expression )*
    ;

logical_and_expression
    : inclusive_or_expression ( T_AND inclusive_or_expression )*
    ;

inclusive_or_expression
    : exclusive_or_expression ( T_BW_OR exclusive_or_expression )*
    ;

exclusive_or_expression
    : and_expression ( T_BW_XOR and_expression )*
    ;

and_expression
    : equality_expression ( T_BW_AND equality_expression )*
    ;

equality_expression
    : relational_expression ( ( T_EQUAL | T_NOT_EQUAL ) relational_expression )*
    ;

relational_expression
    : shift_expression ( ( T_LT | T_GT | T_LTE | T_GTE ) shift_expression )*
    ;

shift_expression
    : additive_expression ( ( T_BW_SHIFTL | T_BW_SHIFTR ) additive_expression )*
    ;

// S t a t e m e n t s

statement
    : compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;

compound_statement
    : T_LCURLY ( statement )* T_RCURLY
    ;

expression_statement
    : expression? T_SEMI
    ;

selection_statement
    : T_IF T_LPAREN expression T_RPAREN statement (options {k=1; backtrack=false;}: T_ELSE statement)?
    | T_SWITCH T_LPAREN expression T_RPAREN statement
    ;

iteration_statement
    : T_WHILE T_LPAREN expression T_RPAREN statement
    | T_DO statement T_WHILE T_LPAREN expression T_RPAREN T_SEMI
    | T_FOR T_LPAREN expression_statement expression_statement expression? T_RPAREN statement
    ;

jump_statement
    : T_CONTINUE T_SEMI
    | T_BREAK T_SEMI
    | T_RETURN T_SEMI
    | T_RETURN expression T_SEMI
    ;

// K e y w o r d s

T_BREAK                 : 'break';
T_BUFFER                : 'buffer';
T_CASE                  : 'case';
T_CBUFFER               : 'cbuffer';
T_CONST                 : 'const';
T_CONTINUE              : 'continue';
T_DISCARD               : 'discard';
T_DO                    : 'do';
T_ELSE                  : 'else';
T_FOR                   : 'for';
T_IF                    : 'if';
T_IN                    : 'in';
T_INOUT                 : 'inout';
T_NOINTERPOLATION       : 'nointerpolation';
T_OUT                   : 'out';
T_PASS                  : 'pass';
T_RETURN                : 'return';
T_REGISTER              : 'register';
T_SHARED                : 'shared';
T_STRUCT                : 'struct';
T_SWITCH                : 'switch';
T_TECHNIQUE             : 'technique';
T_TEXTURE2D             : 'texture2d';
T_TYPEDEF               : 'typedef';
T_VOID                  : 'void';
T_WHILE                 : 'while';

T_SET_VERTEX_SHADER     : 'SetVertexShader';
T_SET_PIXEL_SHADER      : 'SetPixelShader';
T_COMPILE_SHADER        : 'CompileShader';

T_VS40                  : 'vs_4_0';
T_PS40                  : 'ps_4_0';

T_BOOL                  : 'bool';
T_INT                   : 'int';
T_INT2                  : 'int2';
T_INT3                  : 'int3';
T_INT4                  : 'int4';
T_UINT                  : 'uint';
T_UINT2                 : 'uint2';
T_UINT3                 : 'uint3';
T_UINT4                 : 'uint4';
T_HALF                  : 'half';
T_HALF2                 : 'half2';
T_HALF3                 : 'half3';
T_HALF4                 : 'half4';
T_FLOAT                 : 'float';
T_FLOAT2                : 'float2';
T_FLOAT3                : 'float3';
T_FLOAT4                : 'float4';
T_FLOAT4X4              : 'float4x4';

// B a s e   t o k e n s

T_ID
    :   LETTER ( LETTER | '0'..'9' )*
    ;

fragment
LETTER
    :   '$'
    |   'A'..'Z'
    |   'a'..'z'
    |   '_'
    ;

T_HEX_LITERAL
    : '0' ('x'|'X') HexDigit+ IntegerTypeSuffix? ;

T_DECIMAL_LITERAL
    : ('0' | '1'..'9' '0'..'9'*) IntegerTypeSuffix? ;

T_OCTAL_LITERAL
    : '0' ('0'..'7')+ IntegerTypeSuffix? ;

fragment
HexDigit
    : ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
IntegerTypeSuffix
    :   ('u'|'U') ('l'|'L')?
    ;

T_FLOATING_POINT_LITERAL
    :   ('0'..'9')+ '.' ('0'..'9')* Exponent? FloatTypeSuffix?
    |   '.' ('0'..'9')+ Exponent? FloatTypeSuffix?
    ;

fragment
Exponent
    : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
FloatTypeSuffix
    : ('f'|'F'|'d'|'D') ;

fragment
EscapeSequence
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    |   OctalEscape
    ;

fragment
OctalEscape
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

fragment
UnicodeEscape
    :   '\\' 'u' HexDigit HexDigit HexDigit HexDigit
    ;

T_NOT               : '!'   ;
T_NOT_EQUAL         : '!='  ;
T_AND               : '&&'  ;
T_MUL               : '*'   ;
T_MUL_ASSIGN        : '*='  ;
T_PLUS              : '+'   ;
T_PLUSPLUS          : '++'  ;
T_ADD_ASSIGN        : '+='  ;
T_COMMA             : ','   ;
T_MINUS             : '-'   ;
T_MINUSMINUS        : '--'  ;
T_SUB_ASSIGN        : '-='  ;
T_DIV               : '/'   ;
T_DIV_ASSIGN        : '/='  ;
T_MOD               : '%'   ;
T_MOD_ASSIGN        : '%='  ;
T_COLON             : ':'   ;
T_SEMI              : ';'   ;
T_LT                : '<'   ;
T_LTE               : '<='  ;
T_ASSIGN            : '='   ;
T_EQUAL             : '=='  ;
T_GT                : '>'   ;
T_GTE               : '>='  ;
T_QUESTION          : '?'   ;
T_LPAREN            : '('   ;
T_RPAREN            : ')'   ;
T_LBRACKET          : '['   ;
T_RBRACKET          : ']'   ;
T_LCURLY            : '{'   ;
T_RCURLY            : '}'   ;
T_OR                : '||'  ;
T_DOT               : '.'   ;
T_BW_NOT            : '~'   ;
T_BW_SHIFTL         : '<<'  ;
T_BW_SHIFTR         : '>>'  ;
T_BW_AND            : '&'   ;
T_BW_OR             : '|'   ;
T_BW_XOR            : '^'   ;
T_BW_SHIFTL_ASSIGN  : '<<=' ;
T_BW_SHIFTR_ASSIGN  : '>>=' ;
T_BW_AND_ASSIGN     : '&='  ;
T_BW_OR_ASSIGN      : '|='  ;
T_BW_XOR_ASSIGN     : '^='  ;

WS  :  (' '|'\r'|'\t'|'\u000C'|'\n') {$channel=HIDDEN;}
    ;

COMMENT
    :   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

LINE_COMMENT
    : '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    ;

// ignore #line info for now
LINE_COMMAND 
    : '#' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    ;
