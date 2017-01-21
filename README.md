# Compiling

`gcc -o 1xpad 1xpad.c`

# Usage

### Generating a key:

Output via stdout.

`./1xpad -g <key_size_in_mb>`

### Encrypt/decrypt a message:

Input is via stdin, output is stdout

`./1xpad keyfile`
