FROM gentoo/stage3:amd64-nomultilib-openrc
COPY --from=gentoo/portage:latest /var/db/repos/gentoo /var/db/repos/gentoo
RUN rm -rf /etc/portage/gnupg ; getuto
RUN mkdir -p /etc/portage/env && echo MAKEOPTS=\"-j2\" > /etc/portage/env/j2.conf && echo 'llvm-core/clang j2.conf' > /etc/portage/package.env
RUN emerge --quiet --getbinpkg --usepkg dev-cpp/tclap sys-libs/ncurses dev-db/sqlite dev-cpp/tomlplusplus dev-cpp/rapidfuzz-cpp

ENTRYPOINT ["/bin/sh"]