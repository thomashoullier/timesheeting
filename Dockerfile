FROM gentoo/stage3:amd64-nomultilib-openrc
COPY --from=gentoo/portage:latest /var/db/repos/gentoo /var/db/repos/gentoo
RUN rm -rf /etc/portage/gnupg ; getuto
RUN MAKEOPTS="-j1" emerge --quiet --getbinpkg --usepkg dev-build/cmake dev-cpp/tclap sys-libs/ncurses dev-db/sqlite dev-cpp/tomlplusplus dev-cpp/rapidfuzz-cpp

ENTRYPOINT ["/bin/sh"]