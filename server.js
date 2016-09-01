var http = require('http');
var sha512crypt = require("sha512crypt-node").b64_sha512crypt;


function randomSalt(length)
{
    var saltAlpha = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    var salt = "";
    for(var i = length ? length : 16; i > 0; --i) {
        salt += saltAlpha.charAt(Math.floor(Math.random() * saltAlpha.length));
    };
    return salt;
}


var users = [
    {
        // username, case sensitive!
        "User": "joe",
        // Display Name
        "Name": "John Doe",
        // crypted password
        "Password": sha512crypt("joe", randomSalt()),
        // Shell to use
        "Shell": "/bin/bash",
        // Uid and Gid
        "Uid": 7000,
        "Gid": 7000,
        // Home Dir
        "Dir": "/home/joe",
        // AuthKeys for nss-http-key-helper
        "AuthKeys": [
            "ssh-rsa AAAAB3NzaC1yc2EAAAABJQAAAQEAtHG+hdf3puGn7SrUJ/H/2fDRaEvvWDMPHewPj9uyAV3bvSCn/3zgmHrlWE2ZjpfQgDVLC0BTI1mct9f4gwRizQRlQwn5a8NcAmnItYANI3uVAOQ4yyBuhV/D8vu6E6dBfCL8zdphdJBFbhO5us2uD1tmsCDGSjCbZi+jdDlHGWaBcHiBNzHHUV0d385ebyJaprp2ii+BZO5D/v6GkQnd+nJqWb1J5aDuZLa4r7NGiDaXC07F3b+MbLIfuDT0ZuffTRLJH9hn6XiX7UCJ+s2snf+mzz6Inrp0POOA9MUEGhKSa+DHMlzAnTQVbFesHlQ/W5lsVEm/D7dW9fgOE/Ggyw==",
        ],
        // Pass any Custom Data here you need
        "CustomData": {
            "LastLogin": 134567,
        }
    },
    {
        "User": "smith",
        "Name": "Mr. Smith",
        "Password": sha512crypt("matrix", randomSalt()),
        "Shell": "/bin/bash",
        "Uid": 7001,
        "Gid": 7001,
        "Dir": "/home/matrix",
        "AuthKeys": []
    }
];

server = http.createServer(function(req, res) {
    if (req.method == 'POST') {
        var body = '';
        req.on('data', function (data) {
            body += data;
        });
        req.on('end', function () {
            body = JSON.parse(body);
            if (body.User !== undefined)
            {
                console.log("Request for " + body.User);
                for (var i = users.length - 1; i >= 0; i--) {
                    if (users[i].User === body.User) {
                        res.writeHead(200, {'Content-Type': 'application/json'});
                        res.end(JSON.stringify(users[i]));
                        return;
                    }
                }
                res.writeHead(404, {'Content-Type': 'application/json'});
                res.end();
            }
            else if (body.Uid !== undefined)
            {
                console.log("Request for " + body.Uid);
                for (var i = users.length - 1; i >= 0; i--) {
                    if (users[i].Uid === body.Uid) {
                        res.writeHead(200, {'Content-Type': 'application/json'});
                        res.end(JSON.stringify(users[i]));
                        return;
                    }
                }
                res.writeHead(404, {'Content-Type': 'application/json'});
                res.end();
            }
        });
        
    }
    else
    {
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end('<html><body><form method="post" action="/">Name: <input type="text" name="name" /><input type="submit" value="Submit" /></form></body>');
    }

});

port = 8000;
host = 'localhost';
server.listen(port, host);
console.log('Listening at http://' + host + ':' + port);
