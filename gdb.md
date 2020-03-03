info sources   <REGEX>          # Print list of source with debug information
info functions <REGEX>          # Print list of functions
info variables <REGEX>

info address <SYMBOL>
info symbol <ADDRESS>

file EXECUTABLE
symbol-file
add-symbol-file

set substitute-path
directory


whatis <type>
ptype

x/FMT ADDRESS                   # fmt = count-format-size. 
                                # x=hex, c=chars, d=signed integers, i=instructions, s=C-strings
p *array@len
p/x (short[2])0x12345678

set write on                    # Enable patching
return                          # Return early from function
jump FUNCTION                   # Jump to location

Watching memory or variables. -l (for location) means that the address of expr is watched
The hardware uses special comparator registers.
    Cortex-M0 4 breakpoints, 2 watch points
    Cortex-M3/M4 6 breakpoints, 4 watch points
When singlestepping 1 hw bkpt register is temporarily used.

There are many gdb frontends that supports setting watchpoints on memory and variables
https://mcuoneclipse.com/2018/08/12/tutorial-catching-rogue-memory-accesses-with-arm-watchpoint-comparators-and-instruction-trace/
https://mcuoneclipse.com/2018/08/11/tutorial-catching-rogue-memory-accesses-with-eclipse-and-gdb-watchpoints/
https://mcuoneclipse.com/2012/04/29/watchpoints-data-breakpoints-in-mcu10/
    watch EXPR                     # Stop wath written to expr
    awatch EXPR                   # Stop when read/written to expr
    rwatch EXPR                   # Stop read to expr
You need to dereference a numerically entered address
    (gdb) watch 0x600850
    Cannot watch constant value 0x600850.
    (gdb) watch *(int *) 0x600850
    Watchpoint 1: *(int *) 6293584
