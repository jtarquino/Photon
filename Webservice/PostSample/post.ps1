$deviceId="xxx"
$accessToken="xxx"
$webMethodName = "setRGBColor"
$deviceUrl="https://api.particle.io/v1/devices/$deviceId/$webMethodName`?access_token=$accessToken"


Invoke-RestMethod -Uri $deviceUrl -Method Post -Body "args=255,255,000"


##loop
