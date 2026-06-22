param(
    [string]$BaseUrl = "http://localhost:5001/lb",
    [int]$CountPerMethod = 50,
    [switch]$KeepSession
)

$methods = @("GET", "POST", "PUT", "DELETE")
$stats = @{}

$session = $null
if ($KeepSession) {
    $session = New-Object Microsoft.PowerShell.Commands.WebRequestSession
}

foreach ($method in $methods) {
    for ($i = 1; $i -le $CountPerMethod; $i++) {
        if ($session -ne $null) {
            $response = Invoke-RestMethod -Method $method -Uri $BaseUrl -WebSession $session
        }
        else {
            $response = Invoke-RestMethod -Method $method -Uri $BaseUrl
        }

        $nick = [string]$response.Nick

        if (-not $stats.ContainsKey($method)) {
            $stats[$method] = @{}
        }
        if (-not $stats[$method].ContainsKey($nick)) {
            $stats[$method][$nick] = 0
        }

        $stats[$method][$nick]++
    }
}

Write-Host "=== Result by HTTP method ==="
foreach ($method in $methods) {
    Write-Host "\n[$method]"
    foreach ($nick in ($stats[$method].Keys | Sort-Object)) {
        Write-Host "$nick : $($stats[$method][$nick])"
    }
}

Write-Host "\n=== Total for all methods ==="
$total = @{}
foreach ($method in $methods) {
    foreach ($nick in $stats[$method].Keys) {
        if (-not $total.ContainsKey($nick)) {
            $total[$nick] = 0
        }
        $total[$nick] += $stats[$method][$nick]
    }
}
foreach ($nick in ($total.Keys | Sort-Object)) {
    Write-Host "$nick : $($total[$nick])"
}
