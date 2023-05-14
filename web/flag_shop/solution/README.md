# Flags Shop

## Write-up

- By looking at the app, we can clearly see that the objective is to somehow get enough credits to but the flag that cost 40.99.
- The only way to get more money is to to use the redeem function but it can only be used once since the app update the `redeemed` status once you redeemed.
- The other interesting part of the app is that when you redeem a ticket, the app checks if it's valid by retrieving the regex from the jwt.
- After some search, you can find out that there is something called [ReDoS](https://owasp.org/www-community/attacks/Regular_expression_Denial_of_Service_-_ReDoS) which is basically a denial of service using regex.
- We also have this part of code before updating the balance and making `reedemed` to True.
```py
if checkRedeem(username) and checkTicket(regex, ticket):
    userRedeem(username)
```
- The app first checks if the user have reedemed a ticket then checks the regex after it will update the user information.
- The idea here is that if we give a regex that takes some time to match (like few seconds) and send few requests at the same time, they will all pass the function `checkRedeem` then stops at the function `checkTicket` because the regex takes some time. The will be stuck there once the matche function finishes all the requests will enter the condition block even after the first one updates the `redeemed` variable, it won't matter now and you'll have enough balnace to buy the flag.
- All what needs to be done is provide a malicious regex in order to do that you'll have to forge the jwt, this can be done by brute forcing the jwt secret key (use jwt2john and John using rockyou as worldlist).
- this is an example of a malicious regex `(.*a){24}` with `aaaaaaaaaaaaaaaaaaaaaaaaaaaa` as ticket, now just forge the ticket and send 4 request (the easiest way to do it is using curl `curl req1 & curl req2 ... )

## Flag

`shellmates{Jwt_X_r3d0s_b3ats_3v3ryth1ng}`
