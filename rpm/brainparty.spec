Name: harbour-brainparty
Version: 0.5.992
Release: 1
Summary: A puzzle game to tease your brain
Group: Games
License: Proprietary
Url: http://thp.io/2012/brainparty/
Source: %{name}-%{version}.tar.bz2
BuildRequires: pkgconfig(sdl2)
BuildRequires: pkgconfig(SDL2_ttf)
BuildRequires: pkgconfig(SDL2_image)
BuildRequires: pkgconfig(SDL2_mixer)
BuildRequires: pkgconfig(audioresource)
BuildRequires: pkgconfig(glesv1_cm)
BuildRequires: pkgconfig(egl)
BuildRequires: pkgconfig(glib-2.0)

%description
Brain Party is a fun, free puzzle game for all the family that's made up of 36
minigames designed to push your brain to its limits by testing memory, logic,
mathematics, reaction time and more!

%prep
%setup -q

%build
make clean
make

%install
install -D brainparty %{buildroot}%{_bindir}/%{name}
install -D -m644 brainparty.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop
install -D -m644 brainparty.png %{buildroot}%{_datadir}/icons/hicolor/86x86/apps/%{name}.png
mkdir -p %{buildroot}%{_datadir}/%{name}/
cp -rv Content/* %{buildroot}%{_datadir}/%{name}/

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
%{_datadir}/%{name}
