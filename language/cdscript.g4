grammar cdscript;

options {
	language = Cpp;
}

chunk: block EOF;

block: compound_statement | statement_sequence;

compound_statement: '{' statement_sequence? '}';

statement_sequence: statement+;

statement:
	';'
	| expression ';'
	| declare_variable ';'
	| assignment ';'
	| Return expression ';'
	| compound_statement;

declare_variable: var_modifier var_type (',' var_type)*;

var_modifier: Var | Global;

const_modifier: Const | Static;

var_type: Identifier (':' type_identifier)?;

type_identifier:
	Identifier
	| ArrarIdentifier
	| templateIdentifier;

literal: Null | booleanliteral | numberliteral | Stringliteral;

Stringliteral: '"' Schar* '"';

fragment Schar:
	~ ["\\\r\n]
	| Escapesequence
	| Universalcharactername;

fragment Rawstring: '"' .*? '(' .*? ')' .*? '"';

fragment Escapesequence:
	Simpleescapesequence
	| Octalescapesequence
	| Hexadecimalescapesequence;

fragment Simpleescapesequence:
	'\\\''
	| '\\"'
	| '\\?'
	| '\\\\'
	| '\\a'
	| '\\b'
	| '\\f'
	| '\\n'
	| '\\r'
	| '\\t'
	| '\\v';

fragment Octalescapesequence:
	'\\' OCTAL_DIGIT
	| '\\' OCTAL_DIGIT OCTAL_DIGIT
	| '\\' OCTAL_DIGIT OCTAL_DIGIT OCTAL_DIGIT;

fragment Hexadecimalescapesequence: '\\x' HEXADECIMAL_DIGIT+;
fragment Hexquad:
	HEXADECIMAL_DIGIT HEXADECIMAL_DIGIT HEXADECIMAL_DIGIT HEXADECIMAL_DIGIT;

fragment Universalcharactername:
	'\\u' Hexquad
	| '\\U' Hexquad Hexquad;

booleanliteral: True | False;
numberliteral: IntegerLiteral | FloatingLiteral;

assignment:
	Identifier '=' expression
	| declare_variable '=' expression
	| const_modifier var_type '=' expression;

expression:
	primary_expression
	| unary_expression
	| expression multiplicative_operator expression
	| expression additive_operator expression
	| expression compare_operator expression;

primary_expression:
	literal
	| This
	| Super
	| Identifier
	| '(' expression ')';

unary_expression: Not expression | '#' expression;

multiplicative_operator: '*' | '/' | '%';
additive_operator: | '+' | '-';
compare_operator: '==' | '<' | '>' | '<=' | '>=' | '!=';

IntegerLiteral:
	DecimalLiteral
	| OctalLiteral
	| HexadecimalLiteral
	| BinaryLiteral;

DecimalLiteral: NONZERO_DIGIT ('\''? DIGIT)*;

OctalLiteral: '0' ('\''? OCTAL_DIGIT)*;

HexadecimalLiteral: ('0x' | '0X') HEXADECIMAL_DIGIT (
		'\''? HEXADECIMAL_DIGIT
	)*;

BinaryLiteral: ('0b' | '0B') BINARY_DIGIT ( '\''? BINARY_DIGIT)*;

FloatingLiteral:
	DecimalFloatingLiteral
	| HexadecimalFloatingLiteral;

DecimalFloatingLiteral:
	DIGIT+ '.' DIGIT* EXPONENT_PART?
	| '.'? DIGIT+ EXPONENT_PART?
	| DIGIT+ EXPONENT_PART;

HexadecimalFloatingLiteral:
	'0' [xX] HEXADECIMAL_DIGIT+ '.' HEXADECIMAL_DIGIT* HEX_EXPONENT_PART?
	| '0' [xX] '.' HEXADECIMAL_DIGIT+ HEX_EXPONENT_PART?
	| '0' [xX] HEXADECIMAL_DIGIT+ HEX_EXPONENT_PART;

fragment EXPONENT_PART: [eE] [+-]? DIGIT+;

fragment HEX_EXPONENT_PART: [pP] [+-]? DIGIT+;

fragment Digitsequence: DIGIT ('\''? DIGIT)*;

fragment NON_DIGIT: [a-zA-Z_];

fragment CHINESE_DIGIT:
	'\u4E00' ..'\u9FA5'
	| '\uF900' ..'\uFA2D';

fragment DIGIT: [0-9];

fragment NONZERO_DIGIT: [1-9];

fragment OCTAL_DIGIT: [0-7];

fragment HEXADECIMAL_DIGIT: [0-9a-fA-F];

fragment BINARY_DIGIT: [01];

/* key words ↓ */

Null: 'null';
True: 'true';
False: 'false';
Var: 'var';
Global: 'global';
Const: 'const';
Property: 'property';
This: 'this';
Super: 'super';
Static: 'static';
Return: 'return';
Not: 'not' | '!';

/* key words ↑ */

Identifier:
	IDENTIFIER_NON_DIGIT (IDENTIFIER_NON_DIGIT | DIGIT)*;

ArrarIdentifier: Identifier '[]';
templateIdentifier: Identifier '<' templateparameterlist '>';
templateparameterlist: Identifier (',' Identifier)*;

fragment IDENTIFIER_NON_DIGIT: NON_DIGIT | CHINESE_DIGIT;

SemiColon: ';';
Comma: ',';
Colon: ':';
Less: '<';
Greater: '>';
Assign: '=';
LeftBrace: '{';
RightBrace: '}';
LeftParen: '(';
RightParen: ')';
LeftBracket: '[';
RightBracket: ']';
Dot: '.';
Plus: '+';
Minus: '-';
Star: '*';
Div: '/';
Mod: '%';
Caret: '^';
And: '&';
Or: '|';
Sharp: '#';
Equal: '==';
NotEqual: '!=';

Whitespace: [ \t]+ -> channel(HIDDEN);
Newline: ('\r' '\n'? | '\n') -> channel(HIDDEN);
BlockComment: '/*' .*? '*/' -> channel(HIDDEN);
LineComment: '//' ~ [\r\n]* -> channel(HIDDEN);

SHEBANG: '#' '!' ~('\n' | '\r')* -> channel(HIDDEN);