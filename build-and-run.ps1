if ($args.Count -lt 1) {
	Write-Output "please provide folder name"
	return
}

$path = $args[0]
cmake --preset default $path
if ($LASTEXITCODE -ne 0) { return }
cmake --build $path/build
if ($LASTEXITCODE -ne 0) { return }

$exeName
$exeSearch = $(Get-ChildItem $path/build/*.exe)
if ($exeSearch.Count -gt 0) {
	$exeName = $exeSearch[0].Name
} else {
	$exeSearch = $(Get-ChildItem $path/build/*.bin)
	if ($exeSearch.Count -gt 0) {
		$exeName = $exeSearch[0].Name
		Write-Output "executing .bin"
	} else {
		Write-Output "no .exe or .bin found in build/ folder!"
		return
	}
}

$pcsxReduxPath = 'C:\Users\tplew\Documents\Other\ps1\pcsx-redux-nightly-20568.20241112.3-x64-cli\pcsx-redux.exe'
$duckstationPath = 'C:\Users\tplew\Documents\Other\ps1\duckstation-windows-x64-release\duckstation-qt-x64-ReleaseLTCG.exe'
& $duckstationPath $path/build/$exeName
# & $pcsxReduxPath -exe $path/build/$exeName -run