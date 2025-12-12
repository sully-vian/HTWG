#/bin/env sh

JSON=$(
curl "https://euvdservices.enisa.europa.eu/api/search?\
product=Moodle&\
fromDate=2025-01-01&\
toDate=2025-12-31&\
fromScore=8"
)

echo $JSON | jq > moodle-vulnerabilities.json
