FROM gentoo/stage3:amd64-nomultilib-openrc
RUN emerge sys-libs/ncurses

ENTRYPOINT ["/bin/sh"]