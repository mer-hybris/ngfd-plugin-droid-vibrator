Name:       ngfd-plugin-native-vibrator
Summary:    Droid Vibrator native plugin for ngfd
Version:    1.2.1
Release:    1
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
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%make_install

%files
%defattr(-,root,root,-)
%doc README COPYING
%{_libdir}/ngf/libngfd_droid-vibrator.so
%{_datadir}/ngfd/plugins.d/50-droid-vibrator.ini
