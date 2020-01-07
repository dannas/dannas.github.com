<style type="text/css" rel="stylesheet"> article { white-space: pre; } </style>

# C++ Concepts
entities
    objects
    references
    structured bindings
    functions
    enumerators
    types
    class members
    templates
    template specializations
    namespaces
    parameter packs

entities are introduced by declarations

functions := sequence of stmts
name lookup
    qualified
        on the right side of ::
    unqualified
scope
linkage


# Input/Output
ios::sync_with_stdio(0);                    // don't sync C and C++ streams
cin::tie(0);                                // don't auto-flush between cin and cout calls
getline(cin, str);                          // read one line
while (cin >> val) use(val);                // read a val

# Numbers
strtoll
strtoull
strtof
strtod

# Random

# Math
r = log10(val);
r = log2(val);
r = pow(base, exp);
r = sqrt(val);
h = hypot(x, y);
sin,cos,tan

# Containers
n = s.size();                               // size
    Having it named size() is confusing when it returns length, not number of
    bytes. But I guess length gives the impression that the content is stored linearly which
    is not true for the map/set containers.
for (auto val : s) use(val);                // iterate
    Remember t
T c = {...};                                // construct via initializer_list
it = begin(c);
it = end(c);
it = back_inserter(c);

# Vector
    Vector should have been called Array or DynArray. Naming is hard.
vector<int> v;                              // construct
vector<int> v(n, val=T)                     // construct n elts
v.resize(n, val=T);                         // create n elts with value val
v.reserve(new_cap);                         // v.buf = malloc(new_cap)
v.push_back(val);                           // append val
v.insert(end(v), b, e);                     // append other[b:e] to v

# String
string s;                                   // construct
string s(n, c);                             // construct n chars 
s.resize(n, c=T);                           // create n chars, default '\0'
s.push_back(c);                             // append a char
s.append(str);                              // append a string
s.append(n, c);                             // append n characters
s.append(str, pos, n);                      // append str(pos, pos+n)
s = s.substr(pos, n);                       // strdup + strlcpy(s, s+pos, n)
for (char c : s) use(c);                    // iterate
pos = s.find(c, pos=0);                     // strchr(s+pos, c)
pos = s.find(str, pos=0);                   // strstr(s+pos, str)
pos = s.find_first_of(str, pos=0);          // strpbrk(s+pos, str)
pos = s.find_first_not_of(str, pos=0);      // strspn(s+pos, str)
pos = s.find_last_of(str, pos=npos);        // strrpbrk(s+pos, str)
r = s.compare(other);                       // strcmp(s, other)
r = s.compare(i, n, other, j, m);           // strncmp(s[i:n], other[j:m], min(n, m)

# Set
set<int> s;                                 // construct empty set
s.insert(key);
n = s.count(key);
for (int key : s) use(key);

# Map
map<string, int> m;                         // See [^chrome containers]
val = m[key];                               // Get val. NOTE: [key,val is created if key not in m][CHEN-2019]
m[key] = val;                               // Constructs (key, val). NOTE: [Double construction, perf problem][CHEN-2019]
m.insert_or_assign(key, val);
val = m.at(key)
n = m.count(key);                           // Check if m contains key
for (auto &kv : m) use(k->first,k->second); // iterate

# Algorithms
sort(b, e);                                 // qsort
sort(b, e, less);                           // qsort with bool less(x, y) comparator
stable_sort(b, e);                          // mergesort
stable_sort(b, e, less);                    // mergesort with bool less(x, y) comparator
val = accumulate(b, e, start_val);
reverse(b, e);
i = find(b, e, val);
val = min({...});                             // Return min. Requires [^initializer_list]
val = max({...});

# Time Estimates
input size    required time complexity
n≤10        O(n!)
n≤20        O(2^n)
n≤500       O(n^3)
n≤5000      O(n^2)
n≤10^6      O(n log n) or O(n)
n is large  O(1) or O(log n)

# Competetive programming specific
const int INF = 0x3f3f3f3f;                 // Since INF+INF does not overflow and you can set an array to infinity with memset(a, 0x3f, sizeof(a))
#define MOD 1000000007
#define EPS 1e-9

# Common Loops
// Iterate with indices
for (size_t i = 0; i < N; i++) 
    use(v[i]);

// Iterate without indices
for (auto i = begin(v); i != end(v); ++i) 
    use(*i);

for (auto &val : v) 
    use(val);

// TODO: Two pointer techniques https://github.com/liyin2015/Algorithms-and-Coding-Interviews/blob/master/two_pointer.pdf
// TODO: Sliding window techniques https://medium.com/outco/how-to-solve-sliding-window-problems-28d67601a66


// iterate over spans delimited by c
for (size_t i = 0; i < N; i++) {            
    if (v[i] == c) {
        size_t j = i;
        while (j < N && v[j] != c) j++;        
        use(i, j);
        i = j-1;
    }
}

// Find longest span
for (size_t i = 0; i < N; i++) {
    if (i == 1 || pred(v[i-1], v[i])) {
        m = max(m, ++len);
    } else {
        len = 1;
    }
}

// Count matching substrings
int prev = 0, curr = 1, n = 0;
for (size_t i = 1; i < s.size(); i++) {
    if (s[i-1] != s[i]) {
        n += min(prev, curr);
        prev = curr;
        curr = 1;
    } else {
        curr += 1;
    }
}
return n + min(prev, curr)

// Count pairs
for (size_t i = 0; i < N; i++) {
    int x = f(v[i]);
    n += m[x];
    m[x]++;
}

// Max subarray sum (Kadanes algorithm)
for (size_t i = 0; i < n; i++) {
    sum = max(v[i], sum + v[i]);
    best = max(best, sum);
}

// Binary search
int k = 0;
for (int b = n/2; b >= 1; b /= 2) {
    while (k+b < n && v[k+b] <= x) k += b;
}
if (v[k] == x) {
    // x found at index k
}

// Finding pos where function changes TODO: verify correctness
// z is initial jump length. Must be large enough and point to value
// that is known to be true.
int x = -1;
for (int b = z; b >= 1; b /= 2) {
    while (!ok(x+b)) x += b;
}
int k = x+1;

// Finding max value TODO: verify correctness
// z is initial jump length. Must be large enough and point to value
// that is known to be true. Consecutive values must not be equal.
int x = -1;
for (int b = z; b >= 1; b /= 2) {
    while (f(x+b) < f(x+b+1)) x += b;
}
int k = x+1;


[CHEN-2019]: https://devblogs.microsoft.com/oldnewthing/2019022

[^initializer_list]: You need to supply the args as std::initializer_list if >2.
[^chrome containers]: std::map is O(log n) for insert/lookup but has stable
iterators. std::unordered_map has a high space overhead. The [Chrome developers
uses std::map as default choice](https://chromium.googlesource.com/chromium/src/+/master/base/containers/README.md)

set_union and set_difference are fast for vector, slower for unordered_set and slowest for set. https://lemire.me/blog/2017/01/27/how-expensive-are-the-union-and-intersection-of-two-unordered_set-in-c/.  
TODO Output iterators: back_inserter uses push_back and is faster than inserter.
