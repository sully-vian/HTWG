module.exports = {
    flowFile: "flows.json",
    flowFilePretty: true,
    https: {
        key: require("node:fs").readFileSync("privkey.pem"),
        cert: require("node:fs").readFileSync("cert.pem"),
    },
};
