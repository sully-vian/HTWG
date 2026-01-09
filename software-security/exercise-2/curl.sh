#/bin/env bash

JSON=$(
curl "https://euvdservices.enisa.europa.eu/api/search?\
product=openssl&\
vendor=openssl&\
fromDate=2025-01-01&\
toDate=2025-12-31"
)

echo $JSON | jq > openssl-vulnerabilities.json
