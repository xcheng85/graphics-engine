# C++

utf-8 encoding
nested namespace

## c-style string and c++ string

<cstring>
<string>

'\0': strlen() does not include the \0.
sizeof(): // dangerous, don't use
strcmp():

"aaa" // zero-terminated c-string
"aaa"s // c++ string

char to be replaced by std::byte to represent byte: <cstddef>

stl string: template class specialization
.c_str();
.data();
substr(pos, len)
find(substr)
string::npos
contains(substr)
starts_with
ends_with
""s

to_string()://template func
stoi()
stof
stod

## numeric_limits

<numeric_limits>

### for uninitialized value

1. -numeric_limits<float>::infinity()
2. std::optional

class template and static function: max, min, lowest, infinity()

casting types: static_cast<>(), loss of data or wrong

<cmath>
isnan() isinf()

operator: unary, binary, tenary (1, 2, 3)
xor: ^

enum class: int

c-array: sizeof, size(),
stl array: .size(), iterator, constexpr

optional: class template, nullopt, has_value(), value(), value_or()

structural binding: struct, pair,

container's range based loop: 1. c-array 2. initialize-list 3. containers

## initialze_list: class template

overload ctor for initializer_list

Struct's designated initializer:

{
.xxx = xxx
}

## const + pointer

1. const auto\* p =
2. auto\* const p =
3. auto const\* p =
4. auto const\* const p =

const_cast<>
std::as_const()

## reference to rvalue (must be const)

const auto& v{rvalue}

## passing litrals (rvalue)

same rules apply to function argument: rvalue reference must have "const"

## ref vs pointer

ref: safe, no null ref,

pointer: polymorphisim in stl container, optional(nullptr), change the pointer address,

## exception

<stdexcept>
1. invalid_argument{}

## auto and decltype

auto: strip away const and reference
decltype: does not strip away

## operator overload

move ctor and move assignement: noexcept

### 1. Member overload vs global overload

return type: same --> member; return reference for chaining
different: global, ostream ex.

### 2. operator[]

two overloads; const and non-const
pattern of const_cast to reuse code idiom

### 3. operator++ and operator++(int)

return type, reference vs new object, performance diff

## iterator

### use cases: round robin

round robin, and pool-based design,
one container and iterator to manipulate.

### 1. iterator types

1. input: read
2. output: write
3. forward
4. bidirectional
5. random access
6. continuous

### 2. ++itr vs itr++

itr return reference
itr++, return new object.

### 3. iterate-based loop vs ranged-based for loop ?

iterator is for sub-range loop.

### 4. advance iterator template function

std::advance(itr): useful for non-continous iterators.

### 5. itertor adaptors: output (write )iterator

use case: copy/move between containers

insert_iterator
back_insert_iterator: push_back()
front_insert_iterator: push_front()
back_inserter()

move_iterator: subrange move.

### 6. iterator conceptor

### 7. stream iterators

istream_iterator
ostream_iterator

operator << >>

## container

template class
dynamic:
static size: array and bitset

### 1. requirement on T

1. T is copycase

T must copayble, copy ctor and copy assignment

emplace: don't trigger copy

2. T: reference
   store reference in container

reference_wrapper, get()
std::ref()
std::cref()

alert of lifecycle the what is referenced

3. T: pointer
   smart pointer vs regular pointer

raw pointer: explictly delete memory

4. allocator

5. map, set, std::less, operator< const

6. unorded_map, unordered_set, operator==

7. container comapre: operator !=, <=>, etc

### 2. compile-time computation

vector: constexpr
array:

### 3. vector

assign: re-initialize vector. (memory does not update?)
swap();

insert(): rvalue save copy, T must be movable
push_back: rvalue save copy, T must be movable
emplace: variadic, + ctor, return the iterator of new element, in-place.

insert:

1. one element
2. range from pair of input iterators(read)

### vector use case: remove all elements with a condition

1. for-loop + member erase: O(n2)
2. remove-erase-idom: o(n) // remove move them to the end, and one erase
3. std::erase algorithm // after c+=20

erase: by iterator or pairs of itr

### vector's memory probe: debug, preallocate and reclaim

size()
capacity()

preallocate memory()
pmr, arena,
or

reserve(): need to know the size
resize()
assign:

