---
layout: post
title: Kernighan software tools in rust
---

<!-- excerpt start -->
I'm trying to learn Rust. So far I've read "The Book" and done the rustlings exercises. For hands-on practice I've implemented a few commandline utilities. I've selected a few that Brian W Kernighan uses in his language help files and present my versions here. I only have about one week of rust experience so beware: there are many mistakes.
<!-- excerpt end -->

Here are some of Brian Kernighans cheat sheets for [python](https://www.cs.princeton.edu/courses/archive/spr09/cos333/python.help), [perl](https://www.cs.princeton.edu/courses/archive/spr09/cos333/perl.help) and [awk](https://www.cs.princeton.edu/courses/archive/spr09/cos333/awk.help). And his [Software Tools in Pascal](https://www.amazon.com/Software-Tools-Pascal-Brian-Kernighan/dp/0201103427/) book co-written with P.J. Plauger contains a great collection of small programs to implement. The programs I've written doesn't use any commandline options and they are really basic, but they do highlight some common language constructs in Rust.

<div id="inline_toc" markdown="1">

* TOC
{:toc}
</div>

## Echo

The arguments are accessed as an iterator. It can be collected into a vector. The `collect` function is very versatile. If `args` was a String it would have
concatenated all arguments into one string. A weird thing is that the `print!`macro does borrow its arguments but no ampersand is required.

    use std::env;
    
    fn main() {
        let args: Vec<String> = env::args().collect();
        let mut sep = "";
        for arg in &args[1..] {
            print!("{}{}", sep, arg);
            sep = " ";
        }
        println!();
    }

You can often replace for loops with chained iterators. An annoying thing is that rustfmt wants to join those chained iterator calls into one line.

    use std::env;
    
    fn main() {
        println!("{}", env::args()
            .skip(1)
            .collect::<Vec<String>>()
            .join(" "));
    }

## Cat using fs::read_to_string
The whole contents of a file can be read using `fs::read_to_string`. For most cases that's good enough, but you do risk denial-of-service attacks and it won't work with character devices.

As I understood it, a common idiom for command line programs is to provide a run function that returns a Result to main. As the program grows the run function is moved into a lib.rs file due to it being easier to unit test parts there?

It's annoying that io::Error doesn't store the path for the operation that failed. Seems like many people use the `anyhow` crate for providing that kind of
context.

The run function takes a string slice as parameter but the argument is of type String. Works since String provides a deref to str. Many APIs uses slices instead of the owned types for parameters.

The `?` operators are shorthands for returning an error from the Result and otherwise unwrapping the containing value. Makes for succint code but you do have to decide what Error types to use.

    use std::{io, env, fs};
    use std::io::Writer;
    
    fn run(path: &str) -> io::Result<()> {
        let buf = fs::read_to_string(path)?;
        print!("{}", buf)?;
        Ok(())
    }
    
    fn main() {
        for path in env::args().skip(1) {
            run(&path).unwrap_or_else(|err| {
                eprintln!("cat: {}: {}", path, err);
            });
        }
    }

## Cat line-by-line

Why is the BufRead symbol imported even though it's never used? It's a trait and it needs to be available when calling the lines() function.

    use std::env;
    use std::io::{self, BufReader, BufRead};
    use std::fs::File;
    
    fn run(path: &str) -> io::Result<()> {
        let reader = BufReader::new(File::open(path)?);
        for line in reader.lines() {
            println!("{}", line?);
        }
        Ok(())
    }
    
    fn main() {
        for path in env::args().skip(1) {
            run(&path).unwrap_or_else(|err| {
                eprintln!("cat: {}: {}", path, err);
            });
        }
    }

The error handling seems to work for most cases.

    $ ./cat2 dontexist.txt
    cat: dontexist.txt: No such file or directory (os error 2)
    
    $ ./cat2 /proc/self/mem # A trick for testing I/O errors
    cat: /proc/self/mem: Input/output error (os error 5)

On the other hands, cat2 panics for these errors

    $ ./cat2 test.txt | false
    thread 'main' panicked at 'failed printing to stdout: Broken pipe (os error 32)', library/std/src/io/stdio.rs:1016:9
    
    $ ./cat2 test.txt > /dev/full
    thread 'main' panicked at 'failed printing to stdout: No space left on device (os error 28)', library/std/src/io/stdio.rs:1016:9

Turns out println! panics instead of returning an error. I should use`write!` and `writeln!` instead.

    fn run(path: &str) -> io::Result<()> {
        let reader = BufReader::new(File::open(path)?);
        for line in reader.lines() {
            let line = line?;
            write!(io::stdout(), "{}\n", line)?;
        }
        Ok(())
    }

## Freq

There isn't a canonical tool for counting the frequencies of words in a text,  but it's a great exercise for using associative arrays and sorting. The `HashMap` type has a clever `entry` API for updating existing values or inserting default ones. Removes the big foot gun that C++ std::maps operator[] represents, where it's so easy to accidentally insert unintended values.

```
fn run() -> io::Result<()> {
    let mut freq = HashMap::new();
    for line in io::stdin().lines() {
        for word in line?.split_whitespace() {
            let count = freq.entry(word.to_string()).or_insert(0);
            *count += 1;
        }
    }
    let mut words : Vec<_> = freq.iter().collect();
    // Sort in reverse order
    words.sort_by(|a, b| { b.1.cmp(a.1) });

    for (word, n) in &words {
        println!("{:<20} {:>5}", word, n);
    }
    Ok(())
}
```

## Find

There doesn't seem to exist any simple API for recursivel walking a directory tree. I have to write my own or use an external crate.

```
fn main() -> io::Result<()> {
    let args: Vec<String> = env::args().skip(1).collect(); 

    let needle = Path::new(&args[0])
        .file_name()
        .unwrap();

    let mut stack = vec![PathBuf::from(".")];

    'outer: loop {
        if stack.is_empty() {
            break;
        }
        // Safe since stack is non-empty
        let root = stack.pop().unwrap();
        for entry in fs::read_dir(root)? {
            let dir = entry?;
            let f = dir.file_type()?;
            if f.is_dir() {
                stack.push(dir.path());
            } else {
                if dir.file_name() == needle {
                    println!("{}", dir.path().display());
                    break 'outer;
                }
            }
        }
    }
    Ok(())
}
```

## Tail using VecDeque

Printing the last N lines of a file can be done either by reading all lines and keeping track of the N last read or by starting from the end and scanning
backwards for N newline tokens.

VecDeque is a circular buffer, there's no overwriting mode so I'll have to add that myself. 

    #[derive(Debug)]
    struct RingBuffer {
        buf: VecDeque<String>,
    }
    
    impl RingBuffer {
        fn new(cap: usize) -> Self {
            RingBuffer {
                buf: VecDeque::with_capacity(cap),
            }
        }
    
        fn push(&mut self, line: &str) {
            if self.buf.len() == self.buf.capacity() {
                self.buf.pop_front();
            }
            self.buf.push_back(line.into());
        }
    }

Then the file reading can be done with this snippet.

    fn run() -> io::Result<()> {
        let mut rb = RingBuffer::new(10);
        for line in io::stdin().lines() {
            rb.push(&line?);
        }
        for line in &mut rb.buf {
            writeln!(io::stdout(), "{}", line)?;
        }
        Ok(())
    }
    
    fn main() {
        run().unwrap_or_else(|err| {
            eprintln!("tail: {}", err);
            process::exit(1);
        })
    }

## Tail using my own RingBuffer

I tried writing my own RingBuffer type without relying on a pre-existing collection. It uses two unmasked, unsigned indices that wraparound on overflow.
It takes ownership of the strings read.

    #[derive(Debug)]
    struct RingBuffer {
        buf: Vec<String>,
        read: usize,
        write: usize,
        cap: usize,
    }

By letting `buf` size be a power of two I can easily mask out the positions for the indices.

    fn mask(index: usize, cap: usize) -> usize {
        index & (cap.next_power_of_two() -1)
    }

I'm creating a `Vec` of a fixed power-of-two size. 

    impl RingBuffer {
        fn new(cap: usize) -> Self {
            RingBuffer {
                buf: vec!["".into(); cap.next_power_of_two()],
                read: 0,
                write: 0,
                cap,
            }
        }
        fn push(&mut self, line: String) {
            let i = mask(self.write, self.cap);
            self.buf[i] = line;
            if self.write - self.read == self.cap {
                self.read += 1;
            }
            self.write += 1;
        }
    }

I added an iterator for my Ringbuffer. It should be possible to reuse the lifetime of the internal `Vec` but I wasn't able to do that. So I resorted to cloning the data.

```
impl Iterator for RingBuffer {
    type Item = String;

    fn next(& mut self) -> Option<Self::Item> {
        if self.read == self.write {
            None
        } else {
            let i = mask(self.read, self.cap);
            self.read += 1;
            Some(self.buf[i].clone())
        }
    }
}

fn run() -> io::Result<()> {
    let mut rb = RingBuffer::new(10);
    for line in io::stdin().lines() {
        rb.push(line?);
    }
    for line in &mut rb {
        writeln!(io::stdout(), "{}", line)?;
    }
    Ok(())
}
```

## Grep

I ported Rob Pikes code from the Practice of Programming, a recursive matcher that handles concatenation and repetion but not alternation. Rusts string type is encoded as utf-8, but I simplified the problem and only dealt with byte-strings. 

The code starts off by searching for a match anywhere in the text:

```
fn is_match(regex: &[u8], text: &[u8]) -> bool {
    if regex.len() > 0 && regex[0] == b'^' {
        return match_here(&regex[1..], &text);
    }
    // We need to check even if text is empty
    if match_here(regex, text) {
        return true
    }
    for i in 0..text.len() {
        if match_here(regex, &text[i..]) {
            return true
        }
    }
    false 
}
```

The `match_here` call takes care of matching for either a repeated character or an exact match. It relies on slice pattern matching which is somewhat involved. Thank you StackOverflow user Rodrigo for this solution, https://stackoverflow.com/a/73561795/582010. 

```
fn match_here(regex: &[u8], text: &[u8]) -> bool {
    match (regex, text) {
        (&[], _) => true,
        (&[b'$'], &[]) => true,
        (&[b'$'], _) => false,
        (&[r, b'*', ref rs @ ..], txt) => {
            match_star(r, rs, txt)
        }
        (&[_, ref _s @ ..], &[]) => false,
        (&[r, ref rs @ ..], &[t, ref ts @ ..]) if r == b'.' || r == t => {
            match_here(rs, ts)
        }
        _ => false,
    }
}
```

Finally we have the matching of the star expression.

```
fn match_star(c: u8, regex: &[u8], text: &[u8]) -> bool {
    if match_here(regex, text) {
        return true;
    }
    let mut i = 0;
    while i < text.len() && (text[i] == c || c == b'.') {
        if match_here(regex, &text[i..]) {
            return true;
        }
        i += 1;
    }
    false
}
```

And actually matching on text can be done like this:

```
fn main() {
    if let Some(regex) = std::env::args().skip(1).next() {
        for line in std::io::stdin().lines() {
            let line = line.unwrap();
            if is_match(regex.as_bytes(), line.as_bytes()) {
                println!("{}", line);
            }
        }
    }
}
```

