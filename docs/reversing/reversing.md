# Reversing

## Hardware

### Links

 - [CSCI 4974 / 6974 Hardware Reverse Engineering](http://security.cs.rpi.edu/courses/hwre-spring2014/)
 - [UPC UBEE EVW3226 WPA2 Password Reverse Engineering](https://deadcode.me/blog/2016/07/01/UPC-UBEE-EVW3226-WPA2-Reversing.html)
 - [im-me LCD Interface Hacked](https://daveshacks.blogspot.it/2010/01/im-me-lcd-interface-hacked.html)
 - [Reverse Engineering Flash Memory for Fun and Benefit](https://www.blackhat.com/docs/us-14/materials/us-14-Oh-Reverse-Engineering-Flash-Memory-For-Fun-And-Benefit-WP.pdf)
 - [Reverse Engineering the TP-Link HS110](https://www.softscheck.com/en/reverse-engineering-tp-link-hs110/)
 - [Embedded Devices Security Firmware Reverse Engineering](https://media.blackhat.com/us-13/US-13-Zaddach-Workshop-on-Embedded-Devices-Security-and-Firmware-Reverse-Engineering-Slides.pdf)

## Software

### Links

 - [Reverse engineering and removing Pokémon GO’s certificate pinning](https://eaton-works.com/2016/07/31/reverse-engineering-and-removing-pokemon-gos-certificate-pinning/)


### Plasma

Interactive disassembler for x86/ARM/MIPS. Generates indented pseudo-code with colored syntax code.

This is the [github repo](https://github.com/joelpx/plasma).

```
$ plasma -i /opt/low-level/docs/code/payload-eater_x86
plasma> help
analyzer 
      Analyzer information
dump SYMBOL|0xXXXX|EP [NB_LINES]
      Disassemble only.
exit 
      Exit
functions 
      Print the function list.
help 
      Display this help
hexdump SYMBOL|0xXXXX|EP [NB_LINES]
      Dump memory in hexa.
history 
      Display the command history
info 
      Information about the current binary
jmptable INST_ADDR TABLE_ADDR NB_ENTRIES SIZE_ENTRY
      Create a jump table referenced at TABLE_ADDR and called
      from INST_ADDR.
mips_set_gp ADDR
      Set the register $gp to a fixed value.
py [FILE]
      Run an interactive python shell or execute a script.
      The global variable 'api' will be accessible.
push_analyze_symbols 
      Force to analyze the entry point, symbols and a memory scan will be done.
rename OLD_SYM NEW_SYM
      Rename a symbol.
save 
      Save the database (only symbols and history currently).
sections 
      Print all sections
sym [SYMBOL 0xXXXX] [| FILTER]
      Print all symbols or set a new symbol.
      You can filter symbols by searching the word FILTER.
      If FILTER starts with -, the match is inversed.
x [SYMBOL|0xXXXX|EP]
      Decompile and print on stdout. By default it will be main.
      The decompilation is forced, it dosn't check if addresses
      are defined as code.
v [SYMBOL|0xXXXX|EP]
      Visual mode
      Shortcuts:
      c       create code
      b/w/d/Q create byte/word/dword/qword
      a       create ascii string
      p       create function
      o       set [d|q]word as an offset
      x       show xrefs
      r       rename
      /       binary search: if the first char is ! you can put an
              hexa string example: /!ab 13 42
      n/N     next/previous search occurence
      I       switch to traditional instruction string output
      M       show/hide mangling
      B       show/hide bytes
      g       top
      G       bottom
      z       set current line on the middle
      Q       quit
      ;       edit inline comment (enter/escape to validate/cancel)
      %       goto next bracket
      *       highlight current word (ctrl-k to clear)
      { }     previous/next paragraph
      tab     switch between dump/decompilation
      enter   follow address
      escape  go back
      u       re-enter (for undo)
display.print_section 
      Print or not section when an address is found
xrefs SYMBOL|0xXXXX|EP
      Print all xrefs.
```

### Vivisect

This the [github](https://github.com/pdasilva/vivisect) page.
