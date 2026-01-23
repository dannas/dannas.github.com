# Productivity vs. Uncertainty & Apathy
If you're not getting shit done, it may not be because you suck.  Traditionally
unproductive workers get nailed with labels like undisciplined, lack of focus,
or poor work ethic, but in my experience these are not nearly as influential as
I'd thought.

Over the past two decades of coding, including stints in management/as a
lead/and being self-directed with no immediate manager, I've come to the
conclusion that the main issues that impact productivity are uncertainty and
apathy.

When uncertainty kicks me in the nuts, it's usually in one of several flavors:
high-level uncertainty, strategic uncertainty, and low-level uncertainty.

High-level uncertainty, aka "Where am I and what an I trying to do?", is
when you have a general goal that comes with a set of implicit assumptions.
Those assumptions are a minefield you have to traverse as you try to get to
your finished product.  You end up paralyzed because you're not sure which
direction is safe, especially since none of the mines are marked.  For example,
you might have the task of "In the next month improve our network performance
by 50%".  It seems like a concrete goal, but there are still a ton of hidden
assumptions in there.


Solution: Constrain your problem so that what was once vague is now extremely
well specified.  Constraints tend to dictate solutions.  Okay, I have to
improve network performance by fifty percent.  Can I use more memory to do
this?  Can I use more CPU?  How much more CPU?  Can I use a third party
library?  Is our performance poor sending or receiving or both?  Under what
conditions?

Or, using our lost in the woods analog, get a goddamn map. Once questions like
those are answered the scope of the problem and available solutions becomes a
lot clearer, and we can move on to tackling the next problem.

Strategic uncertainty occurs when you have a well defined goal but you're
unsure how to go about it.  Using our previous example you've determined that
the problem is total network bandwidth, on both sides, and that you can
sacrifice a lot of CPU to do it.  It seems to happen all the time under all
conditions, and unfortunately most of the data going across the network comes
from a bunch of different systems so you can't just pare down the data at one
location.

Now you have to come up with a technical solution.  You know you need to
compress and decompress the data, but how?  Maybe you have no experience with
compression algorithms, so now you're kind of stuck.  Do you just use zlib?  Or
miniz?  Or do you license something?  Is there a hardware solution?  If there's
no clear solution at hand ("drop in zlib") then even very good programmers will
spin their wheels, unsure of how to tackle it.  They just hope for a brilliant
insight.

Solution: Grind it out by talking to someone.  Having an office mate helps
tremendously so you can just say "I have to do this thing, I'm not even sure
how to tackle it".  People like to sound smart and give their opinions, so give
someone else the opportunity to solve your problem for you!

In this case the simplest solution is to use a package like zlib to compress
and decompress your data.  Strategy defined!

This leaves the day to day tasks of getting it done, and here's where low-level
uncertainty can creep in.  Now you know what you're supposed to do at a fairly
specific level ("Implement zlib in our codebase so that the network layer is
much faster") but at any given moment you're just staring at your editor,
unsure what to do next.

Solution: Make a list.  It's that simple.  Instead of writing code, start by
getting yourself organized and enumerate all your fine grained tasks, no matter
how small.  "Find compression and decompression entry points".  "Get
compression code into build system".  "Implement dummy implementation to verify
integration works." "Write a test harness".  etc.  Once you have a list, sort
in terms of priority.  And if you have any tasks that would take longer than a
day, go ahead and break them out into smaller tasks.

You now have a clear road map from A to B, broken down into actionable items at
a very fine grained level.  There's no more uncertainty.  If you're driving,
this is the part where your GPS has figured out how to get you from  your
current location to the nearest Five Guys without having to ask for directions
or pull over.

So what do you do if you're still not getting shit done?!  The glib answer is
something along the lines of...

It's likely that you're just not interested in the work.  If you're passionate
and interested in something, then the work is fun and interesting and you want
to do it.  Unfortunately a lot of the work we do in software -- in fact, the
bulk of work in software development -- is excruciatingly dull.  Finding
obscure bugs.  Dealing with build system issues.  Hunting down compiler
switches.  Typing in the same code we've typed in hundreds of times before.
Dealing with poor or missing API documentation.

For this stuff, you just have to grind.  This is where work ethic and
discipline can come into play, but I prefer to cheat and just try to stick to
things I find interesting.  Unfortunately the amount of interesting things to
work on in a typical software project is dwarfed by the sheer amount of
bullshit tasks.

Brian Hook, posted 12th September 2012

http://bookofhook.blogspot.com/2012/09/productivity-vs-uncertainty-apathy.html

