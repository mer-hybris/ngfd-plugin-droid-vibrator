Name:       ngfd-plugin-droid-vibrator
Summary:    Droid Vibrator HAL plugin for ngfd
Version:    0.91.1
Release:    1
Group:      System/Daemons
License:    LGPL 2.1
URL:        https://github.com/mer-hybris/ngfd-plugin-droid-vibrator
Source:     %{name}-%{version}.tar.gz
Requires:   ngfd >= 0.91
BuildRequires:  cmake
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(ngf-plugin) >= 0.91
BuildRequires:  pkgconfig(android-headers)
BuildRequires:  pkgconfig(libvibrator)
BuildRequires:  pkgconfig(libhardware)

%description
This package contains the Droid Vibrator plugin
for the non-graphical feedback daemon.

%prep
%setup -q -n %{name}-%{version}

%build
%cmake
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
