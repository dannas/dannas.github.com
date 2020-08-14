[Djordjes Github Repositories](https://github.com/djolertrk?tab=repositories)
[A 15 minute introduction to DWARF](https://www.youtube.com/watch?v=OEa0EfJja_Y)
[Debug Information Validation for Optimized Code](https://helloqirun.github.io/papers/pldi20_yuanbo1.pdf)
[Compile Once Debug Twice: Picking a compiler for debuggability](https://engineering.backtrace.io/posts/compile-once-debug-twice/)

## [Implementing a Debugger](https://engineering.backtrace.io/posts/2016-08-11-debugger-internals/)
gdb 500k SLOC
/proc/$PID/maps gives mem map
debug info
	locations  func and global vars
	stack layout
	expr
	type of vars

ptrace
	PTRACE_ATTACH, PTRACE_DETACH
		establish debugger-debugee relationship
		allow the debugger to read mem/regs
	PTRACE_GETREGS, PTRACE_GETFPREGS
		get regs for specific thread

libthread_db
	get lists of threads
unwind
	start with rip
	Frame Description Entry (FDE)
	Canonical Frame Address (FDA)
	steps
		retrieve ret addr with libthread_db
		recurse: find calling frames next instr and sp
variables
	use rip and search dwarf for local variables

DWARF
	Debugging Information Entry (DIE)
		tag (DW_TAG_subprogram, DW_TAG_formal_parameter, DW_TAG_variable...)
		attribute set
			DW_AT_name
			DW_AT_decl_file
			DW_AT_decl_line
			DW_AT_low_pc
			DW_AT_high_pc
			DW_AT_frame_base
			DW_AT_type
			DW_AT_sibling
			DW_AT_location

readelf ---debug-dump=info EXECUTABLE
readelf ---debug-dump=frames-interp EXECUTABLE

## Symbolic Debugging with DWARF
.debug_line mem addr -> line numbers
.debug_info type, var and fun info
.debug_frame unwind info by mem
.eh_frame unwind info by mem

## Github repositories

LLVM DI Checker
	collect debug info before each pass
	check if any info was dropped

## Debug Information Validation for Optimized Code
Code Location Problem
Data Value Problem

## Symbolic Debugging of Optimized Code, Hennesey
Do not affect symbolic debugging, since no stores are altered
	constant folding
	copy propagation
	global common subexpression elimination
	code hoisting (provided only expr are hoisted)
	loop unrolling
Do affect
	code motion
	induction variable elimination
	dead variable elimination
	

