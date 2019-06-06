Name:       ngfd-plugin-native-vibrator
Summary:    Droid Vibrator native plugin for ngfd
Version:    1.1
Release:    1
Group:      System/Daemons
License:    LGPLv2+
URL:        https://github.com/mer-hybris/ngfd-plugin-droid-vibrator
Source:     %{name}-%{version}.tar.gz
Requires:   ngfd >= 1.0
BuildRequires:  cmake
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(ngf-plugin) >= 1.0
Conflicts:  ngfd-plugin-droid-vibrator

%description
This package contains the Droid Vibrator plugin
for the non-graphical feedback daemon.

%prep
%setup -q -n %{name}-%{version}

%build
%cmake -DNATIVE_VIBRATOR=ON
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post
if [ "$1" -ge 1 ]; then
    systemctl-user daemon-reload || true
    systemctl-user restart ngfd.service || true
fi

%postun
if [ "$1" -eq 0 ]; then
    systemctl-user stop ngfd.service || true
    systemctl-user daemon-reload || true
fi

%files
%defattr(-,root,root,-)
%doc README COPYING
%{_libdir}/ngf/libngfd_droid-vibrator.so
%{_datadir}/ngfd/plugins.d/50-droid-vibrator.ini
