@echo off
set BASE=http://localhost:3000

curl -s -X GET %BASE%/A >NUL
curl -s -X GET %BASE%/A/B >NUL
curl -s -X POST %BASE%/A >NUL
curl -s -X POST %BASE%/A/B >NUL
curl -s -X PUT %BASE%/A >NUL
curl -s -X PUT %BASE%/A/B >NUL

curl -s -X GET %BASE%/other >NUL
curl -s -X POST %BASE%/other >NUL
curl -s -X PUT %BASE%/other >NUL

echo done
