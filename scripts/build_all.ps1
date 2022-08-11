#!/usr/bin/pwsh
$ErrorActionPreference = "stop"
.\scripts\clean.ps1 .
.\scripts\build_bootloader.ps1
.\scripts\build_kernel.ps1
