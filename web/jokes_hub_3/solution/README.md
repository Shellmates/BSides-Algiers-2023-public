# Jokes Hub 3

## Write-up

- Now we'll read the source code for the `flagger.py`.
- As we can see we'll only need to get `secret` in order to get the flag.
- Since the `secret` is in the app envioronment, all we need to do is get the pid of the app then read `/proc/$pid/environ` using the function provided by flagger.
- Problem is that we don't know the pid of the app, so we'll just bruteforce it.
- Once you got the secret, send it to the app to get the flag.

## Flag 
`Shellmates{A_L0ng_waY-Fr0m_LAUGHING_AT_J0K3$_T0_My_SYSTEM}`