((Get-Content -path $OUTPUT_PB_SOURCE -Raw) -replace 'namespace public {','namespace public_ {') | Set-Content -Path $OUTPUT_PB_SOURCE
((Get-Content -path $OUTPUT_PB_SOURCE -Raw) -replace 'tinkoff::public::','tinkoff::public_::') | Set-Content -Path $OUTPUT_PB_SOURCE

((Get-Content -path $OUTPUT_PB_HEADER -Raw) -replace 'namespace public {','namespace public_ {') | Set-Content -Path $OUTPUT_PB_HEADER
((Get-Content -path $OUTPUT_PB_HEADER -Raw) -replace 'tinkoff::public::','tinkoff::public_::') | Set-Content -Path $OUTPUT_PB_HEADER

((Get-Content -path $OUTPUT_GRPC_SOURCE -Raw) -replace 'namespace public {','namespace public_ {') | Set-Content -Path $OUTPUT_GRPC_SOURCE
((Get-Content -path $OUTPUT_GRPC_SOURCE -Raw) -replace 'tinkoff::public::','tinkoff::public_::') | Set-Content -Path $OUTPUT_GRPC_SOURCE

((Get-Content -path $OUTPUT_GRPC_HEADER -Raw) -replace 'namespace public {','namespace public_ {') | Set-Content -Path $OUTPUT_GRPC_HEADER
((Get-Content -path $OUTPUT_GRPC_HEADER -Raw) -replace 'tinkoff::public::','tinkoff::public_::') | Set-Content -Path $OUTPUT_GRPC_HEADER


