# syntax=docker/dockerfile:1-labs
FROM devuan/migrated:excalibur-slim

COPY . /calamares

# Update packages
RUN apt update && apt upgrade -y

# Install debhelper
RUN apt install -y debhelper

WORKDIR "/calamares"

# Install dependencies
RUN ./ci/deps-debian11.sh

# Fuck up dpkg-source
RUN echo -e '#!/bin/sh\nexit 0' >/bin/dpkg-source

# Move debian folder to top
RUN mv ./ci/debian .

RUN dpkg-buildpackage
