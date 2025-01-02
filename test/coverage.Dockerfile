FROM test_environment
RUN ACCEPT_KEYWORDS="~amd64" emerge --quiet dev-util/lcov
ENTRYPOINT ["/bin/sh"]