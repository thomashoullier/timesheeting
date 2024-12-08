FROM gentoo/stage3:amd64-nomultilib-openrc
COPY --from=gentoo/portage:latest /var/db/repos/gentoo /var/db/repos/gentoo
RUN emerge sys-libs/ncurses

ENTRYPOINT ["/bin/sh"]