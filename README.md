### install gcc gtk3 cmake ###
tdm-gcc32-5.1.0
gtk+-bundle_3.10.4-20131202_win32
cmake-3.5.0

### MSYS2 ###
pacman -Syu
pacman -S --needed filesystem msys2-runtime bash libreadline libiconv libarchive libgpgme libcurl pacman ncurses libintl base-devel vim 
pacman -S mingw-w64-i686-gtk3
pacman -S  mingw-w64-x86_64-toolchain mingw-w64-i686-toolchain mingw-w64-i686-gtk3 mingw-w64-x86_64-gtk3
mingw-w64-i686-gcc

### cmake ###
./bootstrap --prefix="/D/workspace_CL/Trends/solo_opensource_lib/cmake-3.5.0"
make && make install
MSYS Makefiles																	pacman -S make
export PATH=$PATH:/d/workspace_CL/Trends/solo_opensource_lib/cmake-3.5.0/bin;

pacman -Ss cmake binutils

### cmake ###
ld --verbose | grep SEARCH_DIR | tr -s ' ;' \\012

### zlib ###
make -fwin32/Makefile.gcc --prefix=$PWD/dist -no-undefined
make install -fwin32/Makefile.gcc DESTDIR=$PWD/dists/ INCLUDE_PATH=include LIBRARY_PATH=lib BINARY_PATH=bin
make install -fwin32/Makefile.gcc DESTDIR=$PWD/distd/ INCLUDE_PATH=include LIBRARY_PATH=bin BINARY_PATH=bin SHARED_MODE=1
DESTDIR="$PWD/dist"  -no-undefined

### openssl ###
./Configure --prefix=$PWD/dist no-idea no-mdc2 no-rc5 shared mingw(64)
make depend && make && make install
add .bashrc
	export PATH="$PATH:/d/workspace_CL/Trends/solo_opensource_src/openssl-1.0.2g/dist/bin"
	export INCLUDE="$INCLUDE:/d/workspace_CL/Trends/solo_opensource_src/openssl-1.0.2g/dist/include"
	export LIB="$LIB:/d/workspace_CL/Trends/solo_opensource_src/openssl-1.0.2g/dist/lib"
	export PATH="$PATH:/d/workspace_CL/Trends/solo_opensource_src/curl-7.48.0/dist/bin"
	export INCLUDE="$INCLUDE:/d/workspace_CL/Trends/solo_opensource_src/curl-7.48.0/dist/include"
	export LIB="$LIB:/d/workspace_CL/Trends/solo_opensource_src/curl-7.48.0/dist/lib"	
	LD_RUN_PATH
	C_INCLUDE_PATH		CPLUS_INCLUDE_PATH
	LD_LIBRARY_PATH
	LIBRARY_PATH

### libssh2 ###
./configure --prefix=$PWD/dist --with-openssl --disable-examples-build		//--with-libz 
make && make install

### curl ###
./configure --prefix=$PWD/dist
make && make install

### GTK3 ###
gtk3-runtime-3.18.6-2016-01-10-ts-win64.exe
	adwaita icon theme (3.18.0)
	atk (2.18.0)
	atkmm (2.24.2)
	cairo (1.14.6)
	cairomm (1.12.0)
	fontconfig (2.11.1)
	freetype (2.6.2)
	gdk-pixbuf (2.32.3)
	gettext (0.19.7)
	glib (2.46.2)
	glibmm (2.46.3)
	gtk2 (2.24.29)
	gtk3 (3.18.6)
	gtkmm2 (2.24.4)
	gtkmm3 (3.18.0)
	harfbuzz (1.1.2)
	hicolor-icon-theme (0.15)
	icon-naming-utils (0.8.90)
	intltool (0.51.0)
	json-glib (1.0.4)
	libepoxy (1.3.1)
	libffi (3.2.1)
	libpng (1.6.20)
	libsigc++ (2.6.2)
	libxml2 (2.9.3)
	libxml++ (2.38.1)
	libxslt (1.1.28)
	mm-common (0.9.9)
	pango (1.38.1)
	pangomm (2.38.1)
	pixman (0.32.8)
	pkg-config (0.29)
	win-iconv (0.0.6)
	zlib (1.2.8)

