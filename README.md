MiniCD
======
aka Minimalistic C run-rime library for D.
A principled Win32/64 C standard library with focus on simplicity and speed.

See also musl project as examplar POSIX project with simillar goals, 
barring this project's D-centric name.

Goals
- no dependencies on CRT of _any_ kind; only (kernel32.dll & ntdll.dll)
- no conflicts with other CRTs, suitable for any 3rd party D compiler
- route most "stuff" to user-mode OS API, as of Win2000+ Win32 has TONS of good stuff
- minimal binary ( < X Kb ) and source code size ( < 2KLOC)
- Opensource with Boost license
- UTF8/16 as the only codepages supported, char* functions are UTF-8 
- ARM & x86_64 ready

Specifically its primary use case is a slickest CRT drop-in repalcement 
library for the D programming language on Windows.
 
What is not a goal:
- full pedantic ANSI compatibility (I don't care much but if anything I look at ersatz C11)
- typical non-standard extensions
- redoing FP math (as D has it's own and spuperior alternative)
- *any* kind of support for versions of Windows that MS itself doesn't support