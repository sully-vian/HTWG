    # Lab 3: End-to-end project with Node-RED


To start the Node-RED server, run:

```bash
node-red
```

## HTTPS

If you need more config and security:

```bash
node-red --settings ./settings.js;
```

Make sure your directory holds the `privkey.pem` and `cert.pem` files. If not, you can generate them with

```bash
openssl req -x509 -newkey rsa:2048 -nodes -keyout privkey.pem -out cert.pem -days 365
```