### Trends ###
pkg-config --cflags gtk+-3.0
	-mms-bitfields -pthread -mms-bitfields -I/mingw32/include/gtk-3.0 -I/mingw32/include/cairo -I/mingw32/include -I/mingw32/include/pango-1.0 -I/mingw32/include/atk-1.0 -I/mingw32/include/cairo -I/mingw32/include/pixman-1 -I/mingw32/include -I/mingw32/include/freetype2 -I/mingw32/include/libpng16 -I/mingw32/include/harfbuzz -I/mingw32/include/glib-2.0 -I/mingw32/lib/glib-2.0/include -I/mingw32/include -I/mingw32/include/freetype2 -I/mingw32/include -I/mingw32/include/harfbuzz -I/mingw32/include/libpng16 -I/mingw32/include/gdk-pixbuf-2.0 -I/mingw32/include/libpng16 -I/mingw32/include/glib-2.0 -I/mingw32/lib/glib-2.0/include
pkg-config --libs gtk+-3.0
	-L/mingw32/lib -LC:/building/msys64/mingw32/lib -L/mingw32/lib -LC:/building/msys64/mingw32/lib/../lib -L/mingw32/lib -lgtk-3 -lgdk-3 -lgdi32 -limm32 -lshell32 -lole32 -Wl,-luuid -lwinmm -ldwmapi -lz -lepoxy -lpangocairo-1.0 -lpangoft2-1.0 -lpangowin32-1.0 -lgdi32 -lusp10 -lpango-1.0 -lm -latk-1.0 -lcairo-gobject -lcairo -lz -lpixman-1 -lfontconfig -lexpat -lfreetype -liconv -lexpat -lfreetype -lz -lbz2 -lharfbuzz -lpng16 -lz -lgdk_pixbuf-2.0 -lpng16 -lz -lgio-2.0 -lz -lgmodule-2.0 -pthread -lgobject-2.0 -lffi -lglib-2.0 -lintl -pthread -lws2_32 -lole32 -lwinmm -lshlwapi -lintl

### Publish ###
git init
git add .
git commit -m "first commit"
git remote add origin git@github.com:soloxyq/trends.git
git push -u origin master

### 附录I ###
pacman -Sy abc					#和源同步后安装名为abc的包 
pacman -S  abc					#从本地数据库中得到abc的信息，下载安装abc包 
pacman -Sf abc                  #强制安装包abc 
pacman -Ss abc                  #搜索有关abc信息的包 
pacman -Si abc                  #从数据库中搜索包abc的信息 
pacman -Q						# 列出已经安装的软件包
pacman -Q abc					# 检查 abc 软件包是否已经安装
pacman -Qi abc                  #列出已安装的包abc的详细信息 
pacman -Ql abc					# 列出abc软件包的所有文件
pacman -Qo /path/to/abc			# 列出abc文件所属的软件包
pacman -Syu						#同步源，并更新系统 
pacman -Sy						#仅同步源 
pacman -Su						#更新系统
pacman -R   abc					#删除abc包 
pacman -Rd abc					#强制删除被依赖的包
pacman -Rc abc                  #删除abc包和依赖abc的包 
pacman -Rsc abc					#删除abc包和abc依赖的包 
pacman -Sc						#清理/var/cache/pacman/pkg目录下的旧包 
pacman -Scc						#清除所有下载的包和数据库 
pacman -U   abc					#安装下载的abs包，或新编译的abc包
pacman -Sd abc					#忽略依赖性问题，安装包abc 
pacman -Su --ignore foo			#升级时不升级包foo 
pacman -Sg abc					#查询abc这个包组包含的软件包	

### 附录II ###
curl version:     7.48.0
Host setup:       i686-pc-mingw32
Install prefix:   /d/workspace_CL/Trends/solo_opensource_src/curl-7.48.0/dist
Compiler:         gcc
SSL support:      enabled (OpenSSL)
SSH support:      enabled (libSSH2)
zlib support:     enabled
GSS-API support:  no      (--with-gssapi)
TLS-SRP support:  enabled
resolver:         default (--enable-ares / --enable-threaded-resolver)
IPv6 support:     enabled
Unix sockets support: no      (--enable-unix-sockets)
IDN support:      no      (--with-{libidn,winidn})
Build libcurl:    Shared=yes, Static=yes
Built-in manual:  enabled
--libcurl option: enabled (--disable-libcurl-option)
Verbose errors:   enabled (--disable-verbose)
SSPI support:     no      (--enable-sspi)
ca cert bundle:   no
ca cert path:     no
ca fallback:      no
LDAP support:     enabled (winldap)
LDAPS support:    enabled
RTSP support:     enabled
RTMP support:     no      (--with-librtmp)
metalink support: no      (--with-libmetalink)
PSL support:      no      (libpsl not found)
HTTP2 support:    disabled (--with-nghttp2)
Protocols:        DICT FILE FTP FTPS GOPHER HTTP HTTPS IMAP IMAPS LDAP LDAPS POP3 POP3S RTSP SCP SFTP SMB SMBS SMTP SMTPS TELNET TFTP
