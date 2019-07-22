
// literal
true
false
1
0.5
"abc"
null

// initializer_array
[0,1,2,3]
// initializer_object
{"a":1, "b":2, "c" : [1, 2] }

// types
bool
int
float
type number = int | float; // (Union Types)
string
any
array<int> // template type;
function<null,int, int> // function type

// variable 
const a = 1;                        // const int
var b = 2;                          // int
const s = "hello";                  // const string
const s2:string = "world";          // const string
var numbers = [0,1,2];              // array<int>
var datas = [1, 2, null , "aa"]     // array<any> 


func add(a: int, b: int): int
{
    return a + b;
}

var add2 = func(a: int): int
{
    return a + 2;
}

for (a in list)
{
    print(a)
}