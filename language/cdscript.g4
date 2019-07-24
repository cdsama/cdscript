grammar cdscript;

options {
	language = Cpp;
}

chunk: block EOF;

block: statement*;

statement:
	literal ';'
	| declarevariable ';'
	| assignmentexpression ';';

declarevariable: varModifier varType (',' varType)*;

varModifier: Var | Global;

constModifier: Const | Static;

varType: Identifier (':' typeIdentifier)?;

typeIdentifier:
	Identifier
	| ArrarIdentifier
	| templateIdentifier;

literal: Null | booleanliteral | numberliteral;

booleanliteral: True | False;
numberliteral: IntegerLiteral | FloatingLiteral;

assignmentexpression:
	Identifier '=' expression
	| declarevariable '=' expression
	| constModifier varType '=' expression;

expression: literal;

IntegerLiteral:
	DecimalLiteral
	| OctalLiteral
	| HexadecimalLiteral
	| BinaryLiteral;

fragment DecimalLiteral: NONZERO_DIGIT ('\''? DIGIT)*;

fragment OctalLiteral: '0' ('\''? OCTAL_DIGIT)*;

fragment HexadecimalLiteral: ('0x' | '0X') HEXADECIMAL_DIGIT (
		'\''? HEXADECIMAL_DIGIT
	)*;

fragment BinaryLiteral: ('0b' | '0B') BINARY_DIGIT (
		'\''? BINARY_DIGIT
	)*;

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
Static: 'static';

/* key words ↑ */

Identifier: IdentifierNON_DIGIT (IdentifierNON_DIGIT | DIGIT)*;

ArrarIdentifier: Identifier '[]';
templateIdentifier: Identifier '<' templateparameterlist '>';
templateparameterlist: Identifier (',' Identifier)*;

fragment IdentifierNON_DIGIT: NON_DIGIT | CHINESE_DIGIT;

SemiColon: ';';
Comma: ',';
Colon: ':';
Less: '<';
Greater: '>';
Assign: '=';

Whitespace: [ \t]+ -> channel(HIDDEN);
Newline: ('\r' '\n'? | '\n') -> channel(HIDDEN);
BlockComment: '/*' .*? '*/' -> channel(HIDDEN);
LineComment: '//' ~ [\r\n]* -> channel(HIDDEN);

SHEBANG: '#' '!' ~('\n' | '\r')* -> channel(HIDDEN);