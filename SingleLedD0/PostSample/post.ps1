$deviceId="xxxx"
$accessToken="xxxx"
$deviceUrl="https://api.particle.io/v1/devices/$deviceId/led?access_token=$accessToken"


Invoke-RestMethod -Uri $deviceUrl -Method Post -Body "args=on"


##loop



$ledIs="off"
do{Invoke-RestMethod -Uri $deviceUrl -Method Post -Body "args=$ledIs"; Start-Sleep -s 1; if($ledIs -eq "off"){$ledIs="on"}else{$ledIs="off"}}while (1 -eq 1)