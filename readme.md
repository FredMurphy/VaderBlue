# VaderBlue
## Summary
This project is a re-write of a previous [Lego Darth Vader build monitor](https://0xfred.wordpress.com/2013/10/29/animated-lego-darth-vader-build-monitor/) using the BeagleBone Blue. This didn't really need a re-write. To be honest I don't need a build monitor. The project is mainly to:
* Run .NET 8.0 on the BeagleBone. (.NET 7.0 had some issues with 32-bit Linux ARM.)
* Get experience of communicating between .NET Core and a Linux C library.
* Get experience of communicating between a Linux C library and the Sitara's Programmable Realtime Units.
* Get more in-depth experience working with the PRUs.