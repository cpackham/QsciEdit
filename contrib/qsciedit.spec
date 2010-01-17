#
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#

%define git_repodir %(echo ~/src/)
%define git_gitdir %{git_repo}/.git
%define git_head HEAD
%define git_rpm_ver %(git --git-dir=%{git_gitdir} describe --tags | sed 's/\\([0-9]\\+\\)\\.\\([0-9]\\+\\).*/\\1.\\2/')
%define git_repo_ver %(git describe --always)

%define git_get_source pushd %{git_repo} ;\
        /usr/bin/git archive --format=tar --prefix=%{name}-%{version}/ %{git_head} | \
                gzip -c > %{_sourcedir}/%{name}-%{version}.tar.gz ;\
        popd

%define git_repo %{git_repodir}/qsciapp

%define name	qsciedit

# norootforbuild

Name:           %{name}
Version:	%{git_rpm_ver}
Release:	1
Summary:	Editor
Group:		Utilities/Editors
License:	GPL
Url:		http://localhost/
PreReq:		libqscintilla2-5
Provides:	%{name}
BuildRequires:	qt-devel, libqscintilla-devel
Source:		%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
AutoReqProv:    on

%description
Simple Source Code Editor.

Supports syntax highlighting for C/C++, Java, Python, Perl, BASH, HTML, CSS
and many more.

Authors:
--------
    Chris Packham

%prep
%git_get_source
%setup

%build
export VERSION=%{git_repo_ver} 
qmake
make %{?jobs:-j%jobs} release
unset VERSION

%install
make install INSTALL_ROOT=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%post
%postun

%files
%defattr(-,root,root)
/usr/bin/%{name}

%changelog
* Sat Jan 16 2010 judge.packham@gmail.com
First release
