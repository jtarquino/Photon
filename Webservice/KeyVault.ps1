Login-AzureRmAccount
Select-AzureRmSubscription -subscriptionid 'XXXXXXX'
New-AzureRmKeyVault -VaultName 'BIProSSRS' -ResourceGroupName 'BIPro_SSRS' -Location 'West US 2'


$key = Add-AzureKeyVaultKey -VaultName 'BIProSSRS' -Name 'ArubaKey' -Destination 'Software'

$secretvalue = ConvertTo-SecureString 'ThePassword' -AsPlainText -Force
$secret = Set-AzureKeyVaultSecret -VaultName 'BIProSSRS' -Name 'ArubaPwd' -SecretValue $secretvalue