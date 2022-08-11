#!/usr/bin/pwsh
Get-ChildItem $args[0] -Recurse | Where-Object {$_.Extension -in ".efi",".elf",".o",".d",".lib"} | Remove-Item