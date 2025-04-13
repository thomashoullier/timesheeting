FROM gentoo_environment
RUN echo "dev-util/lcov ~amd64" > /etc/portage/package.accept_keywords/lcov ; emerge --quiet --getbinpkg --usepkg dev-cpp/catch dev-util/cppcheck dev-util/include-what-you-use dev-util/lcov
ENTRYPOINT ["/bin/sh"]