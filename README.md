This is a multi-threaded CPU miner for [BitZeny (ZNY)](https://bitzeny.tech/) using the [Yescrypt Algorithm](http://www.openwall.com/presentations/BSidesLjubljana2017-Yescrypt-Large-scale-Password-Hashing/BSidesLjubljana2017-Yescrypt-Large-scale-Password-Hashing.pdf)

(fork of Jeff Garzik's reference cpuminer https://github.com/noncepool/cpuminer-yescrypt)

License: GPLv2.  See COPYING for details.

Git tree:   https://github.com/KotoDevelopers/cpuminer-yescrypt/

Windows Binary:

Dependencies:
- `libcurl`			[http://curl.haxx.se/libcurl/](http://curl.haxx.se/libcurl/)
- `jansson`			[http://www.digip.org/jansson/](http://www.digip.org/jansson/) (`jansson` is included in the git tree)

Install Build Dependencies on Debian, Ubuntu and other APT-based distros:
```
    sudo apt-get install build-essential libcurl4-openssl-dev autotools-dev automake
```

Install Build Dependencies on Fedora, RHEL, CentOS and other yum-based distros:
```
    sudo yum install gcc make curl-devel automake
```

Install Build Dependencies on OpenSUSE and other ZYpp-based distros:
```
    sudo zypper in gcc make libcurl-devel automake
```

Basic *nix build instructions:
```
./autogen.sh    # Only needed if building from git repo.
./nomacro.pl    # Only needed if building on Mac OS X or with Clang.
./configure CFLAGS="-O3" # For ARM-based devices, please, use
                         # CFLAGS="-O3 -funroll-loops -fomit-frame-pointer"
                         # Alternatively, check out the _arm_ branch of this
                         # repository, and build the project from there.
make
```

Notes for AIX users:

	* To build a 64-bit binary, export OBJECT_MODE=64
	* GNU-style long options are not supported, but are accessible
	  via configuration file


Windows x64 (64-bit) build instructions on Ubuntu 16.04LTS / 17.10:
```
	sudo apt-get install gcc-mingw-w64
        cd depend
        sh depend.sh
        cd ..
        ./autogen.sh
        LDFLAGS="-L./depend/curl-7.40.0-devel-mingw64/lib64 -static" LIBCURL="-lcurldll" CFLAGS="-O3 -msse4.1 -funroll-loops -fomit-frame-pointer" ./configure --host=x86_64-w64-mingw32 --with-libcurl=depend/curl-7.40.0-devel-mingw64
        make


        * Static version

        cd deps
        ./build_win_x64_deps.sh
        cd ..
        autoreconf -fi -I./deps/x86_64-w64-mingw32/share/aclocal
        LDFLAGS="-L./deps/x86_64-w64-mingw32/lib -static" CFLAGS="-O3 -msse4.1 -funroll-loops -fomit-frame-pointer -I./deps/x86_64-w64-mingw32/include -std=c99 -DWIN32 -DCURL_STATICLIB -DPTW32_STATIC_LIB" ./configure --host=x86_64-w64-mingw32 --with-libcurl=deps/x86_64-w64-mingw32
        make
```

Windows x86 (32-bit) build instructions on Ubuntu 16.04LTS / 17.10:
```
	sudo apt-get install gcc-mingw-w64
        cd depend
        sh depend.sh
        cd ..
        ./autogen.sh
        LDFLAGS="-L./depend/curl-7.40.0-devel-mingw32/lib -static" LIBCURL="-lcurldll" CFLAGS="-O3 -msse4.1 -funroll-loops -fomit-frame-pointer" ./configure --host=i686-w64-mingw32 --with-libcurl=depend/curl-7.40.0-devel-mingw32
        make


        * Static version

        cd deps
        ./build_win_x86_deps.sh
        cd ..
        autoreconf -fi -I./deps/i686-w64-mingw32/share/aclocal
        LDFLAGS="-L./deps/i686-w64-mingw32/lib -static" CFLAGS="-O3 -msse4.1 -funroll-loops -fomit-frame-pointer -I./deps/i686-w64-mingw32/include -std=c99 -DWIN32 -DCURL_STATICLIB -DPTW32_STATIC_LIB" ./configure --host=i686-w64-mingw32 --with-libcurl=deps/i686-w64-mingw32
        make
```

Architecture-specific notes:

	ARM:	No runtime CPU detection. The miner can take advantage
		of some instructions specific to ARMv5E and later processors,
		but the decision whether to use them is made at compile time,
		based on compiler-defined macros.

		To use NEON instructions, add "-mfpu=neon" to CFLAGS.

	x86:	The miner checks for SSE2 instructions support at runtime,
		and uses them if they are available.

	x86-64:	(Note: SSE works better than AVX and AVX2 for Yescrypt)
		The miner can take advantage of AVX, AVX2 and XOP instructions,
		but only if both the CPU and the operating system support them.

		    * Linux supports AVX starting from kernel version 2.6.30.
		    * FreeBSD supports AVX starting with 9.1-RELEASE.
		    * Mac OS X added AVX support in the 10.6.8 update.
		    * Windows supports AVX starting from Windows 7 SP1 and
		      Windows Server 2008 R2 SP1.

		The configure script outputs a warning if the assembler
		doesn't support some instruction sets. In that case, the miner
		can still be built, but unavailable optimizations are left off.

Usage instructions:  Run `minerd --help` to see options.

To Connect to pool using your BitZeny Wallet Address:
```
    ./minerd -a yespower -o stratum+tcp://zny.pool.address:port -u Your_BitZeny_Address -p x
```

Note: Do not mine to online wallet addresses, including exchanges, as you may experience loss of coins due to how newly mined coins are confirmed.

Connecting through a proxy:  Use the `--proxy` option.

To use a SOCKS proxy, add a `socks4://` or `socks5://` prefix to the proxy host.

Protocols `socks4a` and `socks5h`, allowing remote name resolving, are also
available since `libcurl 7.18.0`.

If no protocol is specified, the proxy is assumed to be a HTTP proxy.

When the `--proxy` option is not used, the program honors the http_proxy
and all_proxy environment variables.

Also many issues and FAQs are covered in these forum threads dedicated to this program:

* [Original Pooler CPUMiner Bitcointalk Thread](https://bitcointalk.org/index.php?topic=55038.0)
