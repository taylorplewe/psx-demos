# Getting set up for PSX development
(Specifically for this repo)

1. First off, if you don't already have PSn00bSDK, grab a [pre-built copy](https://github.com/Lameguy64/PSn00bSDK/releases)
1. Make sure `cmake` is on your PATH. Entering a Visual Studio dev environment ensures this.
1. Create an environment variable `PSN00BSDK_LIBS` that points to your `psn00bsdk/lib/libpsn00b` folder

For ease of use, I created a PowerShell function that ensures all of this automatically:
```powershell
function psxdev {
	devps # enters a VS 2022 dev environment
	$env:PATH = "$($env:PATH);C:\Users\me\ps1\PSn00bSDK-0.24-win32\bin"
	$env:PSN00BSDK_LIBS = "C:\Users\me\ps1\PSn00bSDK-0.24-win32\lib\libpsn00b"
}
```

Once the above steps are complete, you should be able to run `./build-and-run.ps1 <folder name>` from the root `psx-demos` directory and have it work.

I've gotten Duckstation and pscx-redux to work with the `.exe`s the build script spits out, but not no$psx.