shrink_to_fit(); not 100%
swap to a tmp vector; tmp vector will be destructed; then memory is released in heap by default
swap idiom

### raw data of vector

.data(); // const char\*

### deque vs vector

1. not memory continous
2. no memory management like reserve.capacity
   remove-erase-idiom

### list

bidirectional iterator
double-linked list
front()
back()
push_back()
push_front()
no operator[]: no random access, std::advance can help, std::sort cannot sort list

insert():
merge()
splice(): move list to a list based on itr

### sorting list

std::sort does not work on list: list is not random-access
list.sort()

### queue

### use case: network packat buffer

T, Container=deque

### pq

T, container=vector, compare=less<T>

### stack

T, container=deque

### map

operator <
balanced tree, red-black tree

tempalte class;
key, value, compartor type: std::less, allocator

### map insert nuance

operator[]: always succeed, requires non-const caller
insert({}): return pair [itr, result] succeed or faliure, like go, like setnx in redis
insert_or_assign(), like set in redis

emplace:

### map searching nuance

operator[]: cannot use with const calller
find
at(): thrwo exception
count(): pre-check
contains(): pre-check

### map removeing

erase(key)
erase(by iterator)

### multimap:

lower_bound(k)
upper_bound(k)
equal_range(k)

### set

insert pair of iterators
emplace:
erase key
contains key

### set to vector<>

vector ctor with two iterators of set

### hash function

### custom class as unordered_map key

two requirements: arg2, arg3 of template class unorded_map

1. write custom hash function is needed
2. operator ==

<functional>
std::hash class template: a template class functor.

a template specialization for custom class in namespace std;

### unordered_map

key, T, hash<key>, std::equal_to<key>, allocatior

linked list of each bucket

### bitset<N>

bitset & bitset
.set
operatorp[]
.flip(bit)

### string to bitset

### bitset to string

to_string()

## algorithm

constexpr

### requriements on iter type

### find and find_if elements in container

operator==
operator<

pair of iter: search range
T: target
or predictor, lamda

find_if_not

### counting in container

count_if: pair of itr + predict

### generate (like map in typescript)

in-place only
pair of itr and lamda

### transform

in-place or not inpalce
pair of itr as inputs
one itr as dst,
predict

### copy without insert

preallocate

### copy with insert

back_inserter

### copy selection

copy_if, return the itr passed

### move container

T needs move assignemnt

pair of itrs
one itr as dst

### replace

pair
predict
relace value
in-place

### erase elements from container

container and lamda

approach1: member function
bad performant and error-prone

approach2: remove-erase-idom
remove: use move assignemnt
return itr (begin of range to be erased)

approach3: c++20 std::erase

### reverse

pair of itr to reverse from

### for_each vs transform vs generate

for_each no generation, just operate on lamda

pair of itr and lamda

### partition

in-place
pair of itr
lamda

### partition_copy

not in place
pair of itr
dstitr1
dstitr2

### sort

pair of itr

is_sorted
is_sorted_until: return itr beginning of not suorted

### nth_element

usefual for quick sort partition step,

// k+1 th largest
nth_element(begin(values), begin() + k, end(value), greater<>())
// k+1 th smallest
nth_element(begin(values), begin() + k, end(value), less<>())

o(n)
partition based on the k+1 anchor

smaller anchor larger

### op on sorted array

lower_bound: pairs of itr and value
upper_bound;
binary_search: pairs of itr and search value, return bool

### subset for container

includes: two pairs of itr

### union, interaection, diff, merge sort

not inplace
two pairs and 1 itr

### min_element

pair of itr
min, max, minmax: initialize_list

### clamp

### iota

pair of iterator and initial value

### accumulate vs reduce

pair of iter
intitial value
lamda: int(int,int)

accumulate: operator+
reduce: std::plus

transform_reduce
transform first and then reduce

### prefix sum

exclusive_scan()

### random number generation

    // mimic random exception
    random_device seeder; // functor
    const auto seed{seeder.entropy() ? seeder() : time(nullptr)};
    // use seeder for the software engin
    mt19937 engine{seed};
    // distribution
    uniform_int_distribution<int> distribution{1, 10};
    auto randomSeconds = distribution(engine);

### stateful and stateless callback

functor with state
std::ref

## template metaprogramming

typename is needed for the types based on T
