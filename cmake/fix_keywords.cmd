::((Get-Content -path %1 -Raw) -replace 'namespace public {','namespace public_ {') | Set-Content -Path %1
::((Get-Content -path %1 -Raw) -replace 'tinkoff::public::','tinkoff::public_::') | Set-Content -Path %1

::((Get-Content -path %2 -Raw) -replace 'namespace public {','namespace public_ {') | Set-Content -Path %2
::((Get-Content -path %2 -Raw) -replace 'tinkoff::public::','tinkoff::public_::') | Set-Content -Path %2

::((Get-Content -path %3 -Raw) -replace 'namespace public {','namespace public_ {') | Set-Content -Path %3
::((Get-Content -path %3 -Raw) -replace 'tinkoff::public::','tinkoff::public_::') | Set-Content -Path %3

::((Get-Content -path %4 -Raw) -replace 'namespace public {','namespace public_ {') | Set-Content -Path %4
::((Get-Content -path %4 -Raw) -replace 'tinkoff::public::','tinkoff::public_::') | Set-Content -Path %4

((Get-Content -path instruments.pb.cc -Raw) -replace 'namespace public {','namespace public_ {') | Set-Content -Path instruments.pb.cc

