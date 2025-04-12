FROM gentoo_environment
RUN emerge --quiet --getbinpkg --usepkg dev-cpp/catch dev-util/cppcheck dev-util/include-what-you-use && env-update && source /etc/profile
ENTRYPOINT ["/bin/sh"]