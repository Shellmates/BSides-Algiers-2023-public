# Free Ram Installer

## Write-up

This is pseado writeup, A more detailed writeup will be coming soon.

Extract the backup using abe.jar

Explore the files, find the messages sent on the phone.

Download the app, reverse it.

When you arrive to the point where you will need the AES key used for encryption, you will have to extract it from the native library.

You can try to reverse it, but it might be difficult since the library is stripped.

The first solution is to use frida to get the key.

The second one is to create a new android studio project, and manually add the libraries, and then call the function getKey from class A.

Decrypt using cyberchef (AES ECB).

## Flag

`shellmates{android_&_foren$ics=<3}`
