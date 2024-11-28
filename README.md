# ft_ssl
Simple OpenSSL implementation in C

## md5


- [x] -p, echo STDIN to STDOUT and append the checksum to STDOUT
- [x] -q, quiet mode
- [x] -r, reverse the format of the output.
- [x] -s, print the sum of the given string

### -p

**echo STDIN to STDOUT and append the checksum to STDOUT**

#### Examples

```
> echo "42 is nice" | ./ft_ssl md5 -p
("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9
> echo "42 is nice" | ./ft_ssl md5 -p -q
42 is nice
35f1d6de0302e2086a4e472266efb3a9
> echo "42 is nice" | ./ft_ssl md5 -p -r
("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9
> echo "42 is nice" | ./ft_ssl md5 -p -s "pity those that aren't following baerista on spotify."
("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9
MD5 ("pity those that aren't following baerista on spotify.") = a3c990a1964705d9bf0e602f44572f5f
```

### -q

**quiet mode**

#### Examples

```
> echo "42 is nice" | ./ft_ssl md5 -q
35f1d6de0302e2086a4e472266efb3a9
> echo "42 is nice" | ./ft_ssl md5 -p -q
42 is nice
35f1d6de0302e2086a4e472266efb3a9
> echo "42 is nice" | ./ft_ssl md5 -q -r
35f1d6de0302e2086a4e472266efb3a9
> ./ft_ssl md5 -q -s "42 is nice"
35f1d6de0302e2086a4e472266efb3a9
```

### -r

**reverse the format of the output**

#### Examples

```
> echo "42 is nice" | ./ft_ssl md5 -r
35f1d6de0302e2086a4e472266efb3a9 *stdin
> echo "42 is nice" | ./ft_ssl md5 -r -q
35f1d6de0302e2086a4e472266efb3a9
> ./ft_ssl md5 -r -s "42 is nice"
35f1d6de0302e2086a4e472266efb3a9 "42 is nice"
> ./ft_ssl md5 -r file
35f1d6de0302e2086a4e472266efb3a9 file
> echo "42 is nice" | ./ft_ssl md5 -p -r
("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9
```

### -s

**print the sum of the given string**

```
> ./ft_ssl md5 -s "42 is nice"
MD5 ("42 is nice") = 35f1d6de0302e2086a4e472266efb3a9
> ./ft_ssl md5 -q -s "42 is nice"
35f1d6de0302e2086a4e472266efb3a9
> ./ft_ssl md5 -r -s "42 is nice"
35f1d6de0302e2086a4e472266efb3a9 "42 is nice"
```

### Other examples

```
> ./ft_ssl md5 file
MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a
> echo "one more thing" | ./ft_ssl md5 -r -p -s "foo" file -s "bar"
("one more thing")= a0bd1876c6f011dd50fae52827f445f5
acbd18db4cc2f85cedef654fccc4a4d8 "foo"
53d53ea94217b259c11a5a2d104ec58a file
ft_ssl: md5: -s: No such file or directory
ft_ssl: md5: bar: No such file or directory
```

## sha256

- [x] -p, echo STDIN to STDOUT and append the checksum to STDOUT
- [x] -q, quiet mode
- [x] -r, reverse the format of the output.
- [x] -s, print the sum of the given string

The flags are the same and their behavior too.

```
> echo "https://www.42.fr/" > website
> ./ft_ssl sha256 -q website
1ceb55d2845d9dd98557b50488db12bbf51aaca5aa9c1199eb795607a2457daf
> sha256sum website
1ceb55d2845d9dd98557b50488db12bbf51aaca5aa9c1199eb795607a2457daf website
>
> ./ft_ssl sha256 -s "42 is nice"
SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f
> echo -n "42 is nice" | sha256sum
b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f -
```