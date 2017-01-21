# Compiling

`gcc -o 1xpad 1xpad.c`

# Usage

Redirect stdout to a file to save the output.

### Generating a key:

`./1xpad -g <key_size_in_mb>`

### Encrypt/decrypt a message:

`./1xpad keyfile messagefile`
