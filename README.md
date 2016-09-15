# nss_http


## Requirements
#### Redis server  
Currently there is just one cache algorithm implemented using redis.  
It is highly recmommended to use the redis server instead of no cache.  
Redis can be installed on the same server you install `nss_http`.  

#### A backend server
You need a server that serves the user data.  
You can use the sample server in this repo.


## Components
#### nss_http  
The library (duh!)

#### nss-http-key-helper  
A helper app that allows authentication using ssh-keys

#### nss-custom-data-example
Example app for your uses

## Building
1. Make sure you have following packages installed:
    * build-essential
    * libhiredis-dev
    * libjson-c-dev
    * libcurl4-openssl-dev
    (You can run `make prepare` to install all needed packages)
2. Adjust `shared/global.h` to your needs.
3. run `make install` to compile and install `nss_http` and the `nss-http-key-helper`  
    If you only want nss_http `cd nss_http` and run `make install` there

> Remove using `make uninstall`

-----

Modify `/etc/nsswitch.conf` and add `http` to `passwd` and `shadow`

    # /etc/nsswitch.conf
    passwd:         compat http
    group:          compat
    shadow:         compat http
    gshadow:        files
    ...

-----

> Security note: Keep in mind, that you might have set tokens, usernames and/or passwords in `shared/global.h`
> Clear or remove the file in production use.


## Testing
You can test the functionality by running

    $ getent passwd <username>


## SSH auth

Add following lines to your `sshd_config`

    AuthorizedKeysCommand /usr/bin/nss-http-key-helper
    AuthorizedKeysCommandUser nobody


## Limits
* nss_http can and will not request a whole user list.  
* nss_http accepts only valid JSON data as input.  
* nss_http should use a cache, either local or external.
* nss_http can not change passwords.  
* nss_http accepts all password formats. It depends on the distribution whether the password is correct or not.
* Usernames must match the regex `^[a-z][-_a-z0-9]*$` and *should* be lowercase.

## User Data format
`nss_http` sends a POST request with JSON encoded data

Either containing the username:

    {
        "User": "joe"
    }

Or containing the uid:

    {
        "Uid": 7000
    }

The backend server must respond with following JSON encoded data:

    {
        // username, case sensitive!
        "User": "joe",
        // Display Name
        "Name": "John Doe",
        // crypted password
        "Password": "$6$jhCNuhl2ycbWn2YZ$mzNzPcdXsBhp5VVM/3vTmbeg./tevYxkSZxN96Bz7LtBTDDrKekUyTAYfNexQ8mBQeqL7aCCLRjBUJmsl81i1/",
        // Shell to use
        "Shell": "/bin/bash",
        // Uid and Gid
        "Uid": 7000,
        "Gid": 7000,
        // Home Dir
        "Dir": "/home/joe",
        // AuthKeys for nss-http-key-helper
        "AuthKeys": [
            "ssh-rsa AAAAB3NzaC1yc2EAAAABJQAAAQEA0iftI+BgPr2F0aX1ajg89TVsGZ/nKvyayWSHpynE53s+5KABK7ns66hZyQWe/Q2g15Npebdz7xXES752ch3CtPiBOe//aehUZTYj/rxY9z7/JoYBYeoGeTHNtZ1D6IWnH3Aw9xeAPx1utHPMNRNkXiJWL7FaUKFMsLSK8irc/nyiJrJ5kIoGZBLMXu9CQY775DXP9xkT23nvV2RQw/9Sr20EFtSATqUrYab0/3V4Sb+W4fIAa4zGo75GC8Xpgb0gO96atJzVD8s3bpOFXl+F52JUZXyCU4B68gO+1Chv/TT6TuLU0jtVBJorvincc573NrmSNdhavmJBbHqJ6CXx5Q== rsa-key-20160901",
        ],
        // Pass any Custom Data here you need
        "CustomData": {
            "LastLogin": 134567,
        }
    }


## Other notes
* `html/` contains a sample for the browser to create crypted passwords
* run the sample server with `npm start` (might need `npm install` first)
* the sample backend server has two users: `joe` and `smith`

## TODO
* Implement a local cache algorithm

## Changelog
1.0.0: First release