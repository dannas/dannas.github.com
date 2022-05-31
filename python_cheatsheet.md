# Containers
## list
L = []                      # Create empty list
L.append(val)               # Append val
L += other                  # Concatenate with list other

## dict
D = {}                      # Create empty dict
D[key] = val
D = defaultdict(int)        # Create dict that takes default values
key in D                    # Is key present in D?
val = D.get(key, default)   # Return D[key] or default if key is missing
del D[key]                  # Remove D[key]

## Regular expressions
L = re.findall(pat, str)
b = re.fullmatch(pat, str)
m = re.match(pat, str)

## Strings
t = str.maketrans(from, to)
copy = s.translate(t)
str.format(args)

enum
data classes
format strings
underscores in numerical literals
pathlib
type hinting
walrus operator
extended iterable unpacking
