# References

## exit

>Accepts an exit code and terminates current process.

### Arguments

### Returns

### Details


## string_length

>Accepts a pointer to a string and returns its length.

### Arguments

### Returns

### Details


## print_string

>Accepts a pointer to a null-terminated string and prints it to stdout.

### Arguments

### Returns

### Details


## print_char

>Accepts a character code directly as its first argument and prints it to stdout.

### Arguments

### Returns

### Details


## print_newline

>Prints a character with code 0xA.

### Arguments

### Returns

### Details


## print_uint

>Outputs an unsigned 8-byte integer in decimal format.
><br>We suggest you create a buffer on the stack and store the division results there. Each time you divide the last value by 10 and store the corresponding digit inside the buffer. Do not forget, that you should transform each digit into its ASCII code (e.g., 0x04 becomes 0x34).

Parse the unsigned number written in string and return as number.

### Arguments

* rdi: the address of the string

### Returns

* rax: the number parsed.
* rdx: the length of the number when the number is written as decimal.

### Details

If there is both of numbers and alphabets, only numbers will be parsed. It will not throw any error.


## print_int

>Output a signed 8-byte integer in decimal format.

Parse the signed number written in string and return as number.

### Arguments

* rdi: the address of the string

### Returns

* rax: the number parsed.
* rdx: the length of the number when the number is written as decimal.

### Details

If there are both of numbers and alphabets, only numbers will be parsed. It will not throw any error.

The negative sign `-` is also counted as the length. For example, the length of `-1` is `2`, not `1`.

If there are only characters without number, `0` will be returned as the number.


## read_char

>Read one character from stdin and return it. If the end of input stream occurs, return 0.

### Arguments

### Returns

### Details


## read_word

>Accepts a buffer address and size as arguments. Reads next word from stdin (skipping whitespaces into buffer). Stops and returns 0 if word is too big for the buffer specified; otherwise returns buffer address.
><br>This function should null-terminate the accepted string.

### Arguments

### Returns

### Details


## parse_uint

>Accepts a null-terminated string and tries to parse an unsigned number from its start.
><br>returns the number parsed in rax, its characters count in rdx.

### Arguments

### Returns

### Details


## parse_int

>Accepts a null-terminated string and tries to parse a signed number from its start.
><br>Returns the number parsed in rax; its characters count in rdx (including sign if any). No spaces between sign and digits are allowed.

### Arguments

### Returns

### Details


## string_equal

>Accepts two pointers to strings and compares them. Returns 1 if they are equal, otherwise 0.

### Arguments

### Returns

### Details


## string_copy

>Accepts a pointer to a string, a pointer to a buffer, and buffer's length. Copies string to the destination. The destination address is returned if the string fits the buffer; otherwise zero is returned.

### Arguments

### Returns

### Details
