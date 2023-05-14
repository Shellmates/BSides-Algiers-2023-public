# Jokes Hub 2

## Write-up

- When retrieving other tables, you can find a table called `notes` which contains few notes one of them is `unload fileio from the production server`.
- After searching, you can find out that [fileio](https://github.com/nalgeon/sqlean/blob/main/docs/fileio.md) allows access to the filesystem so you can list and read files.
- After listing files using `fileio_ls` you'll find an `nginx.conf` file which you can read like this:
```json
{"hex(fileio_read('../nginx.conf'))--":5}
```
- You'll need to add `hex` since `fileio_read` returns a blob and not a string.
- Reading the nginx.conf will show that there is a second app called `flagger` that can be accesss by changing the `user-agent` and we also have the filepath for it.
- Checking the source of the `flagger.py` will reveal the the second flag.

## Flag

`shellmates{it5_l1k3_5q1i73_0n_573r01d5}`